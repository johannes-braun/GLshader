/*******************************************************************************/
/* File     preprocess.hpp
/* Author   Johannes Braun
/* Created  30.03.2018
/*******************************************************************************/

#pragma once

#include "definition.hpp"
#include "config.hpp"

#if defined(__GNUC__) && __GNUC__ < 8
    #include <experimental/filesystem>
#else
    #include <filesystem>
#endif
#include <map>
#include <set>
#include <string>
#include <vector>

namespace glshader {
namespace process {

    #if defined(__GNUC__) && __GNUC__ < 8
    #include <experimental/filesystem>
        namespace files = std::experimental::filesystem;
    #else
        namespace files = std::filesystem;
    #endif

    /* Refers to in-shader version declaration profile, e.g. #version 450 core/compatibility */
    enum class shader_profile
    {
        core = 0,
        compatibility
    };

    /* Refers to in-shader extension declarations, e.g. #extension GL_ARB_some_extension : enable */
    enum class ext_behavior {
        enable = 0,
        require,
        warn,
        disable
    };

    /* The file data after processing. */
    struct processed_file
    {
        uint32_t version{ 0 };                              /* GLSL Shader version in integral form (e.g. 450) or 0 if no version tag is found. */
        shader_profile profile{ shader_profile::core };     /* GLSL Shader profile (core or compatibility) or core if none specified. */
        files::path file_path;                              /* The original path of the loaded file or the "name" parameter if processed with preprocess_source(...). */
        std::set<files::path> dependencies;                 /* All files included while loading the shader. */
        std::map<std::string, ext_behavior> extensions;     /* All explicitly enabled/required glsl extensions. */
        std::map<std::string, definition_info> definitions; /* All definitions which have been defined in the shader without being undefined afterwards. */
        std::string contents;                               /* The fully processed shader code string. */
        int error_count = 0;                                /* The number of syntax errors that occurred while preprocessing. */
        bool minified = false;                              /* Generate the smallest possible code footprint. */

        bool valid() const noexcept;                        /* Returns true when the file has been processed successfully, false when there were syntax errors. */
        operator bool() const noexcept;                     /* Returns true when the file has been processed successfully, false when there were syntax errors. */
    };

    struct preprocess_info_base {
      std::vector<files::path> include_directories = {}; // A list of include directories to search in when parsing includes.
      std::vector<definition> definitions = {};          // A list of predefined definitions. 
      bool expand_in_macros = false;                     // Recursively expand preprocessor statements if passed as a definition.
      bool do_minify = false;                            // Generate the shortest possible code and leave out #line directives.
    };

    struct preprocess_file_info : preprocess_info_base {
      files::path file_path; // The source file to load.
    };

    struct preprocess_source_info : preprocess_info_base {
      std::string source; // The shader source.
      std::string name;   // The name of the source which will be shown when printing errors.
    };

    [[deprecated]] processed_file preprocess_file(const files::path& file_path, const std::vector<files::path>& include_directories ={}, const std::vector<definition>& definitions ={}, bool expand_in_macros = false);
    [[deprecated]] processed_file preprocess_source(const std::string& source, const std::string& name, const std::vector<files::path>& include_directories ={}, const std::vector<definition>& definitions ={}, bool expand_in_macros = false);

    /// <summary>
    /// Load and preprocess a shader source text file.
    /// </summary>
    /// <param name="info">Look into the struct <see cref="preprocess_file_info"/> for more info.</param>
    /// <returns>A preprocessed file source. <see cref="processed_file"/></returns>
    processed_file preprocess_file(preprocess_file_info const& info);

    /// <summary>
    /// Load and preprocess a shader source text.
    /// </summary>
    /// <param name="info">Look into the struct <see cref="preprocess_source_info"/> for more info.</param>
    /// <returns>A preprocessed file source. <see cref="processed_file"/></returns>
    processed_file preprocess_source(preprocess_source_info const& info);


    /* Customizable function which is called when a syntax error was detected.
    You can redefine ERR_OUTPUT(str) in config.h. */
    inline void syntax_error_print(const files::path& file, const int line, const std::string& reason)
    {
        ERR_OUTPUT("Error in " + file.string() + ":" + std::to_string(line) + ": " + reason);
    }

    /* A preprocessor state holding include directories and definitions.
    Can be used as a global default for when processing shaders, or as a slightly more flexible way to add definitions and include directories. */
    class state
    {
    public:
        /* Add a persistent definition. */
        void add_definition(const definition& d);
        /* Remove a persistent definition by it's name (without parameters, ect.!). */
        void remove_definition(const std::string& name);

        /* Add a persistent include directory. */
        void add_include_dir(const files::path& dir);
        /* Remove a persistent include directory. */
        void remove_include_dir(const files::path& dir);

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_file function. */
        [[deprecated]] processed_file preprocess_file(const files::path& file_path, std::vector<files::path> include_directories ={}, std::vector<definition> definitions ={});

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_source function. */
        [[deprecated]] processed_file preprocess_source(const std::string& source, const std::string& name, std::vector<files::path> include_directories ={}, std::vector<definition> definitions ={});

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_file function. */
        processed_file preprocess_file(preprocess_file_info info);

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_source function. */
        processed_file preprocess_source(preprocess_source_info info);

    protected:
        std::vector<files::path> _include_directories;
        std::vector<definition> _definitions;
    };
}}