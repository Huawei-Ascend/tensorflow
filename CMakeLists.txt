cmake_minimum_required(VERSION 3.14)
project(TFAdapter)

    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_C_FLAGS "-O2 -DNDEBUG -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -DNDEBUG -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_CXX_FLAGS}")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    # build external prjects
    if(DEFINED ENV{D_PKG_SERVER})
        set(TF_PKG_SERVER $ENV{D_PKG_SERVER})
        message("Download packages from PKG server.")
    endif()
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/nlohmann_json.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/secure_c.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/tensorflow.cmake)
    include_directories(${CMAKE_CURRENT_LIST_DIR})
    include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/inc/external)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/inc/soft_dp)

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/tools/COMPILE_FLAGS OR NOT EXISTS
        ${CMAKE_CURRENT_LIST_DIR}/tools/LINK_FLAGS OR NOT EXISTS
        ${CMAKE_CURRENT_LIST_DIR}/tools/PYTHON_BIN_PATH OR NOT EXISTS
        ${CMAKE_CURRENT_LIST_DIR}/tools/SWIG_BIN_PATH)
    message(FATAL_ERROR "No validate configuration found. Did you forget to configure first?")
endif ()

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/tools/COMPILE_FLAGS" COMPILE_FLAGS)
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/tools/LINK_FLAGS" LINK_FLAGS)
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/tools/PYTHON_BIN_PATH" PYTHON_BIN_PATH)
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/tools/SWIG_BIN_PATH" SWIG_BIN_PATH)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/python DESTINATION ${CMAKE_BINARY_DIR}/dist)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/swig DESTINATION ${CMAKE_BINARY_DIR}/dist)
execute_process(COMMAND ${SWIG_BIN_PATH} -python -c++ -threads ${CMAKE_BINARY_DIR}/dist/swig/ge_plugin.i)
file(COPY ${CMAKE_BINARY_DIR}/dist/swig/tf_adapter.py DESTINATION ${CMAKE_BINARY_DIR}/dist/python/npu_bridge)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/*.cc)
add_library(_tf_adapter SHARED ${SOURCES} ${CMAKE_BINARY_DIR}/dist/swig/ge_plugin_wrap.cxx)

foreach (COMPILE_FLAG ${COMPILE_FLAGS})
    target_compile_options(_tf_adapter PUBLIC "${COMPILE_FLAG}")
endforeach (COMPILE_FLAG)

target_link_libraries(_tf_adapter PUBLIC "dl")
foreach (LINK_FLAG ${LINK_FLAGS})
    target_link_libraries(_tf_adapter PUBLIC "${LINK_FLAG}" -Wl,--no-as-needed)
endforeach (LINK_FLAG)

set_target_properties(_tf_adapter PROPERTIES PREFIX "")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/dist/python/npu_bridge)

add_custom_command(TARGET _tf_adapter
        POST_BUILD
        COMMAND cd ${CMAKE_BINARY_DIR}/dist/python/ && ${PYTHON_BIN_PATH} setup.py bdist_wheel
        VERBATIM
        )
