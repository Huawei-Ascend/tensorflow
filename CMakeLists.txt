option(ENABLE_OPEN_SRC "Enable graphengine compile in opensource." FALSE)
if (ENABLE_OPEN_SRC)
    cmake_minimum_required(VERSION 3.14)
    project(TFAdapter)

    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_C_FLAGS "-O2 -DNDEBUG -Wno-deprecated-declarations -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -DNDEBUG -Wno-deprecated-declarations -Wall -fPIC -fstack-protector-all -Wl,-z,relro,-z,now,-z,noexecstack -pipe ${CMAKE_CXX_FLAGS}")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    # build external prjects
    if(DEFINED ENV{D_PKG_SERVER})
        set(TF_PKG_SERVER $ENV{D_PKG_SERVER})
        message("Download packages from PKG server")
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
        target_link_libraries(_tf_adapter PUBLIC "${LINK_FLAG}")
    endforeach (LINK_FLAG)

    set_target_properties(_tf_adapter PROPERTIES PREFIX "")

    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/dist/python/npu_bridge)

    add_custom_command(TARGET _tf_adapter
            POST_BUILD
            COMMAND cd ${CMAKE_BINARY_DIR}/dist/python/ && ${PYTHON_BIN_PATH} setup.py bdist_wheel
            VERBATIM
            )
else()
    get_filename_component(TOP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(CMAKE_VERBOSE_MAKEFILE ON)

    # rewrite __FILE__ to avoid absolute path
    set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> -D__FILE__='\"$(subst ${CMAKE_CURRENT_SOURCE_DIR}/,,$(abspath $<))\"' -Wno-builtin-macro-redefined  <INCLUDES> <FLAGS> -o <OBJECT>  -c <SOURCE>")
    set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <DEFINES>  -D__FILE__='\"$(subst ${CMAKE_CURRENT_SOURCE_DIR}/,,$(abspath $<))\"' -Wno-builtin-macro-redefined  <INCLUDES> <FLAGS> -o <OBJECT>  -c <SOURCE>")

    execute_process(COMMAND ${HI_PYTHON} -c "import tensorflow as tf; print(tf.sysconfig.get_lib(),end='')"
                    OUTPUT_VARIABLE TF_INSTALL_LIB_PATH)

    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/tf_adapter/*.cc)
    add_library(tf_adapter SHARED
        ${SOURCES}
        ${BASE_DIR}/tf_adapter/util/ge_plugin_wrap.cxx
    )

    target_include_directories(tf_adapter PRIVATE
        ${BASE_DIR}/
        ${TOP_DIR}/inc/
        ${TOP_DIR}/inc/external/
        ${TOP_DIR}/inc/common/
        ${TOP_DIR}/inc/soft_dp/
        ${TOP_DIR}/soft_dp/
        ${TOP_DIR}/graphengine/inc/
        ${TOP_DIR}/graphengine/inc/external/
        ${TOP_DIR}/metadef/inc/
        ${TOP_DIR}/metadef/inc/external/
        ${TOP_DIR}/libc_sec/include/
        ${TOP_DIR}/third_party/json/include/
        ${TOP_DIR}/third_party/tensorflow/tensorflow-1.15.0/
        ${TF_INSTALL_LIB_PATH}/include/
        # ${HI_PYTHON_INC}/
        /opt/buildtools/python3.7/include/python3.7m/
    )

    target_compile_options(tf_adapter PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
        -O2
        -DNDEBUG
        -ftrapv
        -Wno-deprecated-declarations
    )
    target_compile_definitions(tf_adapter PUBLIC
        _FORTIFY_SOURCE=2
    )

    target_link_libraries(tf_adapter PUBLIC
        $<BUILD_INTERFACE:intf_pub>
        -Wl,--no-as-needed
        c_sec
        ge_runner
        datatransfer
        fmk_parser
        indextransform
        ${TF_INSTALL_LIB_PATH}/libtensorflow_framework.so.1
        ${TF_INSTALL_LIB_PATH}/python/_pywrap_tensorflow_internal.so
        -Wl,--as-needed
    )

    # rename libtf_adapter.so to _tf_adapter.so
    set_target_properties(tf_adapter
        PROPERTIES
        PREFIX _
    )
    ###################################################################################################
    add_custom_command(
        OUTPUT ${BASE_DIR}/tf_adapter/util/ge_plugin_wrap.cxx
        COMMAND echo "before generate swig files"
        && swig -c++ -threads -python ${BASE_DIR}/tf_adapter/swig/ge_plugin.i
        && mv ${BASE_DIR}/tf_adapter/swig/ge_plugin_wrap.cxx ${BASE_DIR}/tf_adapter/util/
        && mv ${BASE_DIR}/tf_adapter/swig/tf_adapter.py ${BASE_DIR}/tf_adapter/python/npu_bridge/
        && echo "end generate swig files"
    )

    ###################################################################################################
    add_custom_target(
        hw_npu_bridge_1.15.0 ALL
        DEPENDS tf_adapter SoftDp ${CMAKE_CURRENT_BINARY_DIR}/npu_bridge-1.15.0-py3-none-any.whl
    )

    # package _tf_adapter.so and libSoftDp.so in npu_bridge-xxx.whl
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/npu_bridge-1.15.0-py3-none-any.whl
        COMMAND echo "package whl start"
        && mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/wheel
        && cp -r ${BASE_DIR}/tf_adapter/python/. ${CMAKE_CURRENT_BINARY_DIR}/wheel
        && cp -r ${CMAKE_CURRENT_BINARY_DIR}/_tf_adapter.so ${CMAKE_CURRENT_BINARY_DIR}/wheel/npu_bridge
        && cp -r $<TARGET_FILE:SoftDp> ${CMAKE_CURRENT_BINARY_DIR}/wheel/npu_bridge
        # && cp -r ${CMAKE_CURRENT_BINARY_DIR}/../../../../../soft_dp/libSoftDp.so ${CMAKE_CURRENT_BINARY_DIR}/wheel/npu_bridge
    	&& cd ${CMAKE_CURRENT_BINARY_DIR}/wheel
        && ${HI_PYTHON} setup.py bdist_wheel >/dev/null
        && cp -f dist/npu_bridge-1.15.0-py3-none-any.whl ${CMAKE_CURRENT_BINARY_DIR}/
        && echo "package whl end"
    )

    ###################################################################################################
    set(INSTALL_BASE_DIR "")
    set(INSTALL_LIBRARY_DIR lib)

    include(CMakePackageConfigHelpers)

    install(TARGETS tf_adapter OPTIONAL
        EXPORT tfplugin-targets
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/npu_bridge-1.15.0-py3-none-any.whl OPTIONAL
        DESTINATION ${INSTALL_LIBRARY_DIR}
    )
endif()
