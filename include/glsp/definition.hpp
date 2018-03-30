#pragma once

#include <string>
#include <vector>

namespace glshader
{
    struct definition_info
    {
        definition_info() = default;
        definition_info(std::string value);
        definition_info(const char* value);
        template<typename T, typename = decltype(std::to_string(std::declval<T>()))>
        definition_info(const T& value) : definition_info(std::to_string(value)) {}
        definition_info(std::vector<std::string> parameters, std::string replacement);

        std::string replacement;
        std::vector<std::string> parameters;
    };

    struct definition
    {
        definition(const std::string& name);
        definition(const std::string& name, const definition_info& info);

        std::string name;
        definition_info info;
    };
}