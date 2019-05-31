cmake_minimum_required(VERSION 3.8)

# create target
add_executable(from_file main.cpp wgl_context.cpp )

# set required language standard
set_target_properties(from_file PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        )

# link libraries
target_link_libraries(from_file glsp::glsp)
