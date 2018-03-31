# GLShader Preprocessor

This is my attempt to create a preprocessor for GLSL code in C++.

## How to use?
### Load file
Include `<glsp/glsp.hpp>` and that's it.
To load and process a file, you'd call `glsp::processed_file proc = glsp::preprocess_file("path/to/file.glsl", {}, {});`.

### Include Paths
The second parameter of the `preprocess_file` function determines the include search paths for when processing a file.
Example: `auto file = glsp::preprocess_file("path/to/file.glsl", {"my/shaders/"}, {});`.

### Predefined definitions
In the third function parameter, you can add predefined definitions.
Examples:

```C++
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