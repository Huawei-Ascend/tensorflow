include(FetchContent)
set(_json_url "")
if(TF_PKG_SERVER)
  set(_json_url "${TF_PKG_SERVER}/libs/securec/v1.1.10.tar.gz")
  FetchContent_Declare(
          secure_c
          URL ${_json_url}
          URL_HASH MD5=193f0ca5246c1dd84920db34d2d8249f
  )
else()
  FetchContent_Declare(
          secure_c
          URL https://gitee.com/openeuler/libboundscheck/repository/archive/v1.1.10.tar.gz
          URL_HASH MD5=193f0ca5246c1dd84920db34d2d8249f
  )
endif()
FetchContent_GetProperties(secure_c)
if (NOT secure_c_POPULATED)
    FetchContent_Populate(secure_c)
    include_directories(${secure_c_SOURCE_DIR}/include)
endif ()
