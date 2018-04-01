#pragma once

#include "definition.hpp"
#include "config.hpp"

#include <experimental/filesystem>
#include <map>
#include <set>

#ifndef ERR_OUTPUT
#include <iostream>
#define ERR_OUTPUT(x) std::cerr << (x) << '\n'
#endif

namespace glshader::process
{
    namespace files = std::experimental::filesystem;

    enum class shader_profile { core = 0, compat };
    enum class ext_state { enable = 0, require };

    struct processed_file
    {
        /* GLSL Shader version in integral form (e.g. 450) or 0 if no version tag is found. */
        uint32_t version{ 0 };
        /* GLSL Shader profile (core or compatibility) or core if none specified. */
        shader_profile profile{ shader_profile::core };
        /* The original path of the loaded file. */
        files::path file_path;
        /* All files included while loading the shader. */
        std::set<files::path> dependencies;
        /* All explicitly enabled/required glsl extensions. */
        std::map<std::string, ext_state> extensions;
        /* All definitions which have been defined in the shader without being undefined afterwards. */
        std::map<std::string, definition_info> definitions;
        /* The fully processed shader code string. */
        std::string contents;
    };

    /* Loads and processes a shader file. */
    /* If being called while having a valid OpenGL context, all available extension names will be loaded and checked against when compiling. */
    /* Otherwise, extension related #if statements will always be evaluated as false. */
    /* file_path -- The source file to load. */
    /* include_directories -- A list of include directories to search in when parsing includes. */
    /* definitions -- A list of predefined definitions. */
    processed_file preprocess_file(const files::path& file_path, const std::vector<files::path>& include_directories, const std::vector<definition>& definitions);

    /* Customizable function which is called when a syntax error was detected. */
    /* You can redefine ERR_OUTPUT(str) in config.h. */
    inline void syntax_error(const files::path& file, const int line, const std::string& reason)
    {
        ERR_OUTPUT("Error in " + file.string() + ":" + std::to_string(line) + ": " + reason);
    }

    class compiler;
    /* A preprocessor state holding include directories and definitions. */
    /* Can be used as a global default for when processing shaders, or as a slightly more flexible way to add definitions and include directories. */
    class state
    {
    public:
        friend compiler;
        /* Add a persistent definition. */
        void add_definition(const definition& d);
        /* Remove a persistent definition by it's name (without parameters, ect.!). */
        void remove_definition(const std::string& name);

        /* Add a persistent include directory. */
        void add_include_dir(const files::path& dir);
        /* Remove a persistent include directory. */
        void remove_include_dir(const files::path& dir);

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy), */
        /* and calls the global glsp::preprocess_file function. */
        processed_file preprocess_file(const files::path& file_path, std::vector<files::path> include_directories ={}, std::vector<definition> definitions ={});

    private:
        std::vector<files::path> _include_directories;
        std::vector<definition> _definitions;
    };
}
#undef ERR_OUTPUT