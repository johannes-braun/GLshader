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

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/glsp DESTINATION include FILES_MATCHING PATTERN *.hpp)
install (TARGETS glsp EXPORT glspTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

install(EXPORT glspTargets
    FILE glspTargets.cmake
    NAMESPACE jbraun::
    DESTINATION lib/cmake/glsp
)


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