#include "control.hpp"
#include "../opengl/loader.hpp"
#include <string>

namespace glshader::process::impl::control
{
    constexpr uint32_t GL_VENDOR = 0x1F00;

    std::string line_directive(const files::path& file, int line)
    {

        static struct GetStringFunction
        {
        public:
            GetStringFunction() : glGetStringFunc(nullptr)
            {
                namespace lgl = impl::loader;
                if (lgl::valid())
                    glGetStringFunc = reinterpret_cast<decltype(glGetStringFunc)>(lgl::load_function("glGetString"));
            }
            const char *operator()(uint32_t param) {return glGetStringFunc(param);}
        private:
            const char * (*glGetStringFunc)(uint32_t );

        } glGetString;

        std::string s = glGetString(GL_VENDOR);
        if(s.find("NVIDIA") != std::string::npos)
            return "\n#line " + std::to_string(line) + " \"" + file.filename().string() + "\"\n";
        else
            return "\n#line " + std::to_string(line) + "\n";
    }

    void increment_line(int& current_line, processed_file& processed)
    {
        processed.definitions["__LINE__"] = ++current_line;
    }
}