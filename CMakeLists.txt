cmake_minimum_required(VERSION 3.14)
project(TFAdapter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-O2 -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_CXX_FLAGS}")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nlohmann_json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/secure_c.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/tensorflow.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc/external)
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc/soft_dp)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/*.cc)
add_library(_tf_adapter SHARED ${SOURCES})

if (NOT EXISTS ${CMAKE_BINARY_DIR}/COMPILE_FLAGS OR NOT EXISTS
        ${CMAKE_BINARY_DIR}/LINK_FLAGS)
    execute_process(COMMAND ${CMAKE_CURRENT_LIST_DIR}/configure)
endif ()

file(STRINGS "${CMAKE_BINARY_DIR}/COMPILE_FLAGS" COMPILE_FLAGS)
file(STRINGS "${CMAKE_BINARY_DIR}/LINK_FLAGS" LINK_FLAGS)
file(STRINGS "${CMAKE_BINARY_DIR}/PYTHON_BIN_PATH" PYTHON_BIN_PATH)

foreach (COMPILE_FLAG ${COMPILE_FLAGS})
    target_compile_options(_tf_adapter PUBLIC "${COMPILE_FLAG}")
endforeach (COMPILE_FLAG)

foreach (LINK_FLAG ${LINK_FLAGS})
    target_link_options(_tf_adapter PUBLIC "${LINK_FLAG}")
endforeach (LINK_FLAG)

set_target_properties(_tf_adapter PROPERTIES PREFIX "")

file(COPY ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/python DESTINATION ${CMAKE_BINARY_DIR}/dist)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/dist/python/npu_bridge)

add_custom_command(TARGET _tf_adapter
        POST_BUILD
        COMMAND ${PYTHON_BIN_PATH} ${CMAKE_BINARY_DIR}/dist/python/setup.py bdist_wheel
        VERBATIM
        )