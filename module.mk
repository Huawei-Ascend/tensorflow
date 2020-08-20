LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# static check function. Just for static check.
TF_ADPTER_OUTPUT_BASE := $(PWD)/out/$(product)/host/obj/tf_adapter
TF_ADPTER_GEN_PATH := $(TF_ADPTER_OUTPUT_BASE)/genfiles
TF_ADPTER_EXEC_PATH := $(TF_ADPTER_GEN_PATH)/execroot/tf_adapter
TF_ADPTER_REL_BUILD_PATH := $(TF_ADPTER_OUTPUT_BASE)/genfiles/execroot/tf_adapter
TF_ADPTER_WORKSPACE_BASE := third_party/tensorflow/huawei_patch/tf-1.15.0/tf_plugin
TF_ADPTER_REL_WORKSPACE_BASE := $(PWD)/$(TF_ADPTER_WORKSPACE_BASE)
TF_ADPTER_LOG_PATH := $(TF_ADPTER_OUTPUT_BASE)/tf_adapter_log
define do_libtf_adapter_coverity_check
	@mkdir -p ${TF_ADPTER_LOG_PATH}
	@mkdir -p $(PWD)/out/$(product)/host/obj/lib
	@bash ${TF_ADPTER_REL_WORKSPACE_BASE}/tools/build_wheel.sh $(PWD)/$@ $(product) \
	2>&1 | tee $(TF_ADPTER_LOG_PATH)/obuild_libtf_adapter.log
	@python ${TF_ADPTER_REL_WORKSPACE_BASE}/tools/gen_sc_makefile_bazel.py \
	$(TF_ADPTER_LOG_PATH)/obuild_libtf_adapter.log \
	${TF_ADPTER_REL_WORKSPACE_BASE}/tools/sc_list.txt \
	$(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk \
	${TF_ADPTER_REL_WORKSPACE_BASE}/
	#static check tool does not recognize -isystem and -iquote
	@sed -i "s/iquote/I/g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s/isystem/I/g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	#change relative path to full path
	@sed -i "s#"\ bazel-out"#"\ $(TF_ADPTER_EXEC_PATH)/bazel-out"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"\=bazel-out"#"\=$(TF_ADPTER_EXEC_PATH)/bazel-out"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"\ external"#"\ $(TF_ADPTER_GEN_PATH)/external"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"-Iexternal"#"-I$(TF_ADPTER_GEN_PATH)/external"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"-I\ .\ "#"-I\ $(TF_ADPTER_REL_WORKSPACE_BASE)\ "#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"\ tf_adapter"#"\ ${TF_ADPTER_REL_WORKSPACE_BASE}/tf_adapter"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	@sed -i "s#"\ $(TF_ADPTER_WORKSPACE_BASE)"#"\ $(TF_ADPTER_REL_WORKSPACE_BASE)"#g" $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
	#change authority to gcc execute
	@chmod 777 -R $(TF_ADPTER_REL_BUILD_PATH)/bazel-out
	@make -f $(TF_ADPTER_EXEC_PATH)/Makefile_sc.mk
endef
# static check function end.

LOCAL_SHARED_LIBRARIES := \
                          libc_sec \
                          libge_runner \
                          libtsdclient \
                          libdatatransfer \
                          libfmk_parser \

LOCAL_SOFT_DP_LIBRARIES := libSoftDp

npu_bridge_shared_libraries := \
$(addprefix $(HOST_OUT_INTERMEDIATE_LIBRARIES)/, \
$(addsuffix $(TARGET_SHLIB_SUFFIX), \
$(LOCAL_SHARED_LIBRARIES) \
$(LOCAL_SOFT_DP_LIBRARIES))) \

.PHONY: $(HOST_OUT_ROOT)/npu_bridge-1.15.0-py3-none-any.whl
$(HOST_OUT_ROOT)/npu_bridge-1.15.0-py3-none-any.whl:$(npu_bridge_shared_libraries)
	@mkdir -p $(dir $@)
ifeq ($(COVERITY), true)
	$(call do_libtf_adapter_coverity_check)
else ifeq ($(OBB_PRINT_CMD), true)
	$(call do_libtf_adapter_coverity_check)
else
	@swig -c++ -python -threads $(TF_ADPTER_REL_WORKSPACE_BASE)/tf_adapter/swig/ge_plugin.i
	@mv $(TF_ADPTER_REL_WORKSPACE_BASE)/tf_adapter/swig/ge_plugin_wrap.cxx $(TF_ADPTER_REL_WORKSPACE_BASE)/tf_adapter/util/
	@mv $(TF_ADPTER_REL_WORKSPACE_BASE)/tf_adapter/swig/tf_adapter.py $(TF_ADPTER_REL_WORKSPACE_BASE)/tf_adapter/python/npu_bridge/
	@bash $(PWD)/third_party/tensorflow/huawei_patch/tf-1.15.0/tf_plugin/tools/build_wheel.sh $(PWD)/$@ $(product)
endif

LOCAL_MODULE := hw_npu_bridge_1.15.0
.PHONY: $(LOCAL_MODULE)
$(LOCAL_MODULE):$(HOST_OUT_ROOT)/npu_bridge-1.15.0-py3-none-any.whl

include $(BUILD_HOST_SHARED_LIBRARY)
