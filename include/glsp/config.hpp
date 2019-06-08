/*******************************************************************************/
/* File     config.hpp
/* Author   Johannes Braun
/* Created  31.03.2018
/*
/* User space for own settings.
/*******************************************************************************/

#pragma once

// NAMESPACE:
namespace glshader::process {}
// Shorten base namespace. You can use your own namespace if you wish.
namespace glsp = glshader::process;

#include <functional>
#include <string>
#include <iostream>

// LOGGING
// to use a custom logger just set glsp::ERR_OUTPUT to another function
namespace glshader::process {
    extern std::function<void(const std::string &)> ERR_OUTPUT;
}