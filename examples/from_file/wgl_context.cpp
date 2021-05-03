#include "context.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <cassert>
#include <stdexcept>

namespace gl
{
    HDC native_dc(native_handle h) { return reinterpret_cast<HDC>(h); }
    native_handle native_dc(HDC h) { return reinterpret_cast<native_handle&>(h); }

    void context::check_handle(native_handle hnd) const
    {
        assert(hnd == native_handle(0) || IsWindow(reinterpret_cast<HWND>(hnd)));
    } 

    context::context(native_handle window, std::vector<std::pair<context_attribs, int>> context_attributes, const context * const shared)
    {
        check_handle(window);
        PIXELFORMATDESCRIPTOR desc{
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            DWORD((window==native_handle(0) ? PFD_DRAW_TO_WINDOW : 0) | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER),
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0, 0,
            0, 0, 0, 0, 0,
            24,
            8,
            8,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
        HWND win;
        if (window==native_handle(0))
        {
            WNDCLASSW wnd{ 0 };
            wnd.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) ->LRESULT {return DefWindowProcW(hWnd, msg, wParam, lParam); };
            wnd.hInstance = GetModuleHandle(nullptr);
            wnd.lpszClassName = L"temp_win_wgl";
            //Register window class
            RegisterClassW(&wnd);
            win = CreateWindowExW(0, L"temp_win_wgl", L"", 0, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
            _own_window = reinterpret_cast<native_handle&>(win);
            _is_window_owner = true;
        }
        else
        {
            _own_window = window;
            _is_window_owner = false;
        }
        _device_context = native_dc(GetDC(reinterpret_cast<HWND>(_own_window)));
        int pixel_format = ChoosePixelFormat(native_dc(_device_context), &desc);
        SetPixelFormat(native_dc(_device_context), pixel_format, &desc);

        HGLRC temp_context = wglCreateContext(native_dc(_device_context));
        wglMakeCurrent(native_dc(_device_context), temp_context);

        auto wglCreateContextAttribsARB = reinterpret_cast<HGLRC(*)(HDC, HGLRC, const int*)>(wglGetProcAddress("wglCreateContextAttribsARB"));
        swapIntervalEXT = reinterpret_cast<int(*)(int)>(wglGetProcAddress("wglSwapIntervalEXT"));

        context_attributes.emplace_back(context_attribs(0), 0);
        int* attr = reinterpret_cast<int*>(&context_attributes[0].first);
        _context = static_cast<native_handle>(reinterpret_cast<uint64_t>(wglCreateContextAttribsARB(native_dc(_device_context), reinterpret_cast<HGLRC>(shared ? shared->_context : native_handle::null), attr)));
        set_pixel_format({});
        make_current();
        wglDeleteContext(temp_context);
    }

    void context::set_pixel_format(std::vector<std::pair<pixel_format_attribs, int>> attributes)
    {
        float pixel_attribs_f[] = { 0 };
        std::vector<std::pair<pixel_format_attribs, int>> default_attributes
        {
            { GL_DRAW_TO_WINDOW_ARB, true },
            { GL_SUPPORT_OPENGL_ARB, true },
            { GL_COLOR_BITS_ARB, 32 },
            { GL_RED_BITS_ARB, 8 },
            { GL_GREEN_BITS_ARB, 8 },
            { GL_BLUE_BITS_ARB, 8 },
            { GL_ALPHA_BITS_ARB, 8 },
            { GL_DEPTH_BITS_ARB, 24 },
            { GL_STENCIL_BITS_ARB, 8 },
            { GL_DOUBLE_BUFFER_ARB, true },
            { GL_PIXEL_TYPE_ARB, GL_TYPE_RGBA_ARB },
        };
        default_attributes.insert(default_attributes.end(), attributes.begin(), attributes.end());
        default_attributes.emplace_back(pixel_format_attribs(0), 0);
        int* attr = reinterpret_cast<int*>(&default_attributes[0].first);

        auto wglChoosePixelFormatARB = reinterpret_cast<BOOL(*)(HDC, const int*, const float *, UINT, int *, UINT *)>(wglGetProcAddress("wglChoosePixelFormatARB"));
        UINT format_count = 0;
        int pixel_format;
        wglChoosePixelFormatARB(native_dc(_device_context), attr, pixel_attribs_f, 1, &pixel_format, &format_count);
        if (format_count == 0) throw std::runtime_error("Could not find suitable pixel formats.");
        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(native_dc(_device_context), pixel_format, sizeof(pfd), &pfd);
        SetPixelFormat(native_dc(_device_context), pixel_format, &pfd);
    }

    void context::set_swap_interval(int i) const noexcept
    {
        swapIntervalEXT(i);
    }

    context::~context()
    {
        if (_context != native_handle::null)
            wglDeleteContext(reinterpret_cast<HGLRC>(_context));

        if(_is_window_owner)
            DestroyWindow(reinterpret_cast<HWND>(_own_window));
    }

    void context::make_current() const noexcept
    {
        wglMakeCurrent(native_dc(_device_context), reinterpret_cast<HGLRC>(_context));
    }

    void context::swap_buffers() const noexcept
    {
        SwapBuffers(native_dc(_device_context));
    }
}

#endif //defined(WIN32)