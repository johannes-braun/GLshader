cmake_minimum_required(VERSION 3.9)
project(glsp)

# -------------------------------------------------------------
# create target
# -------------------------------------------------------------

add_library(glsp    "src/definition.cpp"
                    "src/compiler/compiler.cpp"
                    "src/compress/huffman.cpp"
                    "src/opengl/loader.cpp"
                    "src/preprocessor/classify.cpp"
                    "src/preprocessor/control.cpp"
                    "src/preprocessor/eval.cpp"
                    "src/preprocessor/extensions.cpp"
                    "src/preprocessor/macro.cpp"
                    "src/preprocessor/preprocessor.cpp"
                    "src/preprocessor/skip.cpp" )

# add an alias so that library can be used inside the build tree, e.g. when testing
add_library(glsp::glsp ALIAS glsp)

# -------------------------------------------------------------
# set include dirs
# -------------------------------------------------------------

target_include_directories( glsp
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src )


# -------------------------------------------------------------
# link libraries
# -------------------------------------------------------------

if (MSVC)
    target_link_libraries(glsp PUBLIC opengl32)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif (UNIX)
    target_link_libraries(glsp PUBLIC dl stdc++fs)
endif()


# -------------------------------------------------------------
# set properties
# -------------------------------------------------------------

set_target_properties(glsp  PROPERTIES LINKER_LANGUAGE CXX
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED YES)


# -------------------------------------------------------------
# export targets for easy integration
# -------------------------------------------------------------
set_target_properties(glsp PROPERTIES EXPORT_NAME glsp)
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/glsp)

# install binaries
install(TARGETS glsp
        EXPORT glsp-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

# install header
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/glsp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# generate version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
            glspConfigVersion.cmake
            VERSION 1.0.0
            COMPATIBILITY AnyNewerVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/glspConfigVersion.cmake DESTINATION ${INSTALL_CONFIGDIR})

# install the cmake config
# this does only work as long as this project has no dependencies (find_package calls)
# see https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html
install(EXPORT glsp-targets
        FILE glspConfig.cmake
        NAMESPACE glsp::
        DESTINATION ${INSTALL_CONFIGDIR} )


# --------------------------------------------------------
# export to build tree to allow instant use
# --------------------------------------------------------

option(EXPORT_BUILD_TREE "Enable, to export the targets from the build tree to your cmake registry. Useful for development." OFF)

if(EXPORT_BUILD_TREE)
    # generate config.cmake inside buid tree
    export(EXPORT glsp-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/glspConfig.cmake NAMESPACE mpUtils::)
    # register package in users package registry
    export(PACKAGE glsp)
endif()


# -------------------------------------------------------------
# compile examples if user wants them
# -------------------------------------------------------------

option(GLSP_BUILD_EXECUTABLE OFF "Builds the example executable(s).")

if(GLSP_BUILD_EXECUTABLE)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/examples ${PROJECT_SOURCE_DIR}/examples/*)
    message("[GLSP] Enabled Executables. Adding...")
    foreach(example ${children})
        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/${example})
            if (NOT ${example} MATCHES "\\..*")
                if(EXISTS ${CMAKE_SOURCE_DIR}/examples/${example}/CMakeLists.txt)
                    message("\t\t \"${example}\"")
                    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/${example})
                endif()
            endif()
        endif()
    endforeach()
endif()