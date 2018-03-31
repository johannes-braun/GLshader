#include "load_gl.hpp"

#include <array>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace glshader::preprocessor::impl::load_gl
{
    class function_loader
    {
    public:
        function_loader()
        {
            for (size_t i = 0; i < std::size(libs); ++i) {
#ifdef _WIN32
                hnd = LoadLibraryA(libs[i]);
#else
                hnd = dlopen(libs[i], RTLD_LAZY | RTLD_GLOBAL);
#endif
                if (hnd != nullptr)
                    break;
            }

#ifdef __APPLE__
            get_fun = nullptr;
#elif defined _WIN32
            get_fun = reinterpret_cast<decltype(get_fun)>(get_handle(hnd, "wglGetProcAddress"));
#else
            get_fun = reinterpret_cast<decltype(get_fun)>(get_handle(hnd, "glXGetProcAddressARB"));
#endif
        }

        ~function_loader() 
        {
#ifdef _WIN32
            FreeLibrary(HMODULE(hnd));
#else
            dlclose(hnd);
#endif
        }

        void* get(const char* name) const
        {
            void* addr = get_fun ? get_fun(name) : nullptr;
            return addr ? addr : get_handle(hnd, name);
        }

    private:
        void *hnd;

        void* get_handle(void* handle, const char* name) const
        {
#if defined _WIN32
            return static_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), name));
#else
            return dlsym(handle, name);
#endif
        }

        void* (*get_fun)(const char*) = nullptr;

#ifdef __APPLE__
        constexpr static std::array<const char *, 4> libs ={
            "../Frameworks/OpenGL.framework/OpenGL",
            "/Library/Frameworks/OpenGL.framework/OpenGL",
            "/System/Library/Frameworks/OpenGL.framework/OpenGL",
            "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL"
        };
#elif defined _WIN32
        constexpr static std::array<const char *, 2> libs ={ "opengl32.dll" };
#else
#if defined __CYGWIN__
        constexpr static std::array<const char *, 3> libs ={
            "libGL-1.so",
#else
        constexpr static std::array<const char *, 2> libs ={
#endif
            "libGL.so.1",
            "libGL.so"
        };
#endif
    };

    const function_loader& get_loader()
    {
        static function_loader l;
        return l;
    }

    glGetIntegerv_fun_t glGetIntegerv_fun;
    glGetStringi_fun_t glGetStringi_fun;
    bool load_opengl()
    {
        glGetIntegerv_fun   = reinterpret_cast<decltype(glGetIntegerv_fun)>(get_loader().get("glGetIntegerv"));
        glGetStringi_fun    = reinterpret_cast<decltype(glGetStringi_fun)>(get_loader().get("glGetStringi"));

        return glGetIntegerv_fun && glGetStringi_fun;
    }
}