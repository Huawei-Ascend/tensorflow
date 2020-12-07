from tensorflow.contrib.util import loader
from tensorflow.python.framework import load_library
from tensorflow.python.platform import resource_loader

from npu_bridge.helper import helper
gen_npu_image_ops = helper.get_gen_ops();

def decode_and_resize_jpeg(image, size):
    """Decode and resize JPEG-encoded image.

    Args:
        image: The JPEG-encoded image.
        size: A 1-D int32 Tensor of 2 elements: new_height, new_width.
            The new size for the images.

    Returns:
        Resized image, a 3-D uint8 tensor: [new_height, new_width, channel=3] .
    """
    return gen_npu_image_ops.decode_and_resize_jpeg(image, size)

def decode_and_crop_and_resize_jpeg(image, crop_size, size):
    """Decode, crop and resize JPEG-encoded image.

    Args:
        image: The JPEG-encoded image.
        crop_size: A 1-D int32 Tensor of 4 elements: [y_min, x_min, crop_height, crop_width].
        size: A 1-D int32 Tensor of 2 elements: new_height, new_width.
            The new size for the images.

    Returns:
        Cropped and Resized image, a 3-D uint8 tensor: [new_height, new_width, channel=3].
    """
    return gen_npu_image_ops.decode_and_crop_and_resize_jpeg(image, crop_size, size)