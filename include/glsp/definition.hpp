#pragma once

#include <string>
#include <vector>

namespace glshader
{
    struct definition_info
    {
        definition_info() = default;
        explicit definition_info(std::string value);
        definition_info(std::vector<std::string> parameters, std::string replacement);

        std::string replacement;
        std::vector<std::string> parameters;
    };

    struct definition
    {
        std::string name;
        definition_info info;
    };
}
