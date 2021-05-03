#include <iostream>
#include "context.hpp"
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
    glsp::preprocess_file_info info;
    info.file_path = "C:\\Users\\johan\\Documents\\Projekte\\myrt\\ext\\glsp\\examples\\from_file\\test.vert";
    info.include_directories = { "C:\\Users\\johan\\Documents\\Projekte\\myrt\\ext\\glsp\\examples\\from_file" };
     const glsp::processed_file file = process_state.preprocess_file(info);
    if(!file)
    {
        std::cout << "Failed to process file.\n";
    }

    constexpr const char* source = R"(
#include "inc.glsl"

#define MY_FUN sin
#define OTHER_FUN() cos

void main()
{
	int x = int(MY_FUN(177.f));
	int y = int(OTHER_FUN( )(12.f));
}
)";

    const glsp::processed_file src = process_state.preprocess_source(source, "Shader");

    // Check definition with pp-macros in it...

    glsp::definition def{ "PP_TEST", R"(
#include "inc.glsl"

#define IN_DEF_MACRO(x) sin((x))

float calc_sin(float x) {
  return IN_DEF_MACRO(x);
}

#undef IN_DEF_MACRO

float calc_sin_notworking(float x) {
  return IN_DEF_MACRO(x);
}
)" };

    constexpr const char* sourcet = R"(
PP_TEST

void main()
{
}
)";

    const glsp::processed_file srct = process_state.preprocess_source(sourcet, "Shader", {"C:\\Users\\johan\\Documents\\Projekte\\myrt\\ext\\glsp\\examples\\from_file"}, { def });



    std::cin.ignore();
    return 0;
}