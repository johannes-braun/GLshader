#include <iostream>
#include <context.hpp>
#include <glsp/preprocess.hpp>

int main()
{
    gl::context context(gl::native_handle::null, {
        { GL_CONTEXT_MAJOR_VERSION_ARB, 4 },
        { GL_CONTEXT_MINOR_VERSION_ARB, 5 },
        { GL_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CORE_PROFILE_BIT_ARB },
        { GL_CONTEXT_FLAGS_ARB, GL_CONTEXT_DEBUG_BIT_ARB }
    });

    glsp::state process_state;
    const glsp::processed_file file = process_state.preprocess_file("test.vert");
    if(!file)
    {
        std::cout << "Failed to process file.\n";
    }

    constexpr const char* source = R"(
#include "inc.glsl"
    
void main()
{}
)";

    const glsp::processed_file src = process_state.preprocess_source(source, "Shader");

    std::cin.ignore();
    return 0;
}