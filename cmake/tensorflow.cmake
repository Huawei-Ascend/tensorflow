include(FetchContent)
set(_json_url "")
if(TF_PKG_SERVER)
  set(_json_url "${TF_PKG_SERVER}/libs/tensorflow/v1.15.0.zip")
  FetchContent_Declare(
          tensorflow
          URL ${_json_url}
          URL_HASH MD5=0ad811d8d59f56ecc1a6032af997ec1d
  )
else()
  FetchContent_Declare(
          tensorflow
          URL https://github.com/tensorflow/tensorflow/archive/v1.15.0.zip
          URL_HASH MD5=0ad811d8d59f56ecc1a6032af997ec1d
  )
endif()
FetchContent_GetProperties(tensorflow)
if (NOT tensorflow_POPULATED)
    FetchContent_Populate(tensorflow)
    include_directories(${tensorflow_SOURCE_DIR})
endif ()
