cmake_minimum_required(VERSION 3.9)
project(GLShader)

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

add_library(jbraun::glsp ALIAS glsp)

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
            file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/examples/${example}/*.cpp)
            add_executable(${example} ${SOURCES})
            target_link_libraries(${example} glsp)
            if(MSVC)
                target_link_libraries(${example} opengl32)
            endif()
            set_target_properties(${example} PROPERTIES LINKER_LANGUAGE CXX RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/${example}" CXX_STANDARD 17)
            target_include_directories(${example} PUBLIC ${INCLUDES})
            target_include_directories(${example} PUBLIC ${PROJECT_SOURCE_DIR}/examples/${example})
        endif()
    endforeach()
endif()