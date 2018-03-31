#pragma once


// CUSTOM LOGGING:
// If you want to use your own logger for error logs, you can define ERR_OUTPUT(x) with x being the logged string.
// Example:
// #define ERR_OUTPUT(x) tlog_e("GLShader") << (x)


// NAMESPACE:
// Shorten base namespace. You can use your own namespace if you wish.
namespace glsp = glshader::preprocessor;