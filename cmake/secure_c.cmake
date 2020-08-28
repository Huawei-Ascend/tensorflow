include(FetchContent)

FetchContent_Declare(
        secure_c
        URL https://gitee.com/openeuler/bounds_checking_function/repository/archive/v1.1.10.tar.gz
        URL_HASH MD5=0782dd2351fde6920d31a599b23d8c91
)
FetchContent_GetProperties(secure_c)
if (NOT secure_c_POPULATED)
    FetchContent_Populate(secure_c)
    include_directories(${secure_c_SOURCE_DIR}/include)
endif ()