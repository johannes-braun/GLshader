#pragma once

#include "glsp.hpp"

namespace glshader::process
{
    constexpr uint32_t make_tag(const char name[4])
    {
        return (name[0] << 0) | (name[1] << 8) | (name[2] << 16) | (name[3] << 24);
    }

    enum class format : uint32_t
    {
        gl_binary   = make_tag("GBIN"),
        spirv       = make_tag("SPRV")
    };

    struct shader_binary
    {
        uint32_t format;
        std::vector<uint8_t> data;
    };

    class compiler : public glsp::state
    {
    public:
        compiler(const std::string& extension, const glsp::files::path& cache_dir);
        void set_extension(const std::string& ext);
        void set_cache_dir(const glsp::files::path& dir);
        void set_default_prefix(const std::string& prefix);
        void set_default_postfix(const std::string& postfix);

        shader_binary compile(const glsp::files::path& shader, format format, bool force_reload = false, std::vector<glsp::files::path> includes ={}, std::vector<glsp::definition> definitions ={});

    private:
        std::string _default_prefix;
        std::string _default_postfix;
        std::string _extension;
        glsp::files::path _cache_dir;
    };
}