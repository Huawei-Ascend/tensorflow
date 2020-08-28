/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "ExternalSoftDp.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tf_adapter/util/plugin_load_manager.h"
#include <dlfcn.h>

namespace tensorflow {
using fDecodeAndResizeJpeg = uint32_t (*)(SoftDpProcsessInfo &);
using fDecodeAndCropAndResizeJpeg = uint32_t (*)(SoftDpProcsessInfo &, DpCropInfo &);
class DecodeImageOp : public OpKernel {
 public:
  explicit DecodeImageOp(OpKernelConstruction *context) : OpKernel(context) {
    if (type_string() == "DecodeAndResizeJpeg") {
      crop_ = false;
    } else if (type_string() == "DecodeAndCropAndResizeJpeg") {
      crop_ = true;
    } else {
      OP_REQUIRES_OK(context, errors::InvalidArgument("Bad op type ", type_string()));
    }
    string lib_path = PluginLoadManager::GetTFPluginRealPath();
    if (lib_path.empty()) { OP_REQUIRES_OK(context, errors::InvalidArgument("Find tf-plugin path failed!")); }
    lib_path += "libSoftDp.so";
    handle_ = PluginLoadManager::DlOpen(lib_path);
    void *decode_and_resize_jpeg = PluginLoadManager::DlSym(handle_, "DecodeAndResizeJpeg");
    void *decode_and_crop_and_resize_jpeg = PluginLoadManager::DlSym(handle_, "DecodeAndCropAndResizeJpeg");
    if (decode_and_resize_jpeg == nullptr || decode_and_crop_and_resize_jpeg == nullptr) {
      OP_REQUIRES_OK(context, errors::InvalidArgument("Can not find function from ", lib_path));
    }
    decode_resize_ = (fDecodeAndResizeJpeg) decode_and_resize_jpeg;
    decode_crop_resize_ = (fDecodeAndCropAndResizeJpeg) decode_and_crop_and_resize_jpeg;
  }
  ~DecodeImageOp() override {
    if (handle_ != nullptr) {
      dlclose(handle_);
      LOG(INFO) << "dlclose handle finish.";
    } else {
      LOG(INFO) << "handle is null.";
    }
  }

  void Compute(OpKernelContext *context) override {
    const Tensor &contents = context->input(0);
    OP_REQUIRES(context, TensorShapeUtils::IsScalar(contents.shape()),
                errors::InvalidArgument("contents must be scalar, got shape ", contents.shape().DebugString()));

    const StringPiece input = contents.scalar<tstring>()();

    Tensor *output = nullptr;
    Tensor resize;
    // if crop_ is true, inputs tensor vector: [image, crop_size, size]
    // if crop_ is false, inputs tensor vector: [image, size]
    if (crop_) {
      resize = context->input(2);
    } else {
      resize = context->input(1);
    }
    auto resize_vec = resize.vec<int32>();
    int height = resize_vec(0);
    int width = resize_vec(1);
    Status status(context->allocate_output(0, TensorShape({height, width, 3}), &output));
    if (!status.ok()) {
      LOG(ERROR) << "DecodeImageOp, Decode image failed when allocate output";
      context->SetStatus(status);
      return;
    }

    SoftDpProcsessInfo dvppInfo = {0};
    dvppInfo.inputBuffer = reinterpret_cast<uint8_t *>(const_cast<char *>(input.data()));
    dvppInfo.inputBufferSize = input.size();
    dvppInfo.outputBuffer = reinterpret_cast<uint8_t *>(output->flat<uint8>().data());
    dvppInfo.outputBufferSize = height * width * 3;  // 3 means RGB
    dvppInfo.outputHeight = height;
    dvppInfo.outputWidth = width;

    int ret = 0;
    if (crop_) {
      // Update flags to include crop window.
      const Tensor &crop_window = context->input(1);
      OP_REQUIRES(context, crop_window.dims() == 1,
                  errors::InvalidArgument("crop_window must be 1-D, got shape ", crop_window.shape().DebugString()));
      OP_REQUIRES(context, crop_window.dim_size(0) == 4,
                  errors::InvalidArgument("crop_size must have four elements ", crop_window.shape().DebugString()));
      auto crop_window_vec = crop_window.vec<int32>();

      // crop_window_vec : [y, x, crop_h, crop_w]
      DpCropInfo cropInfo = {0};
      cropInfo.left = crop_window_vec(1);
      cropInfo.up = crop_window_vec(0);
      cropInfo.right = crop_window_vec(1) + crop_window_vec(3);
      cropInfo.down = crop_window_vec(0) + crop_window_vec(2);

      ret = (*decode_crop_resize_)(dvppInfo, cropInfo);
    } else {
      ret = (*decode_resize_)(dvppInfo);
    }

    OP_REQUIRES(context, ret == 0, errors::Internal("Decode jpeg failed, height is: ", height, "width is: ", width));
  }
  bool IsExpensive() override { return false; }

 private:
  bool crop_;
  void *handle_;
  fDecodeAndResizeJpeg decode_resize_;
  fDecodeAndCropAndResizeJpeg decode_crop_resize_;
};

REGISTER_KERNEL_BUILDER(Name("DecodeAndResizeJpeg").Device(DEVICE_CPU), DecodeImageOp);
REGISTER_KERNEL_BUILDER(Name("DecodeAndCropAndResizeJpeg").Device(DEVICE_CPU), DecodeImageOp);
}  // namespace tensorflow
