include(FetchContent)
set(_json_url "")
if(TF_PKG_SERVER)
  set(_json_url "${TF_PKG_SERVER}/libs/json/v3.6.1/include.zip")
  FetchContent_Declare(
          nlohmann_json
          URL ${_json_url}
          URL_HASH MD5=0dc903888211db3a0f170304cd9f3a89
  )
else()
  FetchContent_Declare(
          nlohmann_json
          URL https://github.com/nlohmann/json/releases/download/v3.6.1/include.zip
          URL_HASH MD5=0dc903888211db3a0f170304cd9f3a89
  )
endif()
FetchContent_GetProperties(nlohmann_json)
if (NOT nlohmann_json_POPULATED)
    FetchContent_Populate(nlohmann_json)
    include_directories(${nlohmann_json_SOURCE_DIR})
endif ()