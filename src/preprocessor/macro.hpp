#pragma once

#include "files.hpp"
#include <glsp.hpp>

namespace glshader::preprocessor::impl::macro
{
    bool is_defined(const std::string& val, const processed_file& processed);
    bool is_macro(const char* text_ptr, processed_file& processed);
    std::string expand(const char* text_ptr, const char* & text_ptr_after, const files::path& current_file, int current_line, processed_file& processed);
}