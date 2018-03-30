# GLShader Preprocessor

This is my attempt to create a preprocessor for GLSL code in C++.

## How to use?
To load and process a file, you'd call `glsp::preprocess_file("path/to/file.glsl", {}, {});`. The second parameter can be a vector of include paths, the last parameter contains definitions.