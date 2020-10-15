include(FetchContent)

FetchContent_Declare(
        secure_c
        URL https://gitee.com/openeuler/libboundscheck/repository/archive/v1.1.10.tar.gz
        URL_HASH MD5=193f0ca5246c1dd84920db34d2d8249f
)
FetchContent_GetProperties(secure_c)
if (NOT secure_c_POPULATED)
    FetchContent_Populate(secure_c)
    include_directories(${secure_c_SOURCE_DIR}/include)
endif ()
