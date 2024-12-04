//
// Created by NeoLiu on 2024/10/27.
//
#include "pch.h"
#include "platform_windows.h"
#include "core/input.h"
#include "renderer/vulkan/vulkan_platform.h"

//for surface creation
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include "renderer/vulkan/vulkan_types.h"

namespace NeoEngine {
    LRESULT CALLBACK WindowsProcessMessage(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

    PlatformWindows::PlatformWindows() : platform_state_(new PlatformState()), clock_frequency_(), start_time_() {
    }

    PlatformWindows::~PlatformWindows() {
        PlatformWindows::Shutdown();
    }

    bool PlatformWindows::Startup(
        const std::string& application_name,
        int32_t x,
        int32_t y,
        int32_t width,
        int32_t height) {

        platform_state_->hInstance = GetModuleHandleA(0);

        //setup && register window class
        HICON hIcon = LoadIcon(platform_state_->hInstance, IDI_APPLICATION);
        WNDCLASSA window_class = {};

        window_class.style = CS_DBLCLKS;
        window_class.lpfnWndProc = WindowsProcessMessage;
        window_class.hInstance = platform_state_->hInstance;
        window_class.hIcon = hIcon;
        window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        window_class.cbClsExtra = 0;
        window_class.cbWndExtra = 0;
        window_class.hbrBackground = nullptr;
        window_class.lpszClassName = "neo_window_class";

        if(!RegisterClassA(&window_class)) {
            MessageBoxA(nullptr, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        //create window
        uint32_t client_x = x;
        uint32_t client_y = y;
        uint32_t client_width = width;
        uint32_t client_height = height;

        uint32_t window_x = client_x;
        uint32_t window_y = client_y;
        uint32_t window_width = client_width;
        uint32_t window_height = client_height;

        uint32_t window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
        uint32_t window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_THICKFRAME;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_MAXIMIZEBOX;

        RECT window_rect = { 0, 0, 0, 0 };
        AdjustWindowRectEx(&window_rect, window_style, 0, window_ex_style);

        window_x += window_rect.left;
        window_y += window_rect.top;

        window_width += window_rect.right - window_rect.left;
        window_height += window_rect.bottom - window_rect.top;

        HWND handle = CreateWindowExA(
            window_ex_style, "neo_window_class",
            application_name.c_str(), window_style,
            window_x, window_y, window_width, window_height,0, 0,
            platform_state_->hInstance, 0
            );

        if(handle == nullptr) {
            MessageBoxA(nullptr, "Window creation failed", "Error", MB_ICONEXCLAMATION | MB_OK);

            NEO_FATAL("Window creation failed, Error Code: %d", ::GetLastError());
            return false;
        } else {
            platform_state_->hWnd = handle;
        }

        //show the window
        bool should_activate = true; //if the window shouldn't accept input , this should be false
        int32_t show_window_command_flag = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        ShowWindow(platform_state_->hWnd, show_window_command_flag);

        //clock setup
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clock_frequency_ = 1.0f / (double)frequency.QuadPart;
        QueryPerformanceCounter(&start_time_);

        return true;
    }

    void PlatformWindows::Shutdown() {
        if(!platform_state_)
            return;

        if(platform_state_->hWnd) {
            DestroyWindow(platform_state_->hWnd);
            platform_state_->hWnd = nullptr;
        }

        platform_state_ = nullptr;
    }

    bool PlatformWindows::PumpMessage() {
        MSG message;
        while(PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return true;
    }

    void* PlatformWindows::Allocate(uint64_t size, bool aligned) {
        //TODO: need to do more
        return malloc(size);
    }

    void PlatformWindows::Deallocate(void *ptr, bool aligned) {
        //TODO: need to do moore
        free(ptr);
    }

    void * PlatformWindows::Zero_Memory(void *ptr, uint64_t size) {
        return memset(ptr, 0, size);
    }

    void * PlatformWindows::Copy_Memory(void *dest, const void *src, uint64_t size) {
        return memcpy(dest, src, size);
    }

    void * PlatformWindows::SetMemory(void *dest, int32_t value, uint64_t size) {
        return memset(dest, value, size);
    }

    bool PlatformWindows::CreateVulkanSurface(VulkanContext &context) {
        VkWin32SurfaceCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.hwnd = platform_state_->hWnd;

        VkResult result = vkCreateWin32SurfaceKHR(context.instance, &create_info, context.allocator, &platform_state_->surface);
        if(result != VK_SUCCESS) {
            NEO_FATAL("Vulkan surface creation failed.");
            return false;
        }

        context.surface = platform_state_->surface;
        return true;
    }

    void PlatformWindows::GetRequiredExtensionNames(DArray<const char *> &dArray) {
        dArray.PushBack("VK_KHR_win32_surface");
    }

    void PlatformWindows::ConsoleWrite(const std::string& message, const uint8_t color) {
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        //FATAL, ERROR, WARNING, INFO, DEBUG, TRACE
        static uint8_t level[6] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(console_handle, level[color]);
        OutputDebugStringA(message.c_str());
        uint64_t length = strlen(message.c_str());
        LPDWORD number_written = nullptr;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message.c_str(), (DWORD)length, number_written, nullptr);
    }

    void PlatformWindows::ConsoleWriteError(const std::string& message, uint8_t color) {
        HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
        //FATAL, ERROR, WARNING, INFO, DEBUG, TRACE
        static uint8_t level[6] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(console_handle, level[color]);
        OutputDebugStringA(message.c_str());
        uint64_t length = strlen(message.c_str());
        LPDWORD number_written = nullptr;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message.c_str(), (DWORD)length, number_written, nullptr);
    }

    double PlatformWindows::GetAbsoluteTime() {
        LARGE_INTEGER now_time;
        QueryPerformanceCounter(&now_time);
        return (double)now_time.QuadPart * clock_frequency_;
    }

    void PlatformWindows::Sleep_(uint64_t ms) {
        Sleep(ms);
    }

    LRESULT WindowsProcessMessage(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_ERASEBKGND:
                return 1;
            case WM_CLOSE:
                //TODO: quit
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_SIZE: {
                // RECT rect;
                // GetClientRect(hwnd, &rect);
                // uint32_t width = rect.right - rect.left;
                // uint32_t height = rect.bottom - rect.top;
                //TODO: resize
                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP: {
                bool pressed = (message==WM_KEYDOWN || message==WM_SYSKEYDOWN);
                Keys key = static_cast<Keys>(wParam);
                Input::GetInputSystem().ProcessKey(key, pressed);
            } break;
            case WM_MOUSEMOVE: {
                int16_t x = GET_X_LPARAM(lParam);
                int16_t y = GET_Y_LPARAM(lParam);

                Input::GetInputSystem().ProcessMouseMove(x, y);
            } break;
            case WM_MOUSEWHEEL: {
                int8_t z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (z_delta != 0) {
                    z_delta = (z_delta > 0) ? 1 : -1;

                    Input::GetInputSystem().ProcessMouseWheel(z_delta);
                }
            } break;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP: {
                bool pressed = message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN;
                MouseButtons mouse_button = MouseButtons::MOUSE_BUTTON_MAX;
                switch (message) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        mouse_button = MouseButtons::MOUSE_BUTTON_LEFT;
                        break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        mouse_button = MouseButtons::MOUSE_BUTTON_RIGHT;
                        break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        mouse_button = MouseButtons::MOUSE_BUTTON_MIDDLE;
                        break;
                }

                if(mouse_button != MouseButtons::MOUSE_BUTTON_MAX) {
                    Input::GetInputSystem().ProcessButton(mouse_button, pressed);
                }
            }
            default: ;
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}
