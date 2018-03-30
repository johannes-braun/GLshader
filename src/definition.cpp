#include <definition.hpp>

namespace glshader
{
    definition_info::definition_info(const std::string value) : replacement(std::move(value))
    {}

    definition_info::definition_info(const std::vector<std::string> parameters, const std::string replacement) :
        replacement(std::move(replacement)), parameters(std::move(parameters))
    {}
}
