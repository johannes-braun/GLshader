# GLShader Processor

This is my attempt to create a preprocessor for GLSL code in C++, as well as a platform-dependant GLSL compiler with [glGetProgramBinary](https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glGetProgramBinary.xhtml) based on [GL_ARB_separate_shader_objects](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_separate_shader_objects.txt).

## How to use?
### Build libraries
You can use cmake to build the binaries to later link against.
Please use a recent compiler which supports c++17 and experimental::filesystem.

### Load file
Include `<glsp/glsp.hpp>` and that's it.
To load and process a file, you'd call `glsp::processed_file proc = glsp::preprocess_file("path/to/file.glsl", {}, {});`.

### Include Paths
The second parameter of the `preprocess_file` function determines the include search paths for when processing a file.
Example: `auto file = glsp::preprocess_file("path/to/file.glsl", {"my/shaders/"}, {});`.

### Predefined definitions
In the third function parameter, you can add predefined definitions.
#### Examples:

```c++
// Simple valueless definition
glsp::definition def_no_val("NO_VAL");

// Parameterless macro
glsp::definition def_my_val("MY_VAL", 2);

// Parameterized macro with parameters a, b and c.
glsp::definition def_add_mul("AddMul", { {"a", "b", "c"}, "(a + b * c)" });

// Two kinds of formatted definitions.
auto fmt_def1 = glsp::definition::from_format("MY_MACRO(a, b, fun) (fun(a, b)-1)");
auto fmt_def2 = "OTHER_MACRO(x) (x * 2.f - 1.f)"_gdef;

// Pass definitions to preprocessor.
auto file = glsp::preprocess_file("file", {}, { def_my_val, def_no_val, def_add_mul, fmt_def1, fmt_def2 });
```
A formatted definition must have one of the following formats:
* Valueless definition: `MACRO`
* Parameterless macro with value: `MACRO val`
* Parameterized with n parameters and empty or non-empty replacement: `MACRO(p0, p1, ..., pn) rep`

### State
You can use `glsp::state` as follows to allow for persistent predefined definitions and include directories.
```c++
// Once
glsp::state preproc_state;
preproc_state.add_definition("my_def(a, b, c) (-a + b * c)"_gdef);
preproc_state.add_definition({ "MY_VAL", 2 });
preproc_state.add_definition({ "NO_VAL" });
preproc_state.add_definition({ "AddMul",{ { "a", "b", "c" }, "a + b * c" } });
preproc_state.add_include_dir("../shaders/");
// ...
// Somewhere else
auto file = preproc_state.preprocess_file("my_file.glsl");
```

### Binary Compiler
The `glsp::compiler` class derives from `glsp::state` and provides the functionality to compile and cache your GLSL text files to the system's proprietary binary format and load them from there.
The resulting compiled program binary can only be used for separable programs with [opengl pipeline objects](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCreateProgramPipelines.xhtml).
Shader files compiled with the `glsp::compiler` need to have proper extensions declaring their shader stage:

| Extension | Type                      |
| :-------- | :-------------------------|
| .vert     | GL_VERTEX_SHADER          |
| .geom     | GL_GEOMETRY_SHADER        |
| .tesc     | GL_TESS_CONTROL_SHADER    |
| .tese     | GL_TESS_EVALUATION_SHADER |
| .frag     | GL_FRAGMENT_SHADER        |
| .comp     | GL_COMPUTE_SHADER         |

#### Compiler usage example:
```c++
glsp::compiler compiler(".bin", "path/to/cache/");
compiler.set_default_prefix(opengl_prefix); // optional prefix string, e.g. containing the #version tag or default extensions.
compiler.set_default_postfix(opengl_postfix); // optional postfix, e.g. bindless uniform layout declarations.

// Definitions and includes like with glsp::state
compiler.add_definition(...);
compiler.add_include_dir("my/include/dir/")

glsp::shader_binary binary = compiler.compile("path/to/shader.vert", glsp::format::gl_binary, false, more_includes, more_defines);

// Data is empty if there was a syntax-, compiler- or linker-error
if(!binary.data.empty())
{
    my_program_id = glCreateProgram();
    glProgramParameteri(_id, GL_PROGRAM_SEPARABLE, GL_TRUE); // Only a single program 
    glProgramBinary(_id, GLenum(bin.format), bin.data.data(), int(bin.data.size()));
}
else
{
    // Error handling...
}
```