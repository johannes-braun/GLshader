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
// Pass definitions to preprocessor.
auto file = glsp::preprocess_file("file", {}, { def_my_val, def_no_val, def_add_mul });
```