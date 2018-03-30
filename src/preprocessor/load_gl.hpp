#pragma once

#include <cinttypes>

namespace glshader::preprocessor::impl::load_gl
{
    constexpr uint32_t NUM_EXTENSIONS    = 0x821D;
    constexpr uint32_t EXTENSIONS        = 0x1F03;
    using glGetIntegerv_fun_t   = const uint8_t* (*)(uint32_t, int*);
    using glGetStringi_fun_t    = const uint8_t* (*)(uint32_t, int);
    extern glGetIntegerv_fun_t glGetIntegerv_fun;
    extern glGetStringi_fun_t glGetStringi_fun;

    bool load_opengl();
}