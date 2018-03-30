#include <glsp/definition.hpp>

namespace glshader
{
    definition_info::definition_info(const char* value)
        : definition_info(std::string(value))
    {

    }

    definition_info::definition_info(const std::string value) : replacement(std::move(value))
    {}

    definition_info::definition_info(const std::vector<std::string> parameters, const std::string replacement) :
        replacement(std::move(replacement)), parameters(std::move(parameters))
    {}

    definition::definition(const std::string& name)
        : name(name)
    {

    }

    definition::definition(const std::string& name, const definition_info& info)
        : name(name), info(info)
    {

    }
}
