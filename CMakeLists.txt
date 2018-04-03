cmake_minimum_required(VERSION 3.9)
project(GLShader)

list(APPEND INCLUDES ${PROJECT_SOURCE_DIR}/include)

if (MSVC)
    list(APPEND LIBRARIES opengl32)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif (UNIX)
    list(APPEND LIBRARIES dl stdc++fs)
endif()

file(GLOB_RECURSE SRC_CPP "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(glsp ${SRC_CPP})
target_link_libraries(glsp ${LIBRARIES})
set_target_properties(glsp PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(glsp PUBLIC ${INCLUDES})
set_property(TARGET glsp PROPERTY CXX_STANDARD 17)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/glsp DESTINATION include FILES_MATCHING PATTERN *.hpp)
install (TARGETS glsp ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)