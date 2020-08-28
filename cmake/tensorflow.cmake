include(FetchContent)

FetchContent_Declare(
        tensorflow
        URL https://github.com/tensorflow/tensorflow/archive/v1.15.0.zip
        URL_HASH MD5=0ad811d8d59f56ecc1a6032af997ec1d
)
FetchContent_GetProperties(tensorflow)
if (NOT tensorflow_POPULATED)
    FetchContent_Populate(tensorflow)
    include_directories(${tensorflow_SOURCE_DIR})
endif ()
