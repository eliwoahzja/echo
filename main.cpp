// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "fonts/sigma.h"
#include "fonts/Montserrat-Bold.h"
#include "fonts/Montserrat-Medium.h"
#include "fonts/logo.h"
#define rawData iconFontData
#include "fonts/IKONYKURWANOWE.H"
#undef rawData
#define rawData eyeIconFontData
#include "fonts/eye_f15e.h"
#undef rawData
#define rawData profileImageData
#include "fonts/pfp.h"
#undef rawData
#define rawData bgImageData
#include "imgui/bg.h"
#undef rawData
#include <d3d11.h>
#include <wincodec.h>
#include <tchar.h>
#include <dwmapi.h>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cfloat>
#include <cwchar>
#include <vector>
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
typedef enum _ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_INVALID_STATE = 5
} ACCENT_STATE;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
typedef struct _ACCENT_POLICY {
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
} ACCENT_POLICY;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
    DWORD Attribute;
    PVOID pData;
    ULONG DataSize;
} WINDOWCOMPOSITIONATTRIBDATA;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static constexpr int kMenuRegionWidth = 708;
static constexpr int kMenuHeight = 564;
static constexpr int kPopupOverflowWidth = 320;
static constexpr int kPopupOverflowHeight = 235;
static constexpr int kWindowWidth = kMenuRegionWidth + kPopupOverflowWidth;
static constexpr int kWindowHeight = kMenuHeight + kPopupOverflowHeight;
static constexpr int kAppWindowWidth = 1920;
static constexpr int kAppWindowHeight = 1080;
static constexpr int kWindowCornerRadius = 5;
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
static ID3D11ShaderResourceView* g_LogoTexture = nullptr;
static ID3D11ShaderResourceView* g_ProfileTexture = nullptr;
static ID3D11ShaderResourceView* g_BackgroundTexture = nullptr;
static UINT                     g_BackgroundTextureWidth = 0;
static UINT                     g_BackgroundTextureHeight = 0;
static ImFont*                  g_TitleFont = nullptr;
static ImFont*                  g_MediumFont = nullptr;
static ImFont*                  g_IconFont = nullptr;
static ImFont*                  g_EyeIconFont = nullptr;
static bool                     g_ModalOverlayHitTest = false;
static ID3D11Texture2D*         g_BlurReadbackTexture = nullptr;
static ID3D11Texture2D*         g_BlurUploadTexture = nullptr;
static ID3D11ShaderResourceView* g_BlurTextureView = nullptr;
static UINT                     g_BlurTextureWidth = 0;
static UINT                     g_BlurTextureHeight = 0;
static DXGI_FORMAT              g_BlurTextureFormat = DXGI_FORMAT_UNKNOWN;
static bool                     g_BlurCacheValid = false;
static bool                     g_BlurCaptureRequested = false;
static std::vector<unsigned char> g_BlurSourcePixels;
static std::vector<unsigned char> g_BlurTempPixels;
static std::vector<unsigned char> g_BlurOutputPixels;
static float                    g_UiScale = 1.0f;
static float                    g_UiTargetScale = 1.0f;
static float                    g_UiWindowScale = 1.0f;
static char                     g_PrimaryMonitorName[128] = "Monitor";
static bool                     g_BottomPopupRegionActive = false;
static bool                     g_BottomProfilePopupRegionActive = false;
static bool                     g_BottomOptionPopupRegionActive = false;
static bool                     g_RightPopupRegionActive = false;
static bool                     g_MenuPosInitialized = false;
static bool                     g_MenuDragging = false;
static ImVec2                   g_MenuPos(0.0f, 0.0f);
static ImVec2                   g_MenuDragDelta(0.0f, 0.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
bool CreateTextureFromPngMemory(const unsigned char* data, unsigned int data_size, ID3D11ShaderResourceView** out_srv, UINT* out_width = nullptr, UINT* out_height = nullptr);
void CleanupLogoTexture();
void CleanupProfileTexture();
void CleanupBackgroundTexture();
void CleanupBlurResources();
void CaptureGaussianBlurBackdrop();
void GaussianBlurDrawCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd);
void UpdateWindowRegion(HWND hWnd, bool include_bottom_popup_region, bool include_right_popup_region);
void UpdatePrimaryMonitorName();
void ApplyDarkTitleBar(HWND hWnd);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static bool DecodeEdidMonitorName(const BYTE* edid, DWORD edid_size, WCHAR* out_name, size_t out_name_count)
{
    if (!edid || edid_size < 128 || !out_name || out_name_count == 0)
        return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    out_name[0] = L'\0';
    for (int descriptor = 0; descriptor < 4; ++descriptor)
    {
        const int offset = 54 + descriptor * 18;
        if (offset + 18 > static_cast<int>(edid_size))
            break;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (edid[offset] != 0x00 || edid[offset + 1] != 0x00 || edid[offset + 2] != 0x00 || edid[offset + 4] != 0x00)
            continue;
        if (edid[offset + 3] != 0xFC && edid[offset + 3] != 0xFE)
            continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        char ascii_name[14] = {};
        int name_length = 0;
        for (int i = 0; i < 13; ++i)
        {
            const BYTE value = edid[offset + 5 + i];
            if (value == 0x00 || value == 0x0A)
                break;
            ascii_name[name_length++] = static_cast<char>(value);
        }
        while (name_length > 0 && ascii_name[name_length - 1] == ' ')
            --name_length;
        ascii_name[name_length] = '\0';
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (name_length <= 0)
            continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        MultiByteToWideChar(CP_ACP, 0, ascii_name, -1, out_name, static_cast<int>(out_name_count));
        out_name[out_name_count - 1] = L'\0';
        return out_name[0] != L'\0';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    return false;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static void ExtractMonitorHardwareId(const WCHAR* device_id, WCHAR* out_hardware_id, size_t out_hardware_id_count)
{
    if (!out_hardware_id || out_hardware_id_count == 0)
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    out_hardware_id[0] = L'\0';
    if (!device_id || device_id[0] == L'\0')
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const WCHAR* start = wcsstr(device_id, L"DISPLAY#");
    WCHAR separator = L'#';
    if (start)
    {
        start += 8;
    }
    else
    {
        start = wcsstr(device_id, L"MONITOR\\");
        separator = L'\\';
        if (start)
            start += 8;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (!start || start[0] == L'\0')
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const WCHAR* end = wcschr(start, separator);
    const size_t length = end ? static_cast<size_t>(end - start) : wcslen(start);
    if (length == 0)
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    wcsncpy_s(out_hardware_id, out_hardware_id_count, start, length);
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static bool TryReadEdidMonitorName(const WCHAR* preferred_hardware_id, WCHAR* out_name, size_t out_name_count)
{
    if (!out_name || out_name_count == 0)
        return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    out_name[0] = L'\0';
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    HKEY display_key = nullptr;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Enum\\DISPLAY", 0, KEY_READ, &display_key) != ERROR_SUCCESS)
        return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    bool found = false;
    for (int pass = 0; pass < 2 && !found; ++pass)
    {
        DWORD display_index = 0;
        WCHAR hardware_id[128] = {};
        DWORD hardware_id_size = IM_ARRAYSIZE(hardware_id);
        while (RegEnumKeyExW(display_key, display_index++, hardware_id, &hardware_id_size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
        {
            const bool match_preferred = preferred_hardware_id && preferred_hardware_id[0] != L'\0' && lstrcmpiW(hardware_id, preferred_hardware_id) == 0;
            if ((pass == 0 && !match_preferred) || (pass == 1 && preferred_hardware_id && preferred_hardware_id[0] != L'\0' && match_preferred))
            {
                hardware_id_size = IM_ARRAYSIZE(hardware_id);
                continue;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            HKEY hardware_key = nullptr;
            if (RegOpenKeyExW(display_key, hardware_id, 0, KEY_READ, &hardware_key) == ERROR_SUCCESS)
            {
                DWORD instance_index = 0;
                WCHAR instance_id[256] = {};
                DWORD instance_id_size = IM_ARRAYSIZE(instance_id);
                while (RegEnumKeyExW(hardware_key, instance_index++, instance_id, &instance_id_size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
                {
                    WCHAR device_parameters_path[384] = {};
                    swprintf_s(device_parameters_path, L"%s\\Device Parameters", instance_id);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    HKEY parameters_key = nullptr;
                    if (RegOpenKeyExW(hardware_key, device_parameters_path, 0, KEY_READ, &parameters_key) == ERROR_SUCCESS)
                    {
                        BYTE edid[256] = {};
                        DWORD edid_size = sizeof(edid);
                        DWORD value_type = 0;
                        if (RegQueryValueExW(parameters_key, L"EDID", nullptr, &value_type, edid, &edid_size) == ERROR_SUCCESS &&
                            value_type == REG_BINARY &&
                            DecodeEdidMonitorName(edid, edid_size, out_name, out_name_count))
                        {
                            found = true;
                        }
                        RegCloseKey(parameters_key);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (found)
                        break;
                    instance_id_size = IM_ARRAYSIZE(instance_id);
                }
                RegCloseKey(hardware_key);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            if (found)
                break;
            hardware_id_size = IM_ARRAYSIZE(hardware_id);
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    RegCloseKey(display_key);
    return found;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void UpdatePrimaryMonitorName()
{
    DISPLAY_DEVICEW display_device = {};
    display_device.cb = sizeof(display_device);
    WCHAR monitor_name[128] = L"";
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    for (DWORD device_index = 0; EnumDisplayDevicesW(nullptr, device_index, &display_device, 0); ++device_index)
    {
        if ((display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) == 0)
        {
            display_device.cb = sizeof(display_device);
            continue;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        DISPLAY_DEVICEW monitor_device = {};
        monitor_device.cb = sizeof(monitor_device);
        if (EnumDisplayDevicesW(display_device.DeviceName, 0, &monitor_device, EDD_GET_DEVICE_INTERFACE_NAME))
        {
            WCHAR hardware_id[128] = {};
            ExtractMonitorHardwareId(monitor_device.DeviceID, hardware_id, IM_ARRAYSIZE(hardware_id));
            if (!TryReadEdidMonitorName(hardware_id, monitor_name, IM_ARRAYSIZE(monitor_name)) &&
                monitor_device.DeviceString[0] != L'\0')
            {
                wcscpy_s(monitor_name, monitor_device.DeviceString);
            }
        }
        if (monitor_name[0] == L'\0' && display_device.DeviceString[0] != L'\0')
        {
            wcscpy_s(monitor_name, display_device.DeviceString);
        }
        break;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (monitor_name[0] == L'\0')
    {
        if (!TryReadEdidMonitorName(nullptr, monitor_name, IM_ARRAYSIZE(monitor_name)))
            wcscpy_s(monitor_name, L"Monitor");
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    WideCharToMultiByte(
        CP_UTF8,
        0,
        monitor_name,
        -1,
        g_PrimaryMonitorName,
        IM_ARRAYSIZE(g_PrimaryMonitorName),
        nullptr,
        nullptr);
    g_PrimaryMonitorName[IM_ARRAYSIZE(g_PrimaryMonitorName) - 1] = '\0';
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void UpdateWindowRegion(HWND hWnd, bool include_bottom_popup_region, bool include_right_popup_region)
{
    (void)hWnd;
    (void)include_bottom_popup_region;
    (void)include_right_popup_region;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    const HRESULT co_init_result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool com_initialized = SUCCEEDED(co_init_result);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ImGui_ImplWin32_EnableDpiAwareness();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Menu App", nullptr };
    ::RegisterClassExW(&wc);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    DWORD window_style = WS_OVERLAPPEDWINDOW;
    RECT window_rect = { 0, 0, kAppWindowWidth, kAppWindowHeight };
    ::AdjustWindowRect(&window_rect, window_style, FALSE);
    const int window_width = window_rect.right - window_rect.left;
    const int window_height = window_rect.bottom - window_rect.top;
    int window_x = (::GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
    int window_y = (::GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;
    if (window_x < 0)
        window_x = 0;
    if (window_y < 0)
        window_y = 0;
    HWND hwnd = ::CreateWindowW(
        wc.lpszClassName,
        L"ImGui DirectX11 Menu",
        window_style,
        window_x,
        window_y,
        window_width,
        window_height,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ApplyDarkTitleBar(hwnd);
    UpdatePrimaryMonitorName();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        if (com_initialized)
            CoUninitialize();
        return 1;
    }
    CreateTextureFromPngMemory(rawData, sizeof(rawData), &g_LogoTexture);
    CreateTextureFromPngMemory(profileImageData, sizeof(profileImageData), &g_ProfileTexture);
    CreateTextureFromPngMemory(bgImageData, sizeof(bgImageData), &g_BackgroundTexture, &g_BackgroundTextureWidth, &g_BackgroundTextureHeight);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig title_font_config;
    title_font_config.FontDataOwnedByAtlas = false;
    g_TitleFont = io.Fonts->AddFontFromMemoryTTF(bold, sizeof(bold), 14.0f, &title_font_config);
    ImFontConfig medium_font_config;
    medium_font_config.FontDataOwnedByAtlas = false;
    g_MediumFont = io.Fonts->AddFontFromMemoryTTF(medium, sizeof(medium), 14.0f, &medium_font_config);
    ImFontConfig icon_font_config;
    icon_font_config.FontDataOwnedByAtlas = false;
    icon_font_config.PixelSnapH = true;
    static const ImWchar icon_ranges[] = { 0xF000, 0xF9FF, 0 };
    g_IconFont = io.Fonts->AddFontFromMemoryTTF(iconFontData, sizeof(iconFontData), 16.0f, &icon_font_config, icon_ranges);
    ImFontConfig eye_icon_font_config;
    eye_icon_font_config.FontDataOwnedByAtlas = false;
    eye_icon_font_config.PixelSnapH = true;
    static const ImWchar eye_icon_ranges[] = { 0xF15E, 0xF15E, 0 };
    g_EyeIconFont = io.Fonts->AddFontFromMemoryTTF(eyeIconFontData, sizeof(eyeIconFontData), 16.0f, &eye_icon_font_config, eye_icon_ranges);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    bool done = false;
    while (!done)
    {
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        g_UiScale = 1.0f;
        g_UiWindowScale = g_UiScale;
        RECT client_rect{};
        ::GetClientRect(hwnd, &client_rect);
        const float client_width = static_cast<float>(client_rect.right - client_rect.left);
        const float client_height = static_cast<float>(client_rect.bottom - client_rect.top);
        io.DisplaySize = ImVec2(
            client_width > 1.0f ? client_width : 1.0f,
            client_height > 1.0f ? client_height : 1.0f);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        POINT current_mouse_pos{};
        if (::GetCursorPos(&current_mouse_pos) && ::ScreenToClient(hwnd, &current_mouse_pos))
        {
            io.AddMousePosEvent(
                static_cast<float>(current_mouse_pos.x),
                static_cast<float>(current_mouse_pos.y));
        }
        ImGui::NewFrame();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 app_canvas_size = io.DisplaySize;
        if (!g_MenuPosInitialized)
        {
            g_MenuPos = ImVec2(
                (app_canvas_size.x - static_cast<float>(kMenuRegionWidth)) * 0.5f,
                (app_canvas_size.y - static_cast<float>(kMenuHeight)) * 0.5f);
            g_MenuPos.x = std::round(g_MenuPos.x);
            g_MenuPos.y = std::round(g_MenuPos.y);
            if (g_MenuPos.x < 0.0f) g_MenuPos.x = 0.0f;
            if (g_MenuPos.y < 0.0f) g_MenuPos.y = 0.0f;
            g_MenuPosInitialized = true;
        }
        const ImVec2 raw_mouse_pos = io.MousePos;
        const ImVec2 local_mouse_pos(raw_mouse_pos.x - g_MenuPos.x, raw_mouse_pos.y - g_MenuPos.y);
        const bool mouse_down = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        if (!mouse_down)
            g_MenuDragging = false;
        if (g_MenuDragging)
        {
            g_MenuPos = ImVec2(raw_mouse_pos.x - g_MenuDragDelta.x, raw_mouse_pos.y - g_MenuDragDelta.y);
            g_MenuPos.x = std::round(g_MenuPos.x);
            g_MenuPos.y = std::round(g_MenuPos.y);
            const float max_x = app_canvas_size.x - static_cast<float>(kMenuRegionWidth);
            const float max_y = app_canvas_size.y - static_cast<float>(kMenuHeight);
            if (g_MenuPos.x < 0.0f) g_MenuPos.x = 0.0f;
            if (g_MenuPos.y < 0.0f) g_MenuPos.y = 0.0f;
            if (max_x > 0.0f && g_MenuPos.x > max_x) g_MenuPos.x = max_x;
            if (max_y > 0.0f && g_MenuPos.y > max_y) g_MenuPos.y = max_y;
        }
        else
        {
            const float max_x = app_canvas_size.x - static_cast<float>(kMenuRegionWidth);
            const float max_y = app_canvas_size.y - static_cast<float>(kMenuHeight);
            if (g_MenuPos.x < 0.0f) g_MenuPos.x = 0.0f;
            if (g_MenuPos.y < 0.0f) g_MenuPos.y = 0.0f;
            if (max_x > 0.0f && g_MenuPos.x > max_x) g_MenuPos.x = max_x;
            if (max_y > 0.0f && g_MenuPos.y > max_y) g_MenuPos.y = max_y;
            g_MenuPos.x = std::round(g_MenuPos.x);
            g_MenuPos.y = std::round(g_MenuPos.y);
        }
        ImDrawList* background_draw_list = ImGui::GetBackgroundDrawList();
        ImDrawList* foreground_draw_list = ImGui::GetForegroundDrawList();
        if (g_BackgroundTexture && g_BackgroundTextureWidth > 0 && g_BackgroundTextureHeight > 0)
        {
            const float canvas_aspect = app_canvas_size.x / app_canvas_size.y;
            const float texture_aspect = static_cast<float>(g_BackgroundTextureWidth) / static_cast<float>(g_BackgroundTextureHeight);
            ImVec2 uv_min(0.0f, 0.0f);
            ImVec2 uv_max(1.0f, 1.0f);
            if (canvas_aspect > texture_aspect)
            {
                const float visible_height = texture_aspect / canvas_aspect;
                const float crop_y = (1.0f - visible_height) * 0.5f;
                uv_min.y = crop_y;
                uv_max.y = 1.0f - crop_y;
            }
            else if (canvas_aspect < texture_aspect)
            {
                const float visible_width = canvas_aspect / texture_aspect;
                const float crop_x = (1.0f - visible_width) * 0.5f;
                uv_min.x = crop_x;
                uv_max.x = 1.0f - crop_x;
            }
            background_draw_list->AddImage(
                (ImTextureID)g_BackgroundTexture,
                ImVec2(0.0f, 0.0f),
                app_canvas_size,
                uv_min,
                uv_max,
                IM_COL32_WHITE);
        }
        else
        {
            background_draw_list->AddRectFilled(ImVec2(0.0f, 0.0f), app_canvas_size, IM_COL32(8, 9, 13, 255));
        }
        const int background_vertex_start = background_draw_list->VtxBuffer.Size;
        const int foreground_vertex_start = foreground_draw_list->VtxBuffer.Size;
        const int background_command_start = background_draw_list->CmdBuffer.Size;
        const int foreground_command_start = foreground_draw_list->CmdBuffer.Size;
        struct WindowDrawListStart
        {
            ImDrawList* drawList;
            int vertexStart;
            int commandStart;
        };
        std::vector<WindowDrawListStart> window_draw_list_starts;
        ImGuiContext* imgui_context = ImGui::GetCurrentContext();
        if (imgui_context)
        {
            window_draw_list_starts.reserve(static_cast<size_t>(imgui_context->Windows.Size));
            for (int window_index = 0; window_index < imgui_context->Windows.Size; ++window_index)
            {
                ImGuiWindow* window = imgui_context->Windows[window_index];
                if (window && window->DrawList)
                    window_draw_list_starts.push_back({ window->DrawList, window->DrawList->VtxBuffer.Size, window->DrawList->CmdBuffer.Size });
            }
        }
        io.MousePos = local_mouse_pos;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const auto mouse_in_rect = [](const ImVec2& mouse, const ImVec2& min, const ImVec2& max) -> bool
        {
            return mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y;
        };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 menu_canvas_size(static_cast<float>(kMenuRegionWidth), static_cast<float>(kMenuHeight));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(0.0f, 0.0f),
            menu_canvas_size,
            IM_COL32(45, 45, 45, 120), 
            5.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(1.0f, 1.0f),
            ImVec2(menu_canvas_size.x - 1.0f, menu_canvas_size.y - 1.0f),
            IM_COL32(10, 10, 10, 120),
            4.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(4.0f, 4.0f),
            ImVec2(menu_canvas_size.x - 4.0f, menu_canvas_size.y - 4.0f),
            IM_COL32(6, 6, 6, 255),
            2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float title_font_size = 14.0f;
        const float first_bar_x = 4.0f + 113.0f;
        const float first_bar_y = 4.0f + 24.0f;
        const char* title_white = "ech0.";
        const char* title_accent = "fun";
        ImFont* title_font = g_TitleFont ? g_TitleFont : ImGui::GetFont();
        const ImVec2 title_white_size = title_font->CalcTextSizeA(title_font_size, 1000.0f, 0.0f, title_white);
        const ImVec2 title_accent_size = title_font->CalcTextSizeA(title_font_size, 1000.0f, 0.0f, title_accent);
        const ImVec2 title_pos(
            first_bar_x - 8.0f - title_white_size.x - title_accent_size.x,
            first_bar_y + 8.0f - title_font_size + 1.0f);
        ImGui::GetBackgroundDrawList()->AddText(
            title_font,
            title_font_size,
            title_pos,
            IM_COL32(235, 235, 235, 255),
            title_white);
        ImGui::GetBackgroundDrawList()->AddText(
            title_font,
            title_font_size,
            ImVec2(title_pos.x + title_white_size.x, title_pos.y),
            IM_COL32(142, 134, 246, 255),
            title_accent);
        if (g_LogoTexture)
        {
            constexpr float logo_size = 16.0f;
            const ImVec2 logo_pos(title_pos.x - 8.0f - logo_size, title_pos.y + title_font_size - logo_size + 1.0f);
            ImGui::GetBackgroundDrawList()->AddImage(
                (ImTextureID)g_LogoTexture,
                logo_pos,
                ImVec2(logo_pos.x + logo_size, logo_pos.y + logo_size));
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(4.0f + 113.0f, 4.0f + 24.0f),
            ImVec2(4.0f + 113.0f + 1.0f, 4.0f + 24.0f + 8.0f),
            IM_COL32(16, 16, 16, 255));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const char* sidebar_icons[] = {
            u8"\uf53b",
            u8"\uf07f",
            u8"\uf15e",
            u8"\uf58e",
            u8"\uf142",
            u8"\uf6d6",
            u8"\uf045",
            u8"\uf3dc"
        };
        const char* main_tab_labels[] = {
            "Self",
            "Combat",
            "Visuals",
            "Entity List",
            "Vehicle",
            "Server",
            "Configurations",
            "Settings"
        };
        constexpr int max_subtabs = 5;
        const int subtab_counts[] = { 4, 5, 4, 4, 3, 3, 0, 0 };
        const char* subtab_icons[][max_subtabs] = {
            { u8"\uf571", u8"\uf07f", u8"\uf044", u8"\uf450", "" },
            { u8"\uf572", u8"\uf677", u8"\uf05f", u8"\uf219", u8"\uf3b8" },
            { u8"\uf53b", u8"\uf142", u8"\uf661", u8"\uf0fd", "" },
            { u8"\uf040", u8"\uf142", u8"\uf661", u8"\uf040", "" },
            { u8"\uf142", u8"\uf142", u8"\uf142", "", "" },
            { u8"\uf095", u8"\uf6d6", u8"\uf045", "", "" },
            { "", "", "", "", "" },
            { u8"\uf3dc", "", "", "", "" }
        };
        const char* subtab_labels[][max_subtabs] = {
            { "Player", "Weapon", "Teleports", "Strafe macro", "" },
            { "Aimlock", "Silent", "Trigger", "FlickBot", "Custom" },
            { "Players", "Vehicles", "Objects", "World", "" },
            { "Players", "Vehicles", "Objects", "Friend Helper", "" },
            { "Current Vehicle", "Vehicle Spawner", "Vehicle Mods", "", "" },
            { "Executor", "Resources", "Dumper", "", "" },
            { "", "", "", "", "" },
            { "Settings", "", "", "", "" }
        };
        ImFont* icon_font = g_IconFont ? g_IconFont : ImGui::GetFont();
        ImFont* medium_font = g_MediumFont ? g_MediumFont : title_font;
        sigma::set_context(title_font, icon_font, medium_font, io.DeltaTime);
        const float icon_font_size = 16.0f;
        const float top_tab_icon_size = 16.0f;
        const float top_tab_text_size = 14.0f;
        const float top_tab_icon_x = 134.0f;
        const float top_tab_text_x = 154.0f;
        const float top_tab_icon_y = 22.0f;
        const float top_tab_text_y = 23.0f;
        const float sidebar_icon_center_x = 35.0f;
        const float sidebar_icon_start_y = 72.0f;
        const float sidebar_icon_step_y = 38.0f;
        const float accent_bar_x = 4.0f + 14.0f;
        const float accent_bar_y = 4.0f + 70.0f;
        const float accent_bar_width = 2.0f;
        const float accent_bar_height = 12.0f;
        static int active_sidebar_tab = 0;
        static int displayed_top_tab = 0;
        static float topbar_visibility = 1.0f;
        static float subtabbar_visibility = 1.0f;
        static int active_sub_tab[IM_ARRAYSIZE(main_tab_labels)] = {};
        static int displayed_sub_tab[IM_ARRAYSIZE(main_tab_labels)] = {};
        static float subtab_content_visibility = 1.0f;
        static float subtab_fade[IM_ARRAYSIZE(main_tab_labels)][max_subtabs] = {};
        static float subtab_hover_fade[IM_ARRAYSIZE(main_tab_labels)][max_subtabs] = {};
        static float sidebar_icon_fade[IM_ARRAYSIZE(sidebar_icons)] = {};
        static float sidebar_bar_fade[IM_ARRAYSIZE(sidebar_icons)] = {};
        static float sidebar_hover_fade[IM_ARRAYSIZE(sidebar_icons)] = {};
        static float sidebar_tooltip_fade[IM_ARRAYSIZE(sidebar_icons)] = {};
        static bool bottom_popup_open = false;
        static float bottom_popup_fade = 0.0f;
        static float bottom_icon_hover_fade = 0.0f;
        static bool bottom_popup_dpi_dragging = false;
        static int bottom_popup_pending_dpi_index = 1;
        static float bottom_popup_dpi_hover_fade = 0.0f;
        static int bottom_popup_theme_index = 0;
        static float bottom_popup_theme_switch_t = 0.0f;
        static int bottom_popup_language_index = 0;
        static int bottom_popup_device_index = 0;
        static float bottom_popup_theme_hover_fade[2] = {};
        static float bottom_popup_combo_hover_fade[2] = {};
        static bool bottom_popup_combo_open[2] = {};
        static float bottom_popup_combo_dropdown_fade[2] = {};
        static float bottom_popup_combo_item_fade[2][2] = {};
        static float teleport_button_hover_fade[4] = {};
        static int teleport_location_selected_index = 0;
        static bool teleport_location_dropdown_open = false;
        static float teleport_location_dropdown_fade = 0.0f;
        static float teleport_location_text_fade[7] = {};
        static float location_manager_button_hover_fade[2] = {};
        static float executor_button_hover_fade[2] = {};
        static float dumper_button_hover_fade = 0.0f;
        static float self_player_scroll_y = 0.0f;
        static float self_player_scroll_target_y = 0.0f;
        static bool self_option_enabled[13] = {};
        static float self_option_toggle_fade[13] = {};
        static float self_option_hover_fade[13] = {};
        static bool self_keybind_popup_open[9] = {};
        static bool self_keybind_waiting[9] = {};
        static float self_keybind_popup_fade[9] = {};
        static float self_gear_hover_fade[9] = {};
        static float self_key_hover_fade[9] = {};
        static int self_keybind_wait_frame[9] = {};
        static ImGuiKey self_keybinds[9] = {};
        static bool self_visual_option_enabled[3] = {};
        static float self_visual_option_toggle_fade[3] = {};
        static float self_visual_option_hover_fade[3] = {};
        static bool gameplay_option_enabled[5] = {};
        static float gameplay_option_toggle_fade[5] = {};
        static float gameplay_option_hover_fade[5] = {};
        static bool gameplay_popup_open[5] = {};
        static bool gameplay_keybind_waiting[5] = {};
        static float gameplay_popup_fade[5] = {};
        static float gameplay_gear_hover_fade[5] = {};
        static float gameplay_key_hover_fade[5] = {};
        static int gameplay_keybind_wait_frame[5] = {};
        static ImGuiKey gameplay_keybinds[5] = {};
        static float fake_lag_delay_value = 200.0f;
        static float fake_lag_delay_target_value = 200.0f;
        static bool fake_lag_delay_dragging = false;
        static float damage_reduction_value = 50.0f;
        static float damage_reduction_target_value = 50.0f;
        static bool damage_reduction_dragging = false;
        static bool wheel_silent_target_line = false;
        static float wheel_silent_target_line_fade = 0.0f;
        static float wheel_silent_distance_value = 300.0f;
        static float wheel_silent_distance_target_value = 300.0f;
        static bool wheel_silent_distance_dragging = false;
        static bool health_option_enabled[2] = {};
        static float health_option_toggle_fade[2] = {};
        static float health_option_hover_fade[2] = {};
        static float health_slider_value = 200.0f;
        static float health_slider_target_value = 200.0f;
        static bool health_slider_dragging = false;
        static float armor_slider_value = 0.0f;
        static float armor_slider_target_value = 0.0f;
        static bool armor_slider_dragging = false;
        static float auto_heal_expand_fade = 0.0f;
        static float minimum_hp_slider_value = 50.0f;
        static float minimum_hp_slider_target_value = 50.0f;
        static bool minimum_hp_slider_dragging = false;
        static float heal_amount_slider_value = 100.0f;
        static float heal_amount_slider_target_value = 100.0f;
        static bool heal_amount_slider_dragging = false;
        static float heal_delay_slider_value = 0.10f;
        static float heal_delay_slider_target_value = 0.10f;
        static bool heal_delay_slider_dragging = false;
        static float auto_armor_expand_fade = 0.0f;
        static float minimum_armor_slider_value = 20.0f;
        static float minimum_armor_slider_target_value = 20.0f;
        static bool minimum_armor_slider_dragging = false;
        static float armor_amount_slider_value = 100.0f;
        static float armor_amount_slider_target_value = 100.0f;
        static bool armor_amount_slider_dragging = false;
        static float armor_delay_slider_value = 0.10f;
        static float armor_delay_slider_target_value = 0.10f;
        static bool armor_delay_slider_dragging = false;
        static float health_action_button_hover_fade[2] = {};
        static bool weapon_option_enabled[7] = {};
        static float weapon_option_toggle_fade[7] = {};
        static float weapon_option_hover_fade[7] = {};
        static bool weapon_tuning_popup_open[4] = {};
        static float weapon_tuning_popup_fade[4] = {};
        static float weapon_tuning_gear_hover_fade[4] = {};
        static float weapon_tuning_value[4] = {};
        static float weapon_tuning_target_value[4] = {};
        static bool weapon_tuning_dragging[4] = {};
        static int weapon_attachment_selected_index = 0;
        static bool weapon_attachment_dropdown_open = false;
        static float weapon_attachment_dropdown_fade = 0.0f;
        static float weapon_attachment_dropdown_scroll = 0.0f;
        static float weapon_attachment_dropdown_target_scroll = 0.0f;
        static float weapon_attachment_text_fade[11] = {};
        static bool weapon_spawner_enabled[2] = {};
        static float weapon_spawner_toggle_fade[2] = {};
        static float weapon_spawner_hover_fade[2] = {};
        static bool world_bullet_tracer_enabled[5] = {};
        static float world_bullet_tracer_toggle_fade[5] = {};
        static float world_bullet_tracer_hover_fade[5] = {};
        static bool world_hit_data_enabled[3] = {};
        static float world_hit_data_toggle_fade[3] = {};
        static float world_hit_data_hover_fade[3] = {};
        static bool world_extra_visuals_enabled[3] = {};
        static float world_extra_visuals_toggle_fade[3] = {};
        static float world_extra_visuals_hover_fade[3] = {};
        static bool world_popup_open[6] = {};
        static float world_popup_fade[6] = {};
        static float world_gear_hover_fade[6] = {};
        static float world_popup_slider_value[20] = {
            3.0f, 50.0f, 1.0f, 12.0f,
            2.0f, 5.0f,
            1.0f, 2.0f,
            1.0f,
            10.0f, 2.0f, 5.0f, 1.0f,
            5.0f, 1.0f, 6.0f, 3.0f
        };
        static float world_popup_slider_target_value[20] = {
            3.0f, 50.0f, 1.0f, 12.0f,
            2.0f, 5.0f,
            1.0f, 2.0f,
            1.0f,
            10.0f, 2.0f, 5.0f, 1.0f,
            5.0f, 1.0f, 6.0f, 3.0f
        };
        static bool world_popup_slider_dragging[20] = {};
        static bool world_popup_toggle_enabled[2] = { true, false };
        static float world_popup_toggle_fade[2] = { 1.0f, 0.0f };
        static bool vehicle_visuals_enabled[6] = {};
        static float vehicle_visuals_toggle_fade[6] = {};
        static float vehicle_visuals_hover_fade[6] = {};
        static bool vehicle_esp_enabled[3] = {};
        static float vehicle_esp_toggle_fade[3] = {};
        static float vehicle_esp_hover_fade[3] = {};
        static float vehicle_distance_value = 200.0f;
        static float vehicle_distance_target_value = 200.0f;
        static bool vehicle_distance_dragging = false;
        static bool vehicle_health_enabled[2] = {};
        static float vehicle_health_toggle_fade[2] = {};
        static float vehicle_health_hover_fade[2] = {};
        static bool friend_helper_enabled[3] = {};
        static float friend_helper_toggle_fade[3] = {};
        static float friend_helper_hover_fade[3] = {};
        static float friend_helper_distance_value = 50.0f;
        static float friend_helper_distance_target_value = 50.0f;
        static bool friend_helper_distance_dragging = false;
        static float friend_helper_button_hover_fade[2] = {};
        static char entity_player_search[64] = "";
        static char entity_vehicle_search[64] = "";
        static char entity_object_search[64] = "";
        static bool player_visuals_enabled[14] = {};
        static float player_visuals_toggle_fade[14] = {};
        static float player_visuals_hover_fade[14] = {};
        static bool player_settings_enabled[8] = {};
        static float player_settings_toggle_fade[8] = {};
        static float player_settings_hover_fade[8] = {};
        static float player_max_distance_value = 200.0f;
        static float player_max_distance_target_value = 200.0f;
        static bool player_max_distance_dragging = false;
        static bool visual_popup_open[12] = {};
        static float visual_popup_fade[12] = {};
        static float visual_gear_hover_fade[12] = {};
        static bool visual_keybind_waiting[12] = {};
        static ImGuiKey visual_keybinds[12] = {};
        static int visual_keybind_wait_frame[12] = {};
        static float visual_key_hover_fade[12] = {};
        static float visual_popup_slider_value[8] = { 1.0f, 1.5f, 50.0f, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        static float visual_popup_slider_target_value[8] = { 1.0f, 1.5f, 50.0f, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        static bool visual_popup_slider_dragging[8] = {};
        static bool visual_popup_toggle_enabled[8] = {};
        static float visual_popup_toggle_fade[8] = {};
        static bool object_visuals_enabled[5] = {};
        static float object_visuals_toggle_fade[5] = {};
        static float object_visuals_hover_fade[5] = {};
        static float object_max_distance_value = 200.0f;
        static float object_max_distance_target_value = 200.0f;
        static bool object_max_distance_dragging = false;
        static float object_list_button_hover_fade[2] = {};
        struct ObjectEntry
        {
            char hash[32] = "";
            char name[32] = "";
            float delete_hover_fade = 0.0f;
        };
        static ObjectEntry object_entries[10] = {};
        static int object_entry_count = 0;
        static bool object_add_overlay_open = false;
        static bool object_add_overlay_click_lock = false;
        static float object_add_overlay_fade = 0.0f;
        static float object_add_close_hover_fade = 0.0f;
        static float object_add_confirm_hover_fade = 0.0f;
        static char object_add_hash_input[32] = "";
        static char object_add_name_input[32] = "";
        static char weapon_search[64] = "";
        static float weapon_ammo_value = 0.0f;
        static float weapon_ammo_target_value = 0.0f;
        static bool weapon_ammo_dragging = false;
        static float weapon_give_ammo_value = 250.0f;
        static float weapon_give_ammo_target_value = 250.0f;
        static bool weapon_give_ammo_dragging = false;
        static float weapon_button_hover_fade[2] = {};
        static int weapon_spawner_selected_category_index = 5;
        static bool weapon_spawner_category_dropdown_open = false;
        static float weapon_spawner_category_dropdown_fade = 0.0f;
        static float weapon_spawner_category_text_fade[7] = {};
        static int weapon_spawner_selected_weapon_index = 4;
        static bool weapon_spawner_weapon_dropdown_open = false;
        static float weapon_spawner_weapon_dropdown_fade = 0.0f;
        static float weapon_spawner_weapon_text_fade[7] = {};
        static char vehicle_spawner_search[64] = "";
        static int selected_spawn_vehicle_index = -1;
        static float vehicle_spawn_selection_y = 0.0f;
        static float vehicle_spawn_selection_slide = 0.0f;
        static float vehicle_spawn_selection_alpha = 0.0f;
        static float vehicle_spawn_item_text_fade[8] = {};
        static bool vehicle_spawn_option_enabled[3] = {};
        static float vehicle_spawn_option_toggle_fade[3] = {};
        static float vehicle_spawn_option_hover_fade[3] = {};
        static float vehicle_spawn_button_hover_fade[2] = {};
        static float vehicle_mods_quick_button_hover_fade = 0.0f;
        static bool macro_activation_enabled[3] = {};
        static float macro_activation_toggle_fade[3] = {};
        static float macro_activation_hover_fade[3] = {};
        static bool macro_enable_popup_open = false;
        static float macro_enable_popup_fade = 0.0f;
        static float macro_enable_gear_hover_fade = 0.0f;
        static bool macro_enable_keybind_waiting = false;
        static int macro_enable_keybind_wait_frame = 0;
        static ImGuiKey macro_enable_keybind = ImGuiKey_None;
        static float macro_enable_key_hover_fade = 0.0f;
        static bool macro_require_shift = false;
        static float macro_require_shift_toggle_fade = 0.0f;
        static float macro_circle_radius_value = 80.0f;
        static float macro_circle_radius_target_value = 80.0f;
        static bool macro_circle_radius_dragging = false;
        static float macro_circle_thickness_value = 2.0f;
        static float macro_circle_thickness_target_value = 2.0f;
        static bool macro_circle_thickness_dragging = false;
        static float macro_circle_segments_value = 96.0f;
        static float macro_circle_segments_target_value = 96.0f;
        static bool macro_circle_segments_dragging = false;
        static float macro_strafe_interval_value = 90.0f;
        static float macro_strafe_interval_target_value = 90.0f;
        static bool macro_strafe_interval_dragging = false;
        static float macro_release_gap_value = 0.0f;
        static float macro_release_gap_target_value = 0.0f;
        static bool macro_release_gap_dragging = false;
        static float macro_humanize_jitter_value = 0.0f;
        static float macro_humanize_jitter_target_value = 0.0f;
        static bool macro_humanize_jitter_dragging = false;
        static int macro_tuning_mode_selected_index = 0;
        static bool macro_tuning_mode_dropdown_open = false;
        static float macro_tuning_mode_dropdown_fade = 0.0f;
        static float macro_tuning_mode_text_fade[5] = {};
        static float macro_scroll_y = 0.0f;
        static float macro_scroll_target_y = 0.0f;
        static float macro_button_hover_fade[3] = {};
        static int macro_preset_selected_index = 0;
        static bool macro_preset_dropdown_open = false;
        static float macro_preset_dropdown_fade = 0.0f;
        static float macro_preset_dropdown_scroll = 0.0f;
        static float macro_preset_dropdown_target_scroll = 0.0f;
        static float macro_preset_text_fade[10] = {};
        static char executor_code[8192] = "-- ech0.fun executor\nprint(\"Hello World!\")";
        static char resource_search[64] = "";
        static int selected_resource_index = -1;
        static float resource_selection_y = 0.0f;
        static float resource_selection_slide = 0.0f;
        static float resource_selection_alpha = 0.0f;
        static float resource_item_text_fade[2] = {};
        static bool resource_running[2] = { true, true };
        static float resource_action_button_hover_fade[2] = {};
        static char dumper_filter[64] = "";
        static float configuration_create_button_hover_fade = 0.0f;
        static float configuration_icon_hover_fade[2] = {};
        struct ConfigEntry
        {
            char name[19] = "";
            char created_at[12] = "";
            bool editing = false;
            bool deleting = false;
            float fade = 1.0f;
            float underline_fade = 0.0f;
            float button_hover_fade = 0.0f;
            float action_hover_fade[2] = {};
        };
        static ConfigEntry configs[9] = {};
        static int config_count = 0;
        static bool configs_initialized = false;
        static char configuration_search[64] = "";
        static bool create_overlay_open = false;
        static bool create_overlay_click_lock = false;
        static float create_overlay_fade = 0.0f;
        static float create_close_hover_fade = 0.0f;
        static float create_confirm_hover_fade = 0.0f;
        static bool create_private_config_enabled = false;
        static bool create_hide_username_enabled = false;
        static float create_private_config_fade = 0.0f;
        static float create_hide_username_fade = 0.0f;
        static float create_private_config_hover_fade = 0.0f;
        static float create_hide_username_hover_fade = 0.0f;
        static char create_config_input[19] = "";
        static bool quick_presets_overlay_open = false;
        static bool quick_presets_overlay_click_lock = false;
        static float quick_presets_overlay_fade = 0.0f;
        static float quick_presets_close_hover_fade = 0.0f;
        static float quick_presets_button_hover_fade[8] = {};
        static bool settings_general_enabled[3] = { false, false, true };
        static float settings_general_toggle_fade[3] = {};
        static float settings_general_hover_fade[3] = {};
        static bool settings_addon_enabled[5] = {};
        static float settings_addon_toggle_fade[5] = {};
        static float settings_addon_hover_fade[5] = {};
        static float settings_button_hover_fade[2] = {};
        static bool aimlock_option_enabled[12] = {};
        static float aimlock_option_toggle_fade[12] = {};
        static float aimlock_option_hover_fade[12] = {};
        static float aimlock_acceleration_curve_fade = 0.0f;
        static float aimlock_acceleration_curve_values[5] = { 0.67f, 0.72f, 0.50f, 0.25f, 0.0f };
        static int aimlock_acceleration_drag_point = -1;
        static float aimlock_fov_value = 180.0f;
        static float aimlock_fov_target_value = 180.0f;
        static bool aimlock_fov_dragging = false;
        static float aimlock_speed_x_value = 50.0f;
        static float aimlock_speed_x_target_value = 50.0f;
        static bool aimlock_speed_x_dragging = false;
        static float aimlock_speed_y_value = 50.0f;
        static float aimlock_speed_y_target_value = 50.0f;
        static bool aimlock_speed_y_dragging = false;
        static float aimlock_speed_max_value = 100.0f;
        static float aimlock_speed_max_target_value = 100.0f;
        static bool aimlock_speed_max_dragging = false;
        static float aimlock_deadzone_value = 0.0f;
        static float aimlock_deadzone_target_value = 0.0f;
        static bool aimlock_deadzone_dragging = false;
        static float aimlock_vertical_offset_value = 0.0f;
        static float aimlock_vertical_offset_target_value = 0.0f;
        static bool aimlock_vertical_offset_dragging = false;
        static float aimlock_retarget_time_value = 250.0f;
        static float aimlock_retarget_time_target_value = 250.0f;
        static bool aimlock_retarget_time_dragging = false;
        static float aimlock_detach_time_value = 0.0f;
        static float aimlock_detach_time_target_value = 0.0f;
        static bool aimlock_detach_time_dragging = false;
        static float aimlock_max_distance_value = 240.0f;
        static float aimlock_max_distance_target_value = 240.0f;
        static bool aimlock_max_distance_dragging = false;
        static float aimlock_settings_scroll_y = 0.0f;
        static float aimlock_settings_scroll_target_y = 0.0f;
        static bool silent_option_enabled[9] = {};
        static float silent_option_toggle_fade[9] = {};
        static float silent_option_hover_fade[9] = {};
        static float silent_fov_value = 40.0f;
        static float silent_fov_target_value = 40.0f;
        static bool silent_fov_dragging = false;
        static float silent_miss_chance_value = 0.0f;
        static float silent_miss_chance_target_value = 0.0f;
        static bool silent_miss_chance_dragging = false;
        static float silent_max_distance_value = 200.0f;
        static float silent_max_distance_target_value = 200.0f;
        static bool silent_max_distance_dragging = false;
        static bool trigger_option_enabled[11] = {};
        static float trigger_option_toggle_fade[11] = {};
        static float trigger_option_hover_fade[11] = {};
        static float trigger_fov_value = 20.0f;
        static float trigger_fov_target_value = 20.0f;
        static bool trigger_fov_dragging = false;
        static float trigger_max_distance_value = 200.0f;
        static float trigger_max_distance_target_value = 200.0f;
        static bool trigger_max_distance_dragging = false;
        static float trigger_min_distance_value = 0.0f;
        static float trigger_min_distance_target_value = 0.0f;
        static bool trigger_min_distance_dragging = false;
        static float trigger_reaction_time_value = 0.0f;
        static float trigger_reaction_time_target_value = 0.0f;
        static bool trigger_reaction_time_dragging = false;
        static float trigger_delay_value = 0.0f;
        static float trigger_delay_target_value = 0.0f;
        static bool trigger_delay_dragging = false;
        static bool flickbot_option_enabled[7] = {};
        static float flickbot_option_toggle_fade[7] = {};
        static float flickbot_option_hover_fade[7] = {};
        static float flickbot_fov_value = 40.0f;
        static float flickbot_fov_target_value = 40.0f;
        static bool flickbot_fov_dragging = false;
        static float flickbot_distance_value = 200.0f;
        static float flickbot_distance_target_value = 200.0f;
        static bool flickbot_distance_dragging = false;
        static float flickbot_speed_value = 80.0f;
        static float flickbot_speed_target_value = 80.0f;
        static bool flickbot_speed_dragging = false;
        static float flickbot_threshold_value = 15.0f;
        static float flickbot_threshold_target_value = 15.0f;
        static bool flickbot_threshold_dragging = false;
        static bool combat_popup_open[10] = {};
        static float combat_popup_fade[10] = {};
        static float combat_gear_hover_fade[10] = {};
        static bool combat_keybind_waiting[10] = {};
        static ImGuiKey combat_keybinds[10] = {};
        static int combat_keybind_wait_frame[10] = {};
        static float combat_key_hover_fade[10] = {};
        static float combat_popup_slider_value[16] = {
            0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            20.0f,
            0.0f, 0.0f,
            40.0f, 10.0f, 1.0f,
            40.0f,
            0.0f, 0.0f, 0.0f
        };
        static float combat_popup_slider_target_value[16] = {
            0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            20.0f,
            0.0f, 0.0f,
            40.0f, 10.0f, 1.0f,
            40.0f,
            0.0f, 0.0f, 0.0f
        };
        static bool combat_popup_slider_dragging[16] = {};
        static bool combat_popup_toggle_enabled[4] = {};
        static float combat_popup_toggle_fade[4] = {};
        static bool combat_dropdown_open[10] = {};
        static float combat_dropdown_fade[10] = {};
        static float combat_dropdown_scroll[10] = {};
        static float combat_dropdown_target_scroll[10] = {};
        static float combat_dropdown_text_fade[10][10] = {};
        static float combat_dropdown_select_fade[10][10] = {};
        static int combat_single_selected[5] = {};
        static int combat_single_pending[5] = { -1, -1, -1, -1, -1 };
        static unsigned int combat_multi_mask[5] = { 1u, 2u, 1u, 1u, 1u };
        static bool hitbox_option_enabled[2] = { false, false };
        static float hitbox_option_toggle_fade[2] = { 0.0f, 0.0f };
        static float hitbox_option_hover_fade[2] = {};
        static float hitbox_height_value = 0.10f;
        static float hitbox_height_target_value = 0.10f;
        static bool hitbox_height_dragging = false;
        static float hitbox_width_value = 0.13f;
        static float hitbox_width_target_value = 0.13f;
        static bool hitbox_width_dragging = false;
        static float hitbox_preview_expand_fade = 0.0f;
        static float hitbox_button_hover_fade[3] = {};
        static ImGuiKey settings_key_bindings[2] = { ImGuiKey_F4, ImGuiKey_F11 };
        static bool settings_key_waiting[2] = {};
        static float settings_key_hover_fade[2] = {};
        static float settings_key_width[2] = {};
        static bool saved_location_enabled = false;
        static float saved_location_toggle_fade = 0.0f;
        static float saved_location_toggle_hover_fade = 0.0f;
        static bool sidebar_anim_initialized = false;
        const auto format_config_time = [](char* output, size_t output_size)
        {
            std::time_t now = std::time(nullptr);
            std::tm local_time = {};
            localtime_s(&local_time, &now);
            sprintf_s(
                output,
                output_size,
                "%02d/%02d %02d:%02d",
                local_time.tm_mon + 1,
                local_time.tm_mday,
                local_time.tm_hour,
                local_time.tm_min);
        };
        const auto lower_ascii = [](char value) -> char
        {
            return value >= 'A' && value <= 'Z' ? static_cast<char>(value - 'A' + 'a') : value;
        };
        const auto contains_case_insensitive = [&lower_ascii](const char* haystack, const char* needle) -> bool
        {
            if (!needle || needle[0] == '\0')
                return true;
            if (!haystack)
                return false;
            for (int start = 0; haystack[start] != '\0'; ++start)
            {
                int haystack_index = start;
                int needle_index = 0;
                while (haystack[haystack_index] != '\0' &&
                    needle[needle_index] != '\0' &&
                    lower_ascii(haystack[haystack_index]) == lower_ascii(needle[needle_index]))
                {
                    ++haystack_index;
                    ++needle_index;
                }
                if (needle[needle_index] == '\0')
                    return true;
            }
            return false;
        };
        if (!configs_initialized)
        {
            sprintf_s(configs[0].name, IM_ARRAYSIZE(configs[0].name), "%s", "Default");
            format_config_time(configs[0].created_at, IM_ARRAYSIZE(configs[0].created_at));
            configs[0].fade = 1.0f;
            configs[0].deleting = false;
            config_count = 1;
            configs_initialized = true;
        }
        if (!sidebar_anim_initialized)
        {
            subtab_fade[0][0] = 1.0f;
            sidebar_icon_fade[0] = 1.0f;
            sidebar_bar_fade[0] = 1.0f;
            sidebar_anim_initialized = true;
        }
        const float topbar_anim_step = io.DeltaTime * 9.0f > 1.0f ? 1.0f : io.DeltaTime * 9.0f;
        const float subtabbar_anim_step = io.DeltaTime * 6.8f > 1.0f ? 1.0f : io.DeltaTime * 6.8f;
        const float subtab_content_anim_step = io.DeltaTime * 8.0f > 1.0f ? 1.0f : io.DeltaTime * 8.0f;
        const auto smooth_fade = [](float value)
        {
            if (value < 0.0f)
                value = 0.0f;
            if (value > 1.0f)
                value = 1.0f;
            return value * value * (3.0f - 2.0f * value);
        };
        if (active_sidebar_tab != displayed_top_tab)
        {
            topbar_visibility += (0.0f - topbar_visibility) * topbar_anim_step;
            subtabbar_visibility += (0.0f - subtabbar_visibility) * subtabbar_anim_step;
            if (topbar_visibility <= 0.45f)
            {
                displayed_top_tab = active_sidebar_tab;
                topbar_visibility = 0.0f;
                subtabbar_visibility = 0.0f;
                displayed_sub_tab[displayed_top_tab] = active_sub_tab[displayed_top_tab];
                subtab_content_visibility = 0.0f;
                for (int subtab_index = 0; subtab_index < max_subtabs; ++subtab_index)
                    subtab_hover_fade[displayed_top_tab][subtab_index] = 0.0f;
            }
        }
        else
        {
            topbar_visibility += (1.0f - topbar_visibility) * topbar_anim_step;
            subtabbar_visibility += (1.0f - subtabbar_visibility) * subtabbar_anim_step;
            if (subtab_counts[displayed_top_tab] > 0 && active_sub_tab[displayed_top_tab] != displayed_sub_tab[displayed_top_tab])
            {
                subtab_content_visibility += (0.0f - subtab_content_visibility) * subtab_content_anim_step;
                if (subtab_content_visibility <= 0.45f)
                {
                    displayed_sub_tab[displayed_top_tab] = active_sub_tab[displayed_top_tab];
                    subtab_content_visibility = 0.0f;
                }
            }
            else
            {
                subtab_content_visibility += (1.0f - subtab_content_visibility) * subtab_content_anim_step;
            }
        }
        const float topbar_alpha = smooth_fade(topbar_visibility);
        const float subtabbar_alpha = smooth_fade(subtabbar_visibility);
        const float subtab_content_alpha = subtab_counts[displayed_top_tab] > 0 ? smooth_fade(subtab_content_visibility) : 1.0f;
        const float content_alpha = topbar_alpha * subtab_content_alpha;
        const bool subtab_content_ready = subtab_counts[displayed_top_tab] == 0 || subtab_content_visibility > 0.82f;
        const bool modal_active =
            create_overlay_open || create_overlay_fade > 0.02f ||
            quick_presets_overlay_open || quick_presets_overlay_fade > 0.02f ||
            object_add_overlay_open || object_add_overlay_fade > 0.02f;
        const bool content_interactive = !modal_active && active_sidebar_tab == displayed_top_tab && topbar_visibility > 0.82f && subtab_content_ready;
        const int render_sub_tab = displayed_sub_tab[displayed_top_tab];
        bool combat_dropdown_click_blocked = false;
        const auto close_combat_dropdowns_except = [&](int keep_id)
        {
            for (int dropdown_index = 0; dropdown_index < IM_ARRAYSIZE(combat_dropdown_open); ++dropdown_index)
            {
                if (dropdown_index != keep_id)
                    combat_dropdown_open[dropdown_index] = false;
            }
            teleport_location_dropdown_open = false;
            macro_tuning_mode_dropdown_open = false;
            macro_preset_dropdown_open = false;
            weapon_attachment_dropdown_open = false;
            weapon_spawner_category_dropdown_open = false;
            weapon_spawner_weapon_dropdown_open = false;
        };
        const auto build_multi_combo_label = [](const char* const* items, int item_count, unsigned int mask, char* output, int output_size)
        {
            if (mask == 0u)
            {
                sprintf_s(output, output_size, "%s", "None");
                return;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            output[0] = '\0';
            for (int item_index = 0; item_index < item_count; ++item_index)
            {
                if ((mask & (1u << item_index)) == 0u)
                    continue;
                if (output[0] != '\0')
                    strcat_s(output, output_size, ", ");
                strcat_s(output, output_size, items[item_index]);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const int max_visible_chars = 15;
            if (static_cast<int>(strlen(output)) > max_visible_chars)
            {
                output[max_visible_chars] = '\0';
                strcat_s(output, output_size, "...");
            }
        };
        const auto draw_combat_dropdown = [&](
            int dropdown_id,
            const char* const* items,
            int item_count,
            const ImVec2& combo_min,
            bool multi_select,
            int single_slot,
            int multi_slot,
            bool enabled,
            bool use_clip,
            const ImVec2& clip_min,
            const ImVec2& clip_max) -> bool
        {
            if (dropdown_id < 0 || dropdown_id >= IM_ARRAYSIZE(combat_dropdown_open) || item_count <= 0)
                return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            int dropdown_group_min = 0;
            int dropdown_group_max = 2;
            if (dropdown_id >= 3 && dropdown_id <= 5)
            {
                dropdown_group_min = 3;
                dropdown_group_max = 5;
            }
            else if (dropdown_id >= 6 && dropdown_id <= 8)
            {
                dropdown_group_min = 6;
                dropdown_group_max = 8;
            }
            else if (dropdown_id == 9)
            {
                dropdown_group_min = 9;
                dropdown_group_max = 9;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            bool other_dropdown_active = false;
            for (int active_dropdown_index = dropdown_group_min; active_dropdown_index <= dropdown_group_max; ++active_dropdown_index)
            {
                if (active_dropdown_index != dropdown_id &&
                    (combat_dropdown_open[active_dropdown_index] || combat_dropdown_fade[active_dropdown_index] > 0.005f))
                {
                    other_dropdown_active = true;
                    break;
                }
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float dropdown_step = io.DeltaTime * 3.8f;
            combat_dropdown_fade[dropdown_id] += combat_dropdown_open[dropdown_id] ? dropdown_step : -dropdown_step;
            if (combat_dropdown_fade[dropdown_id] < 0.0f)
                combat_dropdown_fade[dropdown_id] = 0.0f;
            if (combat_dropdown_fade[dropdown_id] > 1.0f)
                combat_dropdown_fade[dropdown_id] = 1.0f;
            if (!multi_select && single_slot >= 0 && single_slot < IM_ARRAYSIZE(combat_single_selected) && combat_dropdown_fade[dropdown_id] == 0.0f && combat_single_pending[single_slot] >= 0)
            {
                combat_single_selected[single_slot] = combat_single_pending[single_slot];
                combat_single_pending[single_slot] = -1;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            char value_text[64];
            if (multi_select)
                build_multi_combo_label(items, item_count, combat_multi_mask[multi_slot], value_text, IM_ARRAYSIZE(value_text));
            else
                sprintf_s(value_text, IM_ARRAYSIZE(value_text), "%s", items[combat_single_selected[single_slot]]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 combo_size(265.0f, 29.0f);
            const ImVec2 combo_max(combo_min.x + combo_size.x, combo_min.y + combo_size.y);
            const bool combo_hovered =
                io.MousePos.x >= combo_min.x && io.MousePos.x <= combo_max.x &&
                io.MousePos.y >= combo_min.y && io.MousePos.y <= combo_max.y &&
                (!use_clip || (combo_max.y >= clip_min.y && combo_min.y <= clip_max.y));
            sigma::combobox(value_text, combo_min, combo_size, content_alpha, false);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float item_height = 28.0f;
            const float dropdown_height = item_count > 6 ? 190.0f : 14.0f + item_count * item_height;
            const ImVec2 dropdown_size(265.0f, dropdown_height);
            const bool loose_dropdown_hitbox = dropdown_id == 9 && !use_clip;
            const float dropdown_bottom_limit = use_clip ? clip_max.y : 10000.0f;
            const bool open_up = combo_max.y + 7.0f + dropdown_size.y > dropdown_bottom_limit;
            const ImVec2 dropdown_min(
                combo_min.x,
                open_up ? combo_min.y - dropdown_size.y - 7.0f : combo_max.y + 7.0f);
            const ImVec2 dropdown_max(dropdown_min.x + dropdown_size.x, dropdown_min.y + dropdown_size.y);
            const float pre_dropdown_eased = smooth_fade(combat_dropdown_fade[dropdown_id]);
            const float pre_dropdown_slide_offset = loose_dropdown_hitbox ? 0.0f : (1.0f - pre_dropdown_eased) * -14.0f;
            const ImVec2 hit_dropdown_min(dropdown_min.x, dropdown_min.y + pre_dropdown_slide_offset);
            const ImVec2 hit_dropdown_max(
                dropdown_max.x,
                hit_dropdown_min.y + dropdown_size.y * (loose_dropdown_hitbox && combat_dropdown_open[dropdown_id] ? 1.0f : pre_dropdown_eased));
            const bool dropdown_hovered =
                io.MousePos.x >= hit_dropdown_min.x && io.MousePos.x <= hit_dropdown_max.x &&
                io.MousePos.y >= hit_dropdown_min.y && io.MousePos.y <= hit_dropdown_max.y &&
                (!use_clip || (io.MousePos.y >= clip_min.y && io.MousePos.y <= clip_max.y));
            const bool active = combat_dropdown_open[dropdown_id] || combat_dropdown_fade[dropdown_id] > 0.005f;
            if (active && dropdown_hovered)
                combat_dropdown_click_blocked = true;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            if (enabled && !other_dropdown_active && !combat_dropdown_click_blocked && combo_hovered && io.MouseClicked[0])
            {
                combat_dropdown_open[dropdown_id] = !combat_dropdown_open[dropdown_id];
                close_combat_dropdowns_except(dropdown_id);
            }
            if (enabled && combat_dropdown_open[dropdown_id] && io.MouseClicked[0] && !dropdown_hovered && !combo_hovered)
                combat_dropdown_open[dropdown_id] = false;
            if (enabled && !combat_dropdown_click_blocked && combo_hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float dropdown_eased = smooth_fade(combat_dropdown_fade[dropdown_id]);
            if (dropdown_eased > 0.005f)
            {
                ImDrawList* combo_draw_list = ImGui::GetForegroundDrawList();
                const float dropdown_slide_offset = (1.0f - dropdown_eased) * -14.0f;
                const ImVec2 dropdown_draw_min(dropdown_min.x, dropdown_min.y + dropdown_slide_offset);
                const ImVec2 dropdown_draw_max(dropdown_max.x, dropdown_max.y + dropdown_slide_offset);
                const float visible_height = dropdown_size.y * dropdown_eased;
                const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * dropdown_eased);
                const float content_height = 14.0f + item_count * item_height;
                const float max_scroll = content_height > dropdown_size.y ? content_height - dropdown_size.y : 0.0f;
                if (enabled && combat_dropdown_open[dropdown_id] && dropdown_hovered && io.MouseWheel != 0.0f)
                    combat_dropdown_target_scroll[dropdown_id] -= io.MouseWheel * 35.0f;
                if (combat_dropdown_target_scroll[dropdown_id] < 0.0f)
                    combat_dropdown_target_scroll[dropdown_id] = 0.0f;
                if (combat_dropdown_target_scroll[dropdown_id] > max_scroll)
                    combat_dropdown_target_scroll[dropdown_id] = max_scroll;
                combat_dropdown_scroll[dropdown_id] += (combat_dropdown_target_scroll[dropdown_id] - combat_dropdown_scroll[dropdown_id]) * dropdown_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                int clicked_item_index = -1;
                if (enabled && combat_dropdown_open[dropdown_id] && dropdown_hovered && io.MouseClicked[0])
                {
                    const float clicked_local_y = io.MousePos.y - (loose_dropdown_hitbox ? dropdown_min.y : dropdown_draw_min.y) - 7.0f + combat_dropdown_scroll[dropdown_id];
                    const int candidate_index = static_cast<int>(clicked_local_y / item_height);
                    if (clicked_local_y >= 0.0f && candidate_index >= 0 && candidate_index < item_count)
                    {
                        clicked_item_index = candidate_index;
                        combat_dropdown_click_blocked = true;
                    }
                }
                if (loose_dropdown_hitbox && enabled && combat_dropdown_open[dropdown_id])
                {
                    char dropdown_input_id[64];
                    ImFormatString(dropdown_input_id, IM_ARRAYSIZE(dropdown_input_id), "##combat_dropdown_input_%d", dropdown_id);
                    ImGui::SetNextWindowPos(dropdown_min);
                    ImGui::SetNextWindowSize(dropdown_size);
                    ImGui::Begin(
                        dropdown_input_id,
                        nullptr,
                        ImGuiWindowFlags_NoDecoration |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoSavedSettings |
                        ImGuiWindowFlags_NoBackground |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);
                    for (int input_index = 0; input_index < item_count; ++input_index)
                    {
                        const float input_y = 7.0f + input_index * item_height - combat_dropdown_scroll[dropdown_id];
                        ImGui::SetCursorPos(ImVec2(7.0f, input_y));
                        char item_input_id[64];
                        ImFormatString(item_input_id, IM_ARRAYSIZE(item_input_id), "##combat_dropdown_item_%d", input_index);
                        ImGui::InvisibleButton(item_input_id, ImVec2(dropdown_size.x - 14.0f, item_height));
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            combat_dropdown_click_blocked = true;
                        }
                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                        {
                            clicked_item_index = input_index;
                            combat_dropdown_click_blocked = true;
                        }
                    }
                    ImGui::End();
                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                combo_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                combo_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                combo_draw_list->AddRect(
                    ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                    ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                    IM_COL32(15, 15, 15, dropdown_alpha),
                    4.0f,
                    0,
                    1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const ImVec2 draw_clip_min = use_clip
                    ? ImVec2(dropdown_draw_min.x > clip_min.x ? dropdown_draw_min.x : clip_min.x, dropdown_draw_min.y > clip_min.y ? dropdown_draw_min.y : clip_min.y)
                    : dropdown_draw_min;
                const ImVec2 draw_clip_max = use_clip
                    ? ImVec2(visible_dropdown_max.x < clip_max.x ? visible_dropdown_max.x : clip_max.x, visible_dropdown_max.y < clip_max.y ? visible_dropdown_max.y : clip_max.y)
                    : visible_dropdown_max;
                combo_draw_list->PushClipRect(draw_clip_min, draw_clip_max, true);
                for (int item_index = 0; item_index < item_count; ++item_index)
                {
                    const float item_y = dropdown_draw_min.y + 7.0f + item_index * item_height - combat_dropdown_scroll[dropdown_id];
                    const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                    const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + item_height);
                    const bool item_visible = item_max.y >= draw_clip_min.y && item_min.y <= draw_clip_max.y;
                    const bool item_hovered =
                        item_visible &&
                        io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                        io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                        io.MousePos.y >= draw_clip_min.y && io.MousePos.y <= draw_clip_max.y;
                    const bool item_selected = multi_select
                        ? ((combat_multi_mask[multi_slot] & (1u << item_index)) != 0u)
                        : (item_index == combat_single_selected[single_slot]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (clicked_item_index == item_index)
                    {
                        if (multi_select)
                        {
                            const bool will_select = (combat_multi_mask[multi_slot] & (1u << item_index)) == 0u;
                            combat_multi_mask[multi_slot] ^= (1u << item_index);
                            if (will_select)
                                combat_dropdown_select_fade[dropdown_id][item_index] = 0.0f;
                        }
                        else
                        {
                            if (combat_single_selected[single_slot] != item_index)
                                combat_dropdown_select_fade[dropdown_id][item_index] = 0.0f;
                            combat_single_selected[single_slot] = item_index;
                            combat_single_pending[single_slot] = -1;
                            combat_dropdown_open[dropdown_id] = false;
                        }
                    }
                    if (enabled && item_hovered)
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (!item_visible)
                        continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                    combat_dropdown_text_fade[dropdown_id][item_index] += (item_target - combat_dropdown_text_fade[dropdown_id][item_index]) * dropdown_step;
                    const float item_fade = smooth_fade(combat_dropdown_text_fade[dropdown_id][item_index]);
                    combat_dropdown_select_fade[dropdown_id][item_index] += ((item_selected ? 1.0f : 0.0f) - combat_dropdown_select_fade[dropdown_id][item_index]) * dropdown_step;
                    const float selected_fade = smooth_fade(combat_dropdown_select_fade[dropdown_id][item_index]);
                    if (selected_fade > 0.01f)
                    {
                        const int row_alpha = static_cast<int>(dropdown_alpha * selected_fade);
                        const float selection_x = item_min.x - 18.0f + 18.0f * selected_fade;
                        combo_draw_list->AddRectFilled(ImVec2(selection_x, item_min.y), item_max, IM_COL32(31, 31, 31, row_alpha), 2.0f);
                    }
                    else if (item_hovered && item_fade > 0.01f)
                    {
                        const int row_alpha = static_cast<int>(dropdown_alpha * item_fade);
                        combo_draw_list->AddRectFilled(item_min, item_max, IM_COL32(28, 28, 28, row_alpha), 2.0f);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                    float item_text_x = item_min.x + 11.0f + 22.0f * selected_fade;
                    const int selected_icon_alpha = static_cast<int>(dropdown_alpha * selected_fade);
                    if (selected_icon_alpha > 0)
                    {
                        combo_draw_list->AddText(
                            icon_font,
                            12.0f,
                            ImVec2(item_min.x + 11.0f, item_min.y + 7.0f),
                            IM_COL32(142, 134, 246, selected_icon_alpha),
                            u8"\uf313");
                    }
                    combo_draw_list->AddText(
                        medium_font,
                        13.0f,
                        ImVec2(item_text_x, item_min.y + 7.0f),
                        IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                        items[item_index]);
                }
                combo_draw_list->PopClipRect();
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            return active;
        };
        const float top_tab_second_bar_x = top_tab_text_x + title_font->CalcTextSizeA(top_tab_text_size, 1000.0f, 0.0f, main_tab_labels[displayed_top_tab]).x + 12.0f;
        const float subtab_start_x = top_tab_second_bar_x + 23.0f;
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(top_tab_second_bar_x, 4.0f + 24.0f),
            ImVec2(top_tab_second_bar_x + 1.0f, 4.0f + 24.0f + 8.0f),
            IM_COL32(16, 16, 16, static_cast<int>(255.0f * topbar_alpha)));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        for (int icon_index = 0; icon_index < IM_ARRAYSIZE(sidebar_icons); ++icon_index)
        {
            const ImVec2 tab_hit_min(12.0f, sidebar_icon_start_y + sidebar_icon_step_y * icon_index - 15.0f);
            const ImVec2 tab_hit_max(tab_hit_min.x + 48.0f, tab_hit_min.y + 30.0f);
            const bool tab_hovered = !modal_active && mouse_in_rect(io.MousePos, tab_hit_min, tab_hit_max);
            if (tab_hovered)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (io.MouseClicked[0])
                {
                    active_sidebar_tab = icon_index;
                    for (int dropdown_index = 0; dropdown_index < IM_ARRAYSIZE(combat_dropdown_open); ++dropdown_index)
                        combat_dropdown_open[dropdown_index] = false;
                    teleport_location_dropdown_open = false;
                    macro_tuning_mode_dropdown_open = false;
                    macro_preset_dropdown_open = false;
                    weapon_attachment_dropdown_open = false;
                    weapon_spawner_category_dropdown_open = false;
                    weapon_spawner_weapon_dropdown_open = false;
                }
            }
            ImFont* current_icon_font = (icon_index == 2 && g_EyeIconFont) ? g_EyeIconFont : icon_font;
            sigma::tab(
                main_tab_labels[icon_index],
                sidebar_icons[icon_index],
                icon_index,
                active_sidebar_tab,
                ImVec2(12.0f, sidebar_icon_start_y + sidebar_icon_step_y * icon_index - 15.0f),
                ImVec2(sidebar_icon_center_x, sidebar_icon_start_y + sidebar_icon_step_y * icon_index + (icon_index == 2 ? 2.0f : 0.0f)),
                ImVec2(accent_bar_x, accent_bar_y + sidebar_icon_step_y * icon_index),
                ImVec2(58.0f, sidebar_icon_start_y + sidebar_icon_step_y * icon_index - 4.0f),
                sidebar_icon_fade[icon_index],
                sidebar_bar_fade[icon_index],
                sidebar_hover_fade[icon_index],
                current_icon_font);
            const float tooltip_step = io.DeltaTime * 7.0f > 1.0f ? 1.0f : io.DeltaTime * 7.0f;
            sidebar_tooltip_fade[icon_index] += ((tab_hovered ? 1.0f : 0.0f) - sidebar_tooltip_fade[icon_index]) * tooltip_step;
            const float tooltip_fade = smooth_fade(sidebar_tooltip_fade[icon_index]);
            if (tooltip_fade > 0.005f)
            {
                const ImVec2 tooltip_min(58.0f, sidebar_icon_start_y + sidebar_icon_step_y * icon_index - 4.0f);
                const float tooltip_text_size = 13.0f;
                const ImVec2 label_size = medium_font->CalcTextSizeA(tooltip_text_size, 1000.0f, 0.0f, main_tab_labels[icon_index]);
                const float content_width = label_size.x + 18.0f;
                const float tooltip_width = icon_index == 0 ? content_width : (content_width > 55.0f ? content_width : 55.0f);
                const float tooltip_height = 25.0f;
                const int tooltip_alpha = static_cast<int>(255.0f * tooltip_fade);
                ImDrawList* tooltip_draw_list = ImGui::GetForegroundDrawList();
                tooltip_draw_list->AddRectFilled(
                    tooltip_min,
                    ImVec2(tooltip_min.x + tooltip_width, tooltip_min.y + tooltip_height),
                    IM_COL32(21, 21, 21, tooltip_alpha),
                    3.0f);
                tooltip_draw_list->AddText(
                    medium_font,
                    tooltip_text_size,
                    ImVec2(
                        tooltip_min.x + (tooltip_width - label_size.x) * 0.5f,
                        tooltip_min.y + (tooltip_height - label_size.y) * 0.5f),
                    IM_COL32(180, 180, 184, tooltip_alpha),
                    main_tab_labels[icon_index]);
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (topbar_alpha > 0.005f)
        {
            ImFont* current_top_icon_font = (displayed_top_tab == 2 && g_EyeIconFont) ? g_EyeIconFont : icon_font;
            const float top_icon_y = displayed_top_tab == 2 ? top_tab_icon_y + 2.0f : top_tab_icon_y;
            const int tab_alpha = static_cast<int>(255.0f * topbar_alpha);
            ImGui::GetBackgroundDrawList()->AddText(
                current_top_icon_font,
                top_tab_icon_size,
                ImVec2(top_tab_icon_x, top_icon_y),
                IM_COL32(142, 134, 246, tab_alpha),
                sidebar_icons[displayed_top_tab]);
            ImGui::GetBackgroundDrawList()->AddText(
                title_font,
                top_tab_text_size,
                ImVec2(top_tab_text_x, top_tab_text_y),
                IM_COL32(235, 235, 235, tab_alpha),
                main_tab_labels[displayed_top_tab]);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (subtabbar_alpha > 0.005f && subtab_counts[displayed_top_tab] > 0)
        {
            float next_subtab_x = subtab_start_x;
            for (int subtab_index = 0; subtab_index < subtab_counts[displayed_top_tab]; ++subtab_index)
            {
                const float subtab_width = sigma::subtab_width(subtab_icons[displayed_top_tab][subtab_index], subtab_labels[displayed_top_tab][subtab_index]);
                const ImVec2 subtab_hit_min(next_subtab_x - 4.0f, 16.0f);
                const ImVec2 subtab_hit_max(subtab_hit_min.x + subtab_width, subtab_hit_min.y + 30.0f);
                if (!modal_active && subtabbar_visibility > 0.75f && mouse_in_rect(io.MousePos, subtab_hit_min, subtab_hit_max))
                {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (io.MouseClicked[0])
                    {
                        active_sub_tab[displayed_top_tab] = subtab_index;
                        for (int dropdown_index = 0; dropdown_index < IM_ARRAYSIZE(combat_dropdown_open); ++dropdown_index)
                            combat_dropdown_open[dropdown_index] = false;
                        teleport_location_dropdown_open = false;
                        macro_tuning_mode_dropdown_open = false;
                        macro_preset_dropdown_open = false;
                        weapon_attachment_dropdown_open = false;
                        weapon_spawner_category_dropdown_open = false;
                        weapon_spawner_weapon_dropdown_open = false;
                    }
                }
                sigma::subtab(
                    subtab_index,
                    active_sub_tab[displayed_top_tab],
                    subtab_icons[displayed_top_tab][subtab_index],
                    subtab_labels[displayed_top_tab][subtab_index],
                    ImVec2(next_subtab_x, 22.0f),
                    subtabbar_alpha,
                    subtab_fade[displayed_top_tab][subtab_index],
                    subtab_hover_fade[displayed_top_tab][subtab_index],
                    subtabbar_visibility > 0.75f);
                next_subtab_x += subtab_width + 8.0f;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float profile_size = 24.0f;
        const ImVec2 profile_min(
            sidebar_icon_center_x - profile_size * 0.5f,
            476.0f);
        const ImVec2 profile_max(
            profile_min.x + profile_size,
            profile_min.y + profile_size);
        if (g_ProfileTexture)
        {
            draw_list->AddImageRounded(
                (ImTextureID)g_ProfileTexture,
                profile_min,
                profile_max,
                ImVec2(0.0f, 0.0f),
                ImVec2(1.0f, 1.0f),
                IM_COL32_WHITE,
                profile_size * 0.5f);
        }
        const char* bottom_icon = u8"\uf3dc";
        const ImVec2 bottom_icon_center(sidebar_icon_center_x, 522.0f);
        const ImVec2 bottom_icon_hit_size(36.0f, 36.0f);
        const ImVec2 bottom_icon_hit_min(
            bottom_icon_center.x - bottom_icon_hit_size.x * 0.5f,
            bottom_icon_center.y - bottom_icon_hit_size.y * 0.5f);
        bool bottom_icon_hovered = false;
        if (!modal_active)
        {
            const ImVec2 bottom_icon_hit_max(
                bottom_icon_hit_min.x + bottom_icon_hit_size.x,
                bottom_icon_hit_min.y + bottom_icon_hit_size.y);
            bottom_icon_hovered = mouse_in_rect(io.MousePos, bottom_icon_hit_min, bottom_icon_hit_max);
            if (bottom_icon_hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (bottom_icon_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                bottom_popup_open = !bottom_popup_open;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 bottom_popup_size(208.0f, 222.0f);
        const ImVec2 bottom_popup_min(42.0f, bottom_icon_center.y + bottom_icon_hit_size.y * 0.5f);
        const ImVec2 bottom_popup_max(bottom_popup_min.x + bottom_popup_size.x, bottom_popup_min.y + bottom_popup_size.y);
        const bool bottom_popup_hovered =
            io.MousePos.x >= bottom_popup_min.x && io.MousePos.x <= bottom_popup_max.x &&
            io.MousePos.y >= bottom_popup_min.y && io.MousePos.y <= bottom_popup_max.y;
        if (bottom_popup_open && io.MouseClicked[0] && !bottom_icon_hovered && !bottom_popup_hovered)
            bottom_popup_open = false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float bottom_popup_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
        bottom_popup_fade += ((bottom_popup_open ? 1.0f : 0.0f) - bottom_popup_fade) * bottom_popup_step;
        if (bottom_popup_fade < 0.01f)
            bottom_popup_fade = 0.0f;
        if (bottom_popup_fade > 0.99f)
            bottom_popup_fade = 1.0f;
        const bool bottom_popup_region_needed = bottom_popup_fade > 0.005f;
        if (g_BottomProfilePopupRegionActive != bottom_popup_region_needed)
        {
            g_BottomProfilePopupRegionActive = bottom_popup_region_needed;
            const bool bottom_region_needed = g_BottomProfilePopupRegionActive || g_BottomOptionPopupRegionActive;
            if (g_BottomPopupRegionActive != bottom_region_needed)
            {
                g_BottomPopupRegionActive = bottom_region_needed;
                UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
            }
        }
        const float bottom_popup_eased = smooth_fade(bottom_popup_fade);
        bottom_icon_hover_fade += (((bottom_icon_hovered || bottom_popup_open) ? 1.0f : 0.0f) - bottom_icon_hover_fade) * bottom_popup_step;
        const float bottom_icon_eased = smooth_fade(bottom_icon_hover_fade);
        const int bottom_icon_value = static_cast<int>(86.0f + (255.0f - 86.0f) * bottom_icon_eased);
        const ImVec2 bottom_icon_size = icon_font->CalcTextSizeA(icon_font_size, 1000.0f, 0.0f, bottom_icon);
        draw_list->AddText(
            icon_font,
            icon_font_size,
            ImVec2(sidebar_icon_center_x - bottom_icon_size.x * 0.5f, 514.0f),
            IM_COL32(bottom_icon_value, bottom_icon_value, bottom_icon_value, 255),
            bottom_icon);
        if (bottom_popup_eased > 0.005f)
        {
            const float slide_offset = (1.0f - bottom_popup_eased) * -10.0f;
            const ImVec2 popup_draw_min(bottom_popup_min.x, bottom_popup_min.y + slide_offset);
            const ImVec2 popup_draw_max(bottom_popup_max.x, bottom_popup_max.y + slide_offset);
            const int popup_alpha = static_cast<int>(255.0f * bottom_popup_eased);
            ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
            popup_draw_list->AddRectFilled(
                popup_draw_min,
                popup_draw_max,
                IM_COL32(12, 12, 12, popup_alpha),
                3.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float dpi_scale_values[] = { 0.5f, 1.0f, 1.5f, 2.0f };
            int dpi_scale_index = 0;
            float dpi_scale_distance = std::fabs(g_UiTargetScale - dpi_scale_values[0]);
            for (int scale_index = 1; scale_index < IM_ARRAYSIZE(dpi_scale_values); ++scale_index)
            {
                const float distance = std::fabs(g_UiTargetScale - dpi_scale_values[scale_index]);
                if (distance < dpi_scale_distance)
                {
                    dpi_scale_distance = distance;
                    dpi_scale_index = scale_index;
                }
            }
            if (!bottom_popup_dpi_dragging)
                bottom_popup_pending_dpi_index = dpi_scale_index;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 dpi_label_pos(popup_draw_min.x + 24.0f, popup_draw_min.y + 18.0f);
            const int dpi_text_alpha = static_cast<int>(235.0f * bottom_popup_eased);
            popup_draw_list->AddText(
                title_font,
                13.0f,
                dpi_label_pos,
                IM_COL32(205, 205, 210, dpi_text_alpha),
                "DPI Scale");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            char dpi_value_text[16];
            const int dpi_display_index = bottom_popup_dpi_dragging ? bottom_popup_pending_dpi_index : dpi_scale_index;
            ImFormatString(dpi_value_text, IM_ARRAYSIZE(dpi_value_text), "%.0f%%", dpi_scale_values[dpi_display_index] * 100.0f);
            const ImVec2 dpi_value_size = title_font->CalcTextSizeA(13.0f, 1000.0f, 0.0f, dpi_value_text);
            popup_draw_list->AddText(
                title_font,
                13.0f,
                ImVec2(popup_draw_max.x - 24.0f - dpi_value_size.x, popup_draw_min.y + 18.0f),
                IM_COL32(142, 142, 148, dpi_text_alpha),
                dpi_value_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 dpi_track_min(popup_draw_min.x + 24.0f, popup_draw_min.y + 49.0f);
            const ImVec2 dpi_track_max(popup_draw_max.x - 24.0f, dpi_track_min.y + 5.0f);
            const bool dpi_track_hovered =
                io.MousePos.x >= dpi_track_min.x - 6.0f && io.MousePos.x <= dpi_track_max.x + 6.0f &&
                io.MousePos.y >= dpi_track_min.y - 9.0f && io.MousePos.y <= dpi_track_max.y + 9.0f;
            if (bottom_popup_open && dpi_track_hovered && io.MouseClicked[0])
                bottom_popup_dpi_dragging = true;
            const bool dpi_mouse_down = ImGui::IsMouseDown(ImGuiMouseButton_Left);
            if (bottom_popup_open && bottom_popup_dpi_dragging)
            {
                float normalized = (io.MousePos.x - dpi_track_min.x) / (dpi_track_max.x - dpi_track_min.x);
                if (normalized < 0.0f)
                    normalized = 0.0f;
                if (normalized > 1.0f)
                    normalized = 1.0f;
                int snapped_index = static_cast<int>(normalized * 3.0f + 0.5f);
                if (snapped_index < 0)
                    snapped_index = 0;
                if (snapped_index > 3)
                    snapped_index = 3;
                bottom_popup_pending_dpi_index = snapped_index;
                dpi_scale_index = snapped_index;
            }
            if (bottom_popup_dpi_dragging && !dpi_mouse_down)
            {
                bottom_popup_dpi_dragging = false;
                g_UiTargetScale = dpi_scale_values[bottom_popup_pending_dpi_index];
                dpi_scale_index = bottom_popup_pending_dpi_index;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            bottom_popup_dpi_hover_fade += (((dpi_track_hovered || bottom_popup_dpi_dragging) ? 1.0f : 0.0f) - bottom_popup_dpi_hover_fade) * bottom_popup_step;
            const float dpi_hover_eased = smooth_fade(bottom_popup_dpi_hover_fade);
            const float dpi_fill_t = static_cast<float>(bottom_popup_dpi_dragging ? bottom_popup_pending_dpi_index : dpi_scale_index) / 3.0f;
            const float dpi_fill_x = dpi_track_min.x + (dpi_track_max.x - dpi_track_min.x) * dpi_fill_t;
            popup_draw_list->AddRectFilled(
                dpi_track_min,
                dpi_track_max,
                IM_COL32(31, 31, 31, static_cast<int>(255.0f * bottom_popup_eased)),
                2.0f);
            popup_draw_list->AddRectFilled(
                dpi_track_min,
                ImVec2(dpi_fill_x, dpi_track_max.y),
                IM_COL32(142, 134, 246, static_cast<int>(255.0f * bottom_popup_eased)),
                2.0f);
            const float dpi_knob_radius = 7.0f + 2.0f * dpi_hover_eased;
            popup_draw_list->AddCircleFilled(
                ImVec2(dpi_fill_x, (dpi_track_min.y + dpi_track_max.y) * 0.5f),
                dpi_knob_radius,
                IM_COL32(56, 56, 60, static_cast<int>(255.0f * bottom_popup_eased)),
                24);
            popup_draw_list->AddCircle(
                ImVec2(dpi_fill_x, (dpi_track_min.y + dpi_track_max.y) * 0.5f),
                dpi_knob_radius,
                IM_COL32(72, 72, 78, static_cast<int>(255.0f * bottom_popup_eased)),
                24,
                1.0f);
            const int active_dpi_scale_index = bottom_popup_dpi_dragging ? bottom_popup_pending_dpi_index : dpi_scale_index;
            for (int tick_index = 0; tick_index < IM_ARRAYSIZE(dpi_scale_values); ++tick_index)
            {
                const float tick_x = dpi_track_min.x + (dpi_track_max.x - dpi_track_min.x) * (static_cast<float>(tick_index) / 3.0f);
                popup_draw_list->AddCircleFilled(
                    ImVec2(tick_x, (dpi_track_min.y + dpi_track_max.y) * 0.5f),
                    tick_index == active_dpi_scale_index ? 2.5f : 1.6f,
                    IM_COL32(205, 205, 210, static_cast<int>((tick_index == active_dpi_scale_index ? 170.0f : 80.0f) * bottom_popup_eased)),
                    12);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 popup_control_min(popup_draw_min.x + 15.0f, popup_draw_min.y + 78.0f);
            const ImVec2 popup_control_size(178.0f, 35.0f);
            const ImVec2 popup_control_max(popup_control_min.x + popup_control_size.x, popup_control_min.y + popup_control_size.y);
            popup_draw_list->AddRectFilled(
                popup_control_min,
                popup_control_max,
                IM_COL32(24, 24, 24, popup_alpha),
                2.0f);
            popup_draw_list->AddRect(
                popup_control_min,
                popup_control_max,
                IM_COL32(38, 38, 38, popup_alpha),
                2.0f,
                0,
                1.0f);
            const float segment_width = popup_control_size.x * 0.5f;
            bottom_popup_theme_switch_t += (static_cast<float>(bottom_popup_theme_index) - bottom_popup_theme_switch_t) * bottom_popup_step;
            if (std::fabs(bottom_popup_theme_switch_t - static_cast<float>(bottom_popup_theme_index)) < 0.001f)
                bottom_popup_theme_switch_t = static_cast<float>(bottom_popup_theme_index);
            const float selector_x = popup_control_min.x + 2.0f + segment_width * bottom_popup_theme_switch_t;
            popup_draw_list->AddRectFilled(
                ImVec2(selector_x, popup_control_min.y + 2.0f),
                ImVec2(selector_x + segment_width - 4.0f, popup_control_max.y - 2.0f),
                IM_COL32(31, 31, 31, popup_alpha),
                1.0f);
            const char* theme_labels[] = { "Dark", "Light" };
            for (int theme_index = 0; theme_index < 2; ++theme_index)
            {
                const ImVec2 segment_min(popup_control_min.x + segment_width * theme_index, popup_control_min.y);
                const ImVec2 segment_max(segment_min.x + segment_width, popup_control_max.y);
                const bool segment_hovered =
                    io.MousePos.x >= segment_min.x && io.MousePos.x <= segment_max.x &&
                    io.MousePos.y >= segment_min.y && io.MousePos.y <= segment_max.y;
                bottom_popup_theme_hover_fade[theme_index] += (((segment_hovered || bottom_popup_theme_index == theme_index) ? 1.0f : 0.0f) - bottom_popup_theme_hover_fade[theme_index]) * bottom_popup_step;
                const float segment_hover_eased = smooth_fade(bottom_popup_theme_hover_fade[theme_index]);
                if (bottom_popup_open && segment_hovered && io.MouseClicked[0])
                    bottom_popup_theme_index = theme_index;
                if (segment_hovered)
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                float segment_active_mix = 1.0f - std::fabs(bottom_popup_theme_switch_t - static_cast<float>(theme_index));
                if (segment_active_mix < 0.0f)
                    segment_active_mix = 0.0f;
                if (segment_active_mix > 1.0f)
                    segment_active_mix = 1.0f;
                const ImVec2 segment_text_size = title_font->CalcTextSizeA(13.0f, 1000.0f, 0.0f, theme_labels[theme_index]);
                const int segment_text_value = static_cast<int>(72.0f + 138.0f * segment_active_mix + 32.0f * segment_hover_eased);
                popup_draw_list->AddText(
                    title_font,
                    13.0f,
                    ImVec2(segment_min.x + (segment_width - segment_text_size.x) * 0.5f, segment_min.y + (popup_control_size.y - segment_text_size.y) * 0.5f - 1.0f),
                    IM_COL32(segment_text_value, segment_text_value, segment_text_value + 4 > 255 ? 255 : segment_text_value + 4, popup_alpha),
                    theme_labels[theme_index]);
            }
            popup_draw_list->AddLine(
                ImVec2(popup_control_min.x + segment_width, popup_control_min.y + 4.0f),
                ImVec2(popup_control_min.x + segment_width, popup_control_max.y - 4.0f),
                IM_COL32(16, 16, 16, static_cast<int>(180.0f * bottom_popup_eased)),
                1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            if (!bottom_popup_open)
            {
                bottom_popup_combo_open[0] = false;
                bottom_popup_combo_open[1] = false;
            }
            const char* const popup_combo_items[2][2] = {
                { "English", "Polish" },
                { g_PrimaryMonitorName, "Default" }
            };
            const int popup_combo_selected_indices[2] = {
                bottom_popup_language_index,
                bottom_popup_device_index
            };
            bool bottom_popup_combo_dropdown_hovered[2] = {};
            for (int hover_combo_index = 0; hover_combo_index < 2; ++hover_combo_index)
            {
                const ImVec2 combo_min(popup_draw_min.x + 15.0f, popup_draw_min.y + 126.0f + hover_combo_index * 38.0f);
                const ImVec2 combo_size(178.0f, 30.0f);
                const ImVec2 combo_max(combo_min.x + combo_size.x, combo_min.y + combo_size.y);
                const float dropdown_item_height = 23.0f;
                const ImVec2 dropdown_size(combo_size.x, 10.0f + dropdown_item_height * 2.0f);
                const ImVec2 dropdown_min(
                    combo_min.x,
                    hover_combo_index == 0 ? combo_max.y + 6.0f : combo_min.y - dropdown_size.y - 6.0f);
                const float dropdown_eased = smooth_fade(bottom_popup_combo_dropdown_fade[hover_combo_index]);
                const float dropdown_slide_offset = hover_combo_index == 0
                    ? (1.0f - dropdown_eased) * -7.0f
                    : (1.0f - dropdown_eased) * 7.0f;
                const ImVec2 dropdown_draw_min(dropdown_min.x, dropdown_min.y + dropdown_slide_offset);
                const ImVec2 dropdown_draw_max(dropdown_min.x + dropdown_size.x, dropdown_draw_min.y + dropdown_size.y);
                const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + dropdown_size.y * dropdown_eased);
                bottom_popup_combo_dropdown_hovered[hover_combo_index] =
                    io.MousePos.x >= dropdown_draw_min.x && io.MousePos.x <= dropdown_draw_max.x &&
                    io.MousePos.y >= dropdown_draw_min.y && io.MousePos.y <= visible_dropdown_max.y &&
                    bottom_popup_combo_dropdown_fade[hover_combo_index] > 0.005f;
            }
            const bool bottom_popup_any_dropdown_hovered =
                bottom_popup_combo_dropdown_hovered[0] || bottom_popup_combo_dropdown_hovered[1];
            for (int combo_index = 0; combo_index < 2; ++combo_index)
            {
                const ImVec2 combo_min(popup_draw_min.x + 15.0f, popup_draw_min.y + 126.0f + combo_index * 38.0f);
                const ImVec2 combo_size(178.0f, 30.0f);
                const ImVec2 combo_max(combo_min.x + combo_size.x, combo_min.y + combo_size.y);
                const float dropdown_item_height = 23.0f;
                const ImVec2 dropdown_size(combo_size.x, 10.0f + dropdown_item_height * 2.0f);
                const ImVec2 dropdown_min(
                    combo_min.x,
                    combo_index == 0 ? combo_max.y + 6.0f : combo_min.y - dropdown_size.y - 6.0f);
                const ImVec2 dropdown_max(dropdown_min.x + dropdown_size.x, dropdown_min.y + dropdown_size.y);
                bottom_popup_combo_dropdown_fade[combo_index] += ((bottom_popup_combo_open[combo_index] ? 1.0f : 0.0f) - bottom_popup_combo_dropdown_fade[combo_index]) * bottom_popup_step;
                if (bottom_popup_combo_dropdown_fade[combo_index] < 0.01f)
                    bottom_popup_combo_dropdown_fade[combo_index] = 0.0f;
                if (bottom_popup_combo_dropdown_fade[combo_index] > 0.99f)
                    bottom_popup_combo_dropdown_fade[combo_index] = 1.0f;
                const float dropdown_eased = smooth_fade(bottom_popup_combo_dropdown_fade[combo_index]);
                const float dropdown_slide_offset = combo_index == 0
                    ? (1.0f - dropdown_eased) * -7.0f
                    : (1.0f - dropdown_eased) * 7.0f;
                const ImVec2 dropdown_draw_min(dropdown_min.x, dropdown_min.y + dropdown_slide_offset);
                const ImVec2 dropdown_draw_max(dropdown_max.x, dropdown_max.y + dropdown_slide_offset);
                const float visible_dropdown_height = dropdown_size.y * dropdown_eased;
                const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_dropdown_height);
                const bool combo_hovered =
                    io.MousePos.x >= combo_min.x && io.MousePos.x <= combo_max.x &&
                    io.MousePos.y >= combo_min.y && io.MousePos.y <= combo_max.y;
                const bool dropdown_hovered =
                    io.MousePos.x >= dropdown_draw_min.x && io.MousePos.x <= dropdown_draw_max.x &&
                    io.MousePos.y >= dropdown_draw_min.y && io.MousePos.y <= visible_dropdown_max.y &&
                    bottom_popup_combo_dropdown_fade[combo_index] > 0.005f;
                bottom_popup_combo_hover_fade[combo_index] += ((combo_hovered ? 1.0f : 0.0f) - bottom_popup_combo_hover_fade[combo_index]) * bottom_popup_step;
                const float combo_hover_eased = smooth_fade(bottom_popup_combo_hover_fade[combo_index]);
                if (combo_hovered)
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (bottom_popup_open && !bottom_popup_any_dropdown_hovered && combo_hovered && io.MouseClicked[0])
                {
                    bottom_popup_combo_open[combo_index] = !bottom_popup_combo_open[combo_index];
                    bottom_popup_combo_open[1 - combo_index] = false;
                }
                if (bottom_popup_open && bottom_popup_combo_open[combo_index] && io.MouseClicked[0] && !combo_hovered && !dropdown_hovered)
                    bottom_popup_combo_open[combo_index] = false;
                popup_draw_list->AddRectFilled(
                    combo_min,
                    combo_max,
                    IM_COL32(
                        static_cast<int>(24.0f + 7.0f * combo_hover_eased),
                        static_cast<int>(24.0f + 7.0f * combo_hover_eased),
                        static_cast<int>(24.0f + 7.0f * combo_hover_eased),
                        popup_alpha),
                    2.0f);
                popup_draw_list->AddRect(
                    combo_min,
                    combo_max,
                    IM_COL32(36, 36, 36, popup_alpha),
                    2.0f,
                    0,
                    1.0f);
                popup_draw_list->AddRect(
                    ImVec2(combo_min.x - 1.0f, combo_min.y - 1.0f),
                    ImVec2(combo_max.x + 1.0f, combo_max.y + 1.0f),
                    IM_COL32(10, 10, 10, popup_alpha),
                    3.0f,
                    0,
                    1.0f);
                const char* combo_value = popup_combo_items[combo_index][popup_combo_selected_indices[combo_index]];
                const ImVec2 combo_text_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, combo_value);
                popup_draw_list->AddText(
                    title_font,
                    12.0f,
                    ImVec2(combo_min.x + 10.0f, combo_min.y + (combo_size.y - combo_text_size.y) * 0.5f - 1.0f),
                    IM_COL32(226, 226, 230, popup_alpha),
                    combo_value);
                const char* combo_arrow = u8"\uf4fa";
                const ImVec2 combo_arrow_size = icon_font->CalcTextSizeA(10.0f, 1000.0f, 0.0f, combo_arrow);
                popup_draw_list->AddText(
                    icon_font,
                    10.0f,
                    ImVec2(combo_max.x - 17.0f - combo_arrow_size.x * 0.5f, combo_min.y + (combo_size.y - combo_arrow_size.y) * 0.5f),
                    IM_COL32(142, 134, 246, static_cast<int>((150.0f + 60.0f * combo_hover_eased) * bottom_popup_eased)),
                    combo_arrow);
            }
            for (int combo_index = 0; combo_index < 2; ++combo_index)
            {
                const float dropdown_eased = smooth_fade(bottom_popup_combo_dropdown_fade[combo_index]);
                if (dropdown_eased <= 0.005f)
                    continue;
                const ImVec2 combo_min(popup_draw_min.x + 15.0f, popup_draw_min.y + 126.0f + combo_index * 38.0f);
                const ImVec2 combo_size(178.0f, 30.0f);
                const ImVec2 combo_max(combo_min.x + combo_size.x, combo_min.y + combo_size.y);
                const float dropdown_item_height = 23.0f;
                const ImVec2 dropdown_size(combo_size.x, 10.0f + dropdown_item_height * 2.0f);
                const ImVec2 dropdown_min(
                    combo_min.x,
                    combo_index == 0 ? combo_max.y + 6.0f : combo_min.y - dropdown_size.y - 6.0f);
                const float dropdown_slide_offset = combo_index == 0
                    ? (1.0f - dropdown_eased) * -7.0f
                    : (1.0f - dropdown_eased) * 7.0f;
                const ImVec2 dropdown_draw_min(dropdown_min.x, dropdown_min.y + dropdown_slide_offset);
                const ImVec2 dropdown_draw_max(dropdown_min.x + dropdown_size.x, dropdown_draw_min.y + dropdown_size.y);
                const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + dropdown_size.y * dropdown_eased);
                const int dropdown_alpha = static_cast<int>(popup_alpha * dropdown_eased);
                popup_draw_list->AddRectFilled(
                    dropdown_draw_min,
                    visible_dropdown_max,
                    IM_COL32(24, 24, 24, dropdown_alpha),
                    3.0f);
                popup_draw_list->AddRect(
                    dropdown_draw_min,
                    visible_dropdown_max,
                    IM_COL32(36, 36, 36, dropdown_alpha),
                    3.0f,
                    0,
                    1.0f);
                popup_draw_list->AddRect(
                    ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                    ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                    IM_COL32(10, 10, 10, dropdown_alpha),
                    4.0f,
                    0,
                    1.0f);
                popup_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                const int selected_index = combo_index == 0 ? bottom_popup_language_index : bottom_popup_device_index;
                for (int item_index = 0; item_index < 2; ++item_index)
                {
                    const ImVec2 item_min(dropdown_draw_min.x + 6.0f, dropdown_draw_min.y + 5.0f + dropdown_item_height * item_index);
                    const ImVec2 item_max(dropdown_draw_max.x - 6.0f, item_min.y + dropdown_item_height);
                    const bool item_hovered =
                        io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                        io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                        item_max.y <= visible_dropdown_max.y;
                    if (bottom_popup_open && bottom_popup_combo_open[combo_index] && item_hovered && io.MouseClicked[0])
                    {
                        if (combo_index == 0)
                            bottom_popup_language_index = item_index;
                        else
                            bottom_popup_device_index = item_index;
                        bottom_popup_combo_open[combo_index] = false;
                    }
                    if (item_hovered)
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    const bool item_selected = item_index == selected_index;
                    const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                    bottom_popup_combo_item_fade[combo_index][item_index] += (item_target - bottom_popup_combo_item_fade[combo_index][item_index]) * bottom_popup_step;
                    const float item_eased = smooth_fade(bottom_popup_combo_item_fade[combo_index][item_index]);
                    if (item_selected || item_hovered)
                    {
                        popup_draw_list->AddRectFilled(
                            item_min,
                            item_max,
                            item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                            2.0f);
                    }
                    const int item_text_value = static_cast<int>(84.0f + (235.0f - 84.0f) * item_eased);
                    float item_text_x = item_min.x + 10.0f;
                    if (item_selected)
                    {
                        popup_draw_list->AddText(
                            icon_font,
                            11.0f,
                            ImVec2(item_min.x + 9.0f, item_min.y + 5.0f),
                            IM_COL32(142, 134, 246, dropdown_alpha),
                            u8"\uf313");
                        item_text_x += 20.0f;
                    }
                    popup_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(item_text_x, item_min.y + 5.0f),
                        IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                        popup_combo_items[combo_index][item_index]);
                }
                popup_draw_list->PopClipRect();
            }
            popup_draw_list->AddRect(
                popup_draw_min,
                popup_draw_max,
                IM_COL32(10, 10, 10, popup_alpha),
                3.0f,
                0,
                3.0f);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool is_configurations_tab = displayed_top_tab == 6;
        sigma::child(
            ImVec2(66.0f, 56.0f),
            ImVec2(628.0f, 494.0f),
            3.0f,
            IM_COL32(9, 9, 9, 255),
            IM_COL32(17, 17, 17, 255),
            IM_COL32(5, 5, 5, 255));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (is_configurations_tab && content_alpha > 0.005f)
        {
            const ImVec2 configuration_box_min(79.0f, 69.0f);
            const ImVec2 configuration_box_size(395.0f, 45.0f);
            sigma::search_box_input(
                "##configuration_search_input",
                configuration_box_min,
                configuration_box_size,
                configuration_search,
                IM_ARRAYSIZE(configuration_search),
                "Search for config...",
                u8"\uf5ae",
                content_alpha,
                content_interactive);
            const ImVec2 configbox_min(configuration_box_min.x, configuration_box_min.y + configuration_box_size.y + 18.0f);
            const ImVec2 configbox_size(190.0f, 106.0f);
            const float configbox_gap = 10.0f;
            const ImVec2 configbox_button_size(32.0f, 32.0f);
            int visible_config_index = 0;
            for (int config_index = 0; config_index < config_count; ++config_index)
            {
                ConfigEntry& config = configs[config_index];
                const float delete_target = config.deleting ? 0.0f : 1.0f;
                const float delete_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
                config.fade += (delete_target - config.fade) * delete_step;
                if (config.deleting && config.fade <= 0.025f && config_count > 1)
                {
                    for (int move_index = config_index; move_index < config_count - 1; ++move_index)
                        configs[move_index] = configs[move_index + 1];
                    configs[config_count - 1] = ConfigEntry{};
                    --config_count;
                    --config_index;
                    continue;
                }
                const float config_alpha = smooth_fade(config.fade) * content_alpha;
                const bool config_enabled = content_interactive && !config.deleting && config_alpha > 0.25f;
                if (!contains_case_insensitive(config.name, configuration_search))
                    continue;
                const int column = visible_config_index % 3;
                const int row = visible_config_index / 3;
                ++visible_config_index;
                const ImVec2 current_configbox_min(
                    configbox_min.x + column * (configbox_size.x + configbox_gap),
                    configbox_min.y + row * (configbox_size.y + configbox_gap));
                char input_id[64];
                char trash_id[64];
                char copy_id[64];
                char play_id[64];
                sprintf_s(input_id, IM_ARRAYSIZE(input_id), "##configuration_name_input_%d", config_index);
                sprintf_s(trash_id, IM_ARRAYSIZE(trash_id), "##configuration_trash_%d", config_index);
                sprintf_s(copy_id, IM_ARRAYSIZE(copy_id), "##configuration_copy_%d", config_index);
                sprintf_s(play_id, IM_ARRAYSIZE(play_id), "##configuration_play_%d", config_index);
                sigma::configbox(current_configbox_min, configbox_size, config_alpha);
                sigma::config_name_input(
                    input_id,
                    ImVec2(current_configbox_min.x + 16.0f, current_configbox_min.y + 16.0f),
                    130.0f,
                    config.name,
                    IM_ARRAYSIZE(config.name),
                    config.editing,
                    config.underline_fade,
                    config_alpha,
                    config_enabled);
                sigma::config_date(ImVec2(current_configbox_min.x + 16.0f, current_configbox_min.y + 48.0f), config.created_at, config_alpha);
                if (sigma::configbox_actions(trash_id, copy_id, current_configbox_min, config.action_hover_fade[0], config.action_hover_fade[1], config_alpha, config_enabled) && config_count > 1)
                {
                    config.deleting = true;
                    config.editing = false;
                }
                sigma::icon_button(
                    play_id,
                    u8"\uf254",
                    ImVec2(
                        current_configbox_min.x + configbox_size.x - 14.0f - configbox_button_size.x,
                        current_configbox_min.y + configbox_size.y - 13.0f - configbox_button_size.y),
                    configbox_button_size,
                    config.button_hover_fade,
                    14.0f,
                    config_alpha,
                    config_enabled);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float configuration_action_center_y = configuration_box_min.y + configuration_box_size.y * 0.5f;
            sigma::icon_action("##configuration_action_one", u8"\uf2cb", ImVec2(493.0f, configuration_action_center_y), configuration_icon_hover_fade[0], content_alpha, content_interactive);
            ImGui::GetBackgroundDrawList()->AddRectFilled(
                ImVec2(516.0f, configuration_action_center_y - 4.0f),
                ImVec2(517.0f, configuration_action_center_y + 4.0f),
                IM_COL32(16, 16, 16, static_cast<int>(255.0f * content_alpha)));
            sigma::icon_action("##configuration_action_two", u8"\uf41c", ImVec2(539.0f, configuration_action_center_y), configuration_icon_hover_fade[1], content_alpha, content_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 create_button_size(106.0f, 36.0f);
            const float create_button_available_min_x = 556.0f;
            const float create_button_available_max_x = 66.0f + 628.0f - 15.0f;
            const ImVec2 create_button_min(
                create_button_available_min_x + (create_button_available_max_x - create_button_available_min_x - create_button_size.x) * 0.5f,
                configuration_box_min.y + (configuration_box_size.y - create_button_size.y) * 0.5f);
            if (sigma::button("Create", u8"\uf317", create_button_min, create_button_size, configuration_create_button_hover_fade, 12.0f, 14.0f, content_alpha, content_interactive))
            {
                create_overlay_fade = 0.0f;
                create_overlay_open = true;
                create_overlay_click_lock = true;
                g_BlurCacheValid = false;
                g_BlurCaptureRequested = true;
            }
        }
        else if (content_alpha > 0.005f)
        {
            if (displayed_top_tab == 7)
            {
                const ImVec2 general_min(81.0f, 70.0f);
                const ImVec2 general_size(293.0f, 326.0f);
                const ImVec2 addons_min(392.0f, 70.0f);
                const ImVec2 addons_size(293.0f, 206.0f);
                sigma::child(
                    general_min,
                    general_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(22, 22, 22, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                sigma::child_header(general_min, general_size, u8"\uf3dc", "General", content_alpha);
                sigma::child(
                    addons_min,
                    addons_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(22, 22, 22, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                sigma::child_header(addons_min, addons_size, u8"\uf0f0", "Addons", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                ImDrawList* settings_draw_list = ImGui::GetBackgroundDrawList();
                const int settings_alpha = static_cast<int>(255.0f * content_alpha);
                const auto capture_pressed_key = []() -> ImGuiKey
                {
                    for (int key_index = ImGuiKey_NamedKey_BEGIN; key_index < ImGuiKey_NamedKey_END; ++key_index)
                    {
                        const ImGuiKey key = static_cast<ImGuiKey>(key_index);
                        if (ImGui::IsKeyPressed(key, false))
                            return key;
                    }
                    return ImGuiKey_None;
                };
                for (int key_index = 0; key_index < IM_ARRAYSIZE(settings_key_waiting); ++key_index)
                {
                    if (!settings_key_waiting[key_index])
                        continue;
                    const ImGuiKey pressed_key = capture_pressed_key();
                    if (pressed_key != ImGuiKey_None)
                    {
                        settings_key_bindings[key_index] = pressed_key;
                        settings_key_waiting[key_index] = false;
                    }
                }
                const auto key_button_width = [&](
                    const char* value,
                    float minimum_width) -> float
                {
                    const float key_icon_size = 12.0f;
                    const float key_text_size = 11.0f;
                    const char* key_icon = u8"\uf450";
                    const ImVec2 icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                    const ImVec2 text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, value);
                    const float gap = 7.0f;
                    const float content_width = icon_measure.x + gap + text_measure.x + 18.0f;
                    return content_width > minimum_width ? content_width : minimum_width;
                };
                const auto draw_key_button = [&](
                    const char* id,
                    float right_x,
                    float y,
                    float minimum_width,
                    int key_index) -> bool
                {
                    const char* value = settings_key_waiting[key_index] ? "..." : ImGui::GetKeyName(settings_key_bindings[key_index]);
                    const float target_width = key_button_width(value, minimum_width);
                    if (settings_key_width[key_index] <= 0.0f)
                        settings_key_width[key_index] = target_width;
                    const float width_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                    settings_key_width[key_index] += (target_width - settings_key_width[key_index]) * width_step;
                    if (settings_key_width[key_index] < minimum_width)
                        settings_key_width[key_index] = minimum_width;
                    const ImVec2 size(settings_key_width[key_index], 32.0f);
                    const ImVec2 button_min(right_x - size.x, y);
                    const ImVec2 key_max(button_min.x + size.x, button_min.y + size.y);
                    bool hovered = false;
                    bool clicked = false;
                    if (content_interactive)
                    {
                        (void)id;
                        hovered = mouse_in_rect(io.MousePos, button_min, key_max);
                        clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                        if (hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    settings_key_hover_fade[key_index] += ((hovered || settings_key_waiting[key_index] ? 1.0f : 0.0f) - settings_key_hover_fade[key_index]) * (io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f);
                    const float hover = smooth_fade(settings_key_hover_fade[key_index]);
                    const int fill_value = static_cast<int>(24.0f + 8.0f * hover);
                    const int stroke_value = static_cast<int>(31.0f + 18.0f * hover);
                    settings_draw_list->AddRectFilled(button_min, key_max, IM_COL32(fill_value, fill_value, fill_value, settings_alpha), 2.0f);
                    settings_draw_list->AddRect(button_min, key_max, IM_COL32(stroke_value, stroke_value, stroke_value, settings_alpha), 2.0f, 0, 1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float key_icon_size = 12.0f;
                    const float key_text_size = 11.0f;
                    const char* key_icon = u8"\uf450";
                    const ImVec2 icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                    const ImVec2 text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, value);
                    const float gap = 7.0f;
                    const float content_width = icon_measure.x + gap + text_measure.x;
                    const float start_x = button_min.x + (size.x - content_width) * 0.5f;
                    const float center_y = button_min.y + size.y * 0.5f;
                    settings_draw_list->AddText(
                        icon_font,
                        key_icon_size,
                        ImVec2(start_x, center_y - icon_measure.y * 0.5f),
                        IM_COL32(142, 134, 246, settings_alpha),
                        key_icon);
                    settings_draw_list->AddText(
                        medium_font,
                        key_text_size,
                        ImVec2(start_x + icon_measure.x + gap, center_y - text_measure.y * 0.5f),
                        IM_COL32(205, 205, 210, settings_alpha),
                        value);
                    return clicked;
                };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                settings_draw_list->AddText(title_font, 13.0f, ImVec2(general_min.x + 13.0f, general_min.y + 63.0f), IM_COL32(205, 205, 210, settings_alpha), "Menu Key");
                if (draw_key_button("##settings_menu_key_bind", general_min.x + general_size.x - 14.0f, general_min.y + 54.0f, 55.0f, 0))
                {
                    settings_key_waiting[0] = true;
                    settings_key_waiting[1] = false;
                }
                settings_draw_list->AddText(title_font, 13.0f, ImVec2(general_min.x + 13.0f, general_min.y + 110.0f), IM_COL32(205, 205, 210, settings_alpha), "Unload Key");
                if (draw_key_button("##settings_unload_key_bind", general_min.x + general_size.x - 14.0f, general_min.y + 101.0f, 57.0f, 1))
                {
                    settings_key_waiting[1] = true;
                    settings_key_waiting[0] = false;
                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const char* general_toggle_labels[] = {
                    "Self Destruct",
                    "Stream Mode",
                    "VSync"
                };
                for (int option_index = 0; option_index < IM_ARRAYSIZE(general_toggle_labels); ++option_index)
                {
                    const float row_y = general_min.y + 145.0f + option_index * 29.0f;
                    char option_id[48];
                    ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##settings_general_%d", option_index);
                    sigma::checkbox(
                        option_id,
                        general_toggle_labels[option_index],
                        ImVec2(general_min.x + 16.0f, row_y - 9.0f),
                        ImVec2(267.0f, 29.0f),
                        ImVec2(general_min.x + 13.0f, row_y),
                        ImVec2(general_min.x + 249.0f, row_y - 2.0f),
                        settings_general_enabled[option_index],
                        settings_general_toggle_fade[option_index],
                        settings_general_hover_fade[option_index],
                        13.0f,
                        false,
                        content_alpha);
                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                sigma::button(
                    "Unload",
                    u8"\uf313",
                    ImVec2(general_min.x + 13.0f, general_min.y + 232.0f),
                    ImVec2(265.0f, 33.0f),
                    settings_button_hover_fade[0],
                    12.0f,
                    14.0f,
                    content_alpha,
                    content_interactive);
                sigma::button(
                    "Crash Game",
                    u8"\uf313",
                    ImVec2(general_min.x + 13.0f, general_min.y + 274.0f),
                    ImVec2(265.0f, 33.0f),
                    settings_button_hover_fade[1],
                    12.0f,
                    14.0f,
                    content_alpha,
                    content_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const char* addon_labels[] = {
                    "WaterMark",
                    "Console",
                    "Safe Mode",
                    "Anti Screenshot",
                    "Projector"
                };
                for (int option_index = 0; option_index < IM_ARRAYSIZE(addon_labels); ++option_index)
                {
                    const float row_y = addons_min.y + 57.0f + option_index * 29.0f;
                    char option_id[48];
                    ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##settings_addon_%d", option_index);
                    sigma::checkbox(
                        option_id,
                        addon_labels[option_index],
                        ImVec2(addons_min.x + 16.0f, row_y - 9.0f),
                        ImVec2(267.0f, 29.0f),
                        ImVec2(addons_min.x + 13.0f, row_y),
                        ImVec2(addons_min.x + 249.0f, row_y - 2.0f),
                        settings_addon_enabled[option_index],
                        settings_addon_toggle_fade[option_index],
                        settings_addon_hover_fade[option_index],
                        13.0f,
                        false,
                        content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (option_index == 0 || option_index == 3)
                    {
                        settings_draw_list->AddText(
                            icon_font,
                            13.5f,
                            ImVec2(addons_min.x + 224.0f, row_y - 3.0f),
                            IM_COL32(118, 118, 124, static_cast<int>(150.0f * content_alpha)),
                            u8"\uf3dc");
                    }
                }
            }
            else if (displayed_top_tab == 5 && render_sub_tab == 0)
            {
                const ImVec2 executor_box_min(80.0f, 70.0f);
                const ImVec2 executor_box_size(600.0f, 416.0f);
                sigma::child(
                    executor_box_min,
                    executor_box_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(17, 17, 17, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                const ImVec2 editor_min(executor_box_min.x + 8.0f, executor_box_min.y + 8.0f);
                const ImVec2 editor_size(executor_box_size.x - 16.0f, executor_box_size.y - 16.0f);
                sigma::child(
                    editor_min,
                    editor_size,
                    3.0f,
                    IM_COL32(19, 19, 19, 255),
                    IM_COL32(24, 24, 24, 255),
                    IM_COL32(8, 8, 8, 255),
                    content_alpha);
                sigma::code_editor(
                    "##executor_code_editor",
                    editor_min,
                    editor_size,
                    executor_code,
                    IM_ARRAYSIZE(executor_code),
                    content_alpha,
                    content_interactive);
                const ImVec2 monitor_combo_size(120.0f, 29.0f);
                const ImVec2 monitor_combo_min(
                    66.0f + 628.0f - 14.0f - monitor_combo_size.x,
                    56.0f + 494.0f - 25.0f - monitor_combo_size.y);
                sigma::combobox("monitor", monitor_combo_min, monitor_combo_size, content_alpha, content_interactive);
                const ImVec2 execute_button_size(80.0f, 33.0f);
                const ImVec2 load_button_size(110.0f, 33.0f);
                const ImVec2 executor_button_min(executor_box_min.x, executor_box_min.y + executor_box_size.y + 10.0f);
                sigma::button(
                    "Execute",
                    u8"\uf313",
                    executor_button_min,
                    execute_button_size,
                    executor_button_hover_fade[0],
                    12.0f,
                    13.0f,
                    content_alpha,
                    content_interactive);
                sigma::button(
                    "Load file",
                    u8"\uf457",
                    ImVec2(executor_button_min.x + execute_button_size.x + 10.0f, executor_button_min.y),
                    load_button_size,
                    executor_button_hover_fade[1],
                    12.0f,
                    13.0f,
                    content_alpha,
                    content_interactive);
            }
            else if (displayed_top_tab == 5 && render_sub_tab == 1)
            {
                const ImVec2 resource_list_min(81.0f, 70.0f);
                const ImVec2 resource_list_size(293.0f, 466.0f);
                sigma::child(
                    resource_list_min,
                    resource_list_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(22, 22, 22, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                sigma::child_header(resource_list_min, resource_list_size, u8"\uf3a5", "Resource List", content_alpha);
                sigma::search_box_input(
                    "##resource_search_input",
                    ImVec2(resource_list_min.x + 14.0f, resource_list_min.y + 58.0f),
                    ImVec2(265.0f, 36.0f),
                    resource_search,
                    IM_ARRAYSIZE(resource_search),
                    "Search resource...",
                    u8"\uf5ae",
                    content_alpha,
                    content_interactive);
                const ImVec2 resource_box_min(resource_list_min.x + 13.0f, resource_list_min.y + 107.0f);
                const ImVec2 resource_box_size(267.0f, 346.0f);
                sigma::child(
                    resource_box_min,
                    resource_box_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(16, 16, 16, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                const char* resource_names[] = { "monitor", "chat" };
                const float resource_row_height = 36.0f;
                const float resource_row_gap = 5.0f;
                const float resource_row_x = resource_box_min.x + 7.0f;
                const float resource_row_width = resource_box_size.x - 14.0f;
                const float resource_first_row_y = resource_box_min.y + 11.0f;
                float resource_visible_y[IM_ARRAYSIZE(resource_names)] = {};
                bool resource_visible[IM_ARRAYSIZE(resource_names)] = {};
                int resource_visible_index = 0;
                for (int resource_index = 0; resource_index < IM_ARRAYSIZE(resource_names); ++resource_index)
                {
                    if (!contains_case_insensitive(resource_names[resource_index], resource_search))
                        continue;
                    resource_visible[resource_index] = true;
                    resource_visible_y[resource_index] = resource_first_row_y + resource_visible_index * (resource_row_height + resource_row_gap);
                    ++resource_visible_index;
                }
                if (selected_resource_index >= 0 && !resource_visible[selected_resource_index])
                    selected_resource_index = -1;
                const bool selected_resource_visible = selected_resource_index >= 0 && resource_visible[selected_resource_index];
                const float resource_anim_step = io.DeltaTime * 11.5f > 1.0f ? 1.0f : io.DeltaTime * 11.5f;
                if (selected_resource_visible)
                {
                    const float target_y = resource_visible_y[selected_resource_index];
                    if (resource_selection_y <= 0.0f)
                        resource_selection_y = target_y;
                    resource_selection_y += (target_y - resource_selection_y) * resource_anim_step;
                    resource_selection_slide += (1.0f - resource_selection_slide) * resource_anim_step;
                    resource_selection_alpha += (1.0f - resource_selection_alpha) * resource_anim_step;
                    const float eased_slide = smooth_fade(resource_selection_slide);
                    const float selection_x = resource_row_x - 18.0f + 18.0f * eased_slide;
                    ImGui::GetBackgroundDrawList()->AddRectFilled(
                        ImVec2(selection_x, resource_selection_y),
                        ImVec2(resource_row_x + resource_row_width, resource_selection_y + resource_row_height),
                        IM_COL32(23, 23, 23, static_cast<int>(255.0f * smooth_fade(resource_selection_alpha) * content_alpha)),
                        3.0f);
                }
                else
                {
                    resource_selection_alpha += (0.0f - resource_selection_alpha) * resource_anim_step;
                    resource_selection_slide += (0.0f - resource_selection_slide) * resource_anim_step;
                }
                for (int resource_index = 0; resource_index < IM_ARRAYSIZE(resource_names); ++resource_index)
                {
                    if (!resource_visible[resource_index])
                        continue;
                    const ImVec2 row_min(resource_row_x, resource_visible_y[resource_index]);
                    const ImVec2 row_max(resource_row_x + resource_row_width, resource_visible_y[resource_index] + resource_row_height);
                    const bool row_hovered =
                        io.MousePos.x >= row_min.x && io.MousePos.x <= row_max.x &&
                        io.MousePos.y >= row_min.y && io.MousePos.y <= row_max.y;
                    if (content_interactive && row_hovered && io.MouseClicked[0])
                    {
                        if (selected_resource_index != resource_index)
                        {
                            resource_selection_slide = 0.0f;
                            resource_selection_alpha = 0.35f;
                        }
                        selected_resource_index = resource_index;
                    }
                    if (content_interactive && row_hovered)
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const bool selected = resource_index == selected_resource_index && selected_resource_visible;
                    resource_item_text_fade[resource_index] += ((selected ? 1.0f : 0.0f) - resource_item_text_fade[resource_index]) * resource_anim_step;
                    const float active_text = smooth_fade(resource_item_text_fade[resource_index]);
                    const int muted = static_cast<int>(74.0f + (235.0f - 74.0f) * active_text);
                    const int text_alpha = static_cast<int>(255.0f * content_alpha);
                    const int icon_alpha = static_cast<int>(255.0f * content_alpha * active_text);
                    if (icon_alpha > 0)
                    {
                        ImGui::GetBackgroundDrawList()->AddText(
                            icon_font,
                            12.0f,
                            ImVec2(row_min.x + 15.0f, row_min.y + 11.0f),
                            IM_COL32(142, 134, 246, icon_alpha),
                            u8"\uf313");
                    }
                    const ImVec2 text_pos(row_min.x + 18.0f + 22.0f * active_text, row_min.y + 10.0f);
                    ImGui::GetBackgroundDrawList()->AddText(
                        medium_font,
                        13.0f,
                        text_pos,
                        IM_COL32(muted, muted, muted + 4 > 255 ? 255 : muted + 4, text_alpha),
                        resource_names[resource_index]);
                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const ImVec2 resource_action_min(392.0f, 70.0f);
                const ImVec2 resource_action_size(293.0f, 253.0f);
                sigma::child(
                    resource_action_min,
                    resource_action_size,
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(22, 22, 22, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                sigma::child_header(resource_action_min, resource_action_size, u8"\uf0f0", "Action", content_alpha);
                ImDrawList* action_draw_list = ImGui::GetBackgroundDrawList();
                const int action_alpha = static_cast<int>(255.0f * content_alpha);
                const float action_line_x1 = resource_action_min.x + resource_action_size.x - 278.0f;
                const float action_line_x2 = resource_action_min.x + resource_action_size.x;
                const float action_top_line_y = resource_action_min.y + 43.0f + 12.0f;
                action_draw_list->AddRectFilled(
                    ImVec2(action_line_x1, action_top_line_y),
                    ImVec2(action_line_x2, action_top_line_y + 1.0f),
                    IM_COL32(22, 22, 22, action_alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const char* selected_resource_name = selected_resource_visible ? resource_names[selected_resource_index] : "-";
                const float action_label_size = 12.0f;
                const float action_value_size = 12.0f;
                const ImVec2 name_label_pos(resource_action_min.x + 14.0f, action_top_line_y + 22.0f);
                const ImVec2 status_label_pos(resource_action_min.x + 14.0f, action_top_line_y + 50.0f);
                action_draw_list->AddText(
                    title_font,
                    action_label_size,
                    name_label_pos,
                    IM_COL32(78, 78, 82, action_alpha),
                    "Name:");
                const ImVec2 name_value_size = title_font->CalcTextSizeA(action_value_size, 1000.0f, 0.0f, selected_resource_name);
                action_draw_list->AddText(
                    title_font,
                    action_value_size,
                    ImVec2(resource_action_min.x + resource_action_size.x - 14.0f - name_value_size.x, name_label_pos.y),
                    IM_COL32(232, 232, 236, action_alpha),
                    selected_resource_name);
                action_draw_list->AddText(
                    title_font,
                    action_label_size,
                    status_label_pos,
                    IM_COL32(78, 78, 82, action_alpha),
                    "Status:");
                const bool selected_resource_running = selected_resource_visible && resource_running[selected_resource_index];
                const char* status_text = selected_resource_visible ? (selected_resource_running ? "Started" : "Stopped") : "-";
                const ImVec2 status_value_size = title_font->CalcTextSizeA(action_value_size, 1000.0f, 0.0f, status_text);
                action_draw_list->AddText(
                    title_font,
                    action_value_size,
                    ImVec2(resource_action_min.x + resource_action_size.x - 14.0f - status_value_size.x, status_label_pos.y),
                    selected_resource_visible ? (selected_resource_running ? IM_COL32(77, 200, 107, action_alpha) : IM_COL32(214, 82, 82, action_alpha)) : IM_COL32(232, 232, 236, action_alpha),
                    status_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                const float action_mid_line_y = status_label_pos.y + 30.0f;
                action_draw_list->AddRectFilled(
                    ImVec2(action_line_x1, action_mid_line_y),
                    ImVec2(action_line_x2, action_mid_line_y + 1.0f),
                    IM_COL32(22, 22, 22, action_alpha));
                const ImVec2 resource_stop_button_size(265.0f, 33.0f);
                const ImVec2 resource_stop_button_min(resource_action_min.x + 14.0f, action_mid_line_y + 13.0f);
                const char* resource_toggle_label = selected_resource_visible ? (selected_resource_running ? "Stop Resource" : "Start Resource") : "Stop Resource";
                if (sigma::button(
                    resource_toggle_label,
                    resource_stop_button_min,
                    resource_stop_button_size,
                    resource_action_button_hover_fade[0],
                    12.0f,
                    content_alpha,
                    content_interactive && selected_resource_visible))
                {
                    resource_running[selected_resource_index] = !resource_running[selected_resource_index];
                }
                bool any_resource_running = false;
                for (int resource_index = 0; resource_index < IM_ARRAYSIZE(resource_names); ++resource_index)
                    any_resource_running = any_resource_running || resource_running[resource_index];
                const char* all_resource_toggle_label = any_resource_running ? "Stop All Resources" : "Start All Resources";
                if (sigma::button(
                    all_resource_toggle_label,
                    ImVec2(resource_stop_button_min.x, resource_stop_button_min.y + resource_stop_button_size.y + 12.0f),
                    resource_stop_button_size,
                    resource_action_button_hover_fade[1],
                    12.0f,
                    content_alpha,
                    content_interactive))
                {
                    const bool next_running_state = !any_resource_running;
                    for (int resource_index = 0; resource_index < IM_ARRAYSIZE(resource_names); ++resource_index)
                        resource_running[resource_index] = next_running_state;
                }
            }
            else if (displayed_top_tab == 5 && render_sub_tab == 2)
            {
                sigma::search_box_input(
                    "##dumper_filter_input",
                    ImVec2(80.0f, 73.0f),
                    ImVec2(200.0f, 37.0f),
                    dumper_filter,
                    IM_ARRAYSIZE(dumper_filter),
                    "Filter resources...",
                    u8"\uf5ae",
                    content_alpha,
                    content_interactive);
                ImGui::GetBackgroundDrawList()->AddRectFilled(
                    ImVec2(286.0f, 71.0f),
                    ImVec2(287.0f, 536.0f),
                    IM_COL32(44, 44, 44, static_cast<int>(255.0f * content_alpha)));
                const ImVec2 dump_preview_min(291.0f, 71.0f);
                const ImVec2 dump_preview_size(389.0f, 462.0f);
                sigma::child(
                    dump_preview_min,
                    dump_preview_size,
                    3.0f,
                    IM_COL32(24, 24, 24, 255),
                    IM_COL32(31, 31, 31, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                const char* dump_empty_title = "No dump loaded yet.";
                const char* dump_empty_hint = "Click \"Dump resources\" for preview.";
                const float dump_empty_text_size = 12.0f;
                const ImVec2 dump_title_size = title_font->CalcTextSizeA(dump_empty_text_size, 1000.0f, 0.0f, dump_empty_title);
                const ImVec2 dump_hint_size = title_font->CalcTextSizeA(dump_empty_text_size, 1000.0f, 0.0f, dump_empty_hint);
                const float dump_text_center_y = dump_preview_min.y + dump_preview_size.y * 0.5f;
                const int dump_text_alpha = static_cast<int>(82.0f * content_alpha);
                ImGui::GetBackgroundDrawList()->AddText(
                    title_font,
                    dump_empty_text_size,
                    ImVec2(dump_preview_min.x + (dump_preview_size.x - dump_title_size.x) * 0.5f, dump_text_center_y - 13.0f),
                    IM_COL32(96, 96, 100, dump_text_alpha),
                    dump_empty_title);
                ImGui::GetBackgroundDrawList()->AddText(
                    title_font,
                    dump_empty_text_size,
                    ImVec2(dump_preview_min.x + (dump_preview_size.x - dump_hint_size.x) * 0.5f, dump_text_center_y + 1.0f),
                    IM_COL32(96, 96, 100, dump_text_alpha),
                    dump_empty_hint);
                sigma::child(
                    ImVec2(80.0f, 119.0f),
                    ImVec2(200.0f, 326.0f),
                    3.0f,
                    IM_COL32(14, 14, 14, 255),
                    IM_COL32(17, 17, 17, 255),
                    IM_COL32(5, 5, 5, 255),
                    content_alpha);
                sigma::button(
                    "Dump resources",
                    u8"\uf6d6",
                    ImVec2(80.0f, 453.0f),
                    ImVec2(200.0f, 33.0f),
                    dumper_button_hover_fade,
                    12.0f,
                    14.0f,
                    content_alpha,
                    content_interactive);
                sigma::disabled_button(
                    "Open dump folder",
                    u8"\uf457",
                    ImVec2(80.0f, 493.0f),
                    ImVec2(200.0f, 33.0f),
                    12.0f,
                    14.0f,
                    content_alpha);
            }
            else
            {
                const auto combat_keybind_name = [](ImGuiKey key) -> const char*
                {
                    switch (key)
                    {
                    case ImGuiKey_None:
                        return "NONE";
                    case ImGuiKey_MouseLeft:
                        return "M1";
                    case ImGuiKey_MouseRight:
                        return "M2";
                    case ImGuiKey_MouseMiddle:
                        return "M3";
                    case ImGuiKey_MouseX1:
                        return "M4";
                    case ImGuiKey_MouseX2:
                        return "M5";
                    default:
                        break;
                    }
                    const char* name = ImGui::GetKeyName(key);
                    return name && name[0] ? name : "NONE";
                };
                const auto combat_capture_keybind = [&](bool& waiting, ImGuiKey& keybind, int wait_frame)
                {
                    if (!waiting || ImGui::GetFrameCount() <= wait_frame)
                        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    bool captured = false;
                    ImGuiKey captured_key = ImGuiKey_None;
                    if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
                    {
                        captured = true;
                        captured_key = ImGuiKey_None;
                    }
                    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        captured = true;
                        captured_key = ImGuiKey_MouseLeft;
                    }
                    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    {
                        captured = true;
                        captured_key = ImGuiKey_MouseRight;
                    }
                    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
                    {
                        captured = true;
                        captured_key = ImGuiKey_MouseMiddle;
                    }
                    else if (ImGui::IsMouseClicked(3))
                    {
                        captured = true;
                        captured_key = ImGuiKey_MouseX1;
                    }
                    else if (ImGui::IsMouseClicked(4))
                    {
                        captured = true;
                        captured_key = ImGuiKey_MouseX2;
                    }
                    else
                    {
                        for (int key_index = ImGuiKey_NamedKey_BEGIN; key_index < ImGuiKey_NamedKey_END; ++key_index)
                        {
                            const ImGuiKey key = static_cast<ImGuiKey>(key_index);
                            if (key == ImGuiKey_MouseLeft ||
                                key == ImGuiKey_MouseRight ||
                                key == ImGuiKey_MouseMiddle ||
                                key == ImGuiKey_MouseX1 ||
                                key == ImGuiKey_MouseX2 ||
                                key == ImGuiKey_MouseWheelX ||
                                key == ImGuiKey_MouseWheelY)
                                continue;
                            if (ImGui::IsKeyPressed(key, false))
                            {
                                captured = true;
                                captured_key = key;
                                break;
                            }
                        }
                    }
                    if (captured)
                    {
                        keybind = captured_key;
                        waiting = false;
                    }
                };
                const auto combat_popup_active = [&]() -> bool
                {
                    for (int popup_index = 0; popup_index < IM_ARRAYSIZE(combat_popup_open); ++popup_index)
                    {
                        if (combat_popup_open[popup_index] || combat_popup_fade[popup_index] > 0.005f)
                            return true;
                    }
                    return false;
                };
                const auto close_combat_popups = [&](int keep_index)
                {
                    for (int popup_index = 0; popup_index < IM_ARRAYSIZE(combat_popup_open); ++popup_index)
                    {
                        if (popup_index == keep_index)
                            continue;
                        combat_popup_open[popup_index] = false;
                        combat_keybind_waiting[popup_index] = false;
                    }
                };
                const auto draw_combat_gear_button = [&](
                    const char* id_prefix,
                    int popup_index,
                    const ImVec2& gear_pos,
                    bool enabled,
                    bool& out_hovered,
                    bool& out_clicked)
                {
                    const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                    const ImVec2 gear_hit_size(24.0f, 24.0f);
                    const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                    out_hovered = false;
                    out_clicked = false;
                    if (enabled && content_interactive)
                    {
                        (void)id_prefix;
                        const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                        out_hovered = mouse_in_rect(io.MousePos, gear_hit_min, gear_hit_max);
                        out_clicked = out_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                        if (out_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    else if (!content_interactive)
                    {
                        combat_popup_open[popup_index] = false;
                        combat_keybind_waiting[popup_index] = false;
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (out_clicked)
                    {
                        const bool next_open = !combat_popup_open[popup_index];
                        close_combat_popups(popup_index);
                        combat_popup_open[popup_index] = next_open;
                        if (!next_open)
                            combat_keybind_waiting[popup_index] = false;
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                    combat_gear_hover_fade[popup_index] += (((out_hovered || combat_popup_open[popup_index]) ? 1.0f : 0.0f) - combat_gear_hover_fade[popup_index]) * gear_step;
                    const float gear_hover = smooth_fade(combat_gear_hover_fade[popup_index]);
                    const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                    const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                    const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                    const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                    ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
                };
                const auto handle_combat_checkbox_hit = [&](
                    const ImVec2& left_min,
                    float row_y,
                    bool controls_interactive,
                    bool gear_hovered,
                    bool& enabled,
                    float& hover_fade)
                {
                    const ImVec2 label_hit_min(left_min.x + 16.0f, row_y - 9.0f);
                    const ImVec2 label_hit_max(left_min.x + 215.0f, row_y + 20.0f);
                    const ImVec2 toggle_hit_min(left_min.x + 246.0f, row_y - 8.0f);
                    const ImVec2 toggle_hit_max(left_min.x + 282.0f, row_y + 19.0f);
                    const bool hovered =
                        controls_interactive &&
                        !gear_hovered &&
                        ((io.MousePos.x >= label_hit_min.x && io.MousePos.x <= label_hit_max.x &&
                          io.MousePos.y >= label_hit_min.y && io.MousePos.y <= label_hit_max.y) ||
                         (io.MousePos.x >= toggle_hit_min.x && io.MousePos.x <= toggle_hit_max.x &&
                          io.MousePos.y >= toggle_hit_min.y && io.MousePos.y <= toggle_hit_max.y));
                    if (hovered)
                    {
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            enabled = !enabled;
                    }
                    const float hover_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                    hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * hover_step;
                };
                const auto draw_combat_popup_shell = [&](
                    const char* id_prefix,
                    int popup_index,
                    const ImVec2& popup_min,
                    const ImVec2& popup_size,
                    float popup_alpha_f) -> bool
                {
                    const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                    if (content_interactive)
                    {
                        char window_id[64];
                        ImFormatString(window_id, IM_ARRAYSIZE(window_id), "##%s_shell_%d", id_prefix, popup_index);
                        ImGui::SetNextWindowPos(popup_min);
                        ImGui::SetNextWindowSize(popup_size);
                        ImGui::Begin(
                            window_id,
                            nullptr,
                            ImGuiWindowFlags_NoDecoration |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoBackground |
                            ImGuiWindowFlags_NoScrollbar |
                            ImGuiWindowFlags_NoScrollWithMouse);
                        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
                        ImGui::InvisibleButton("##input", popup_size);
                        ImGui::End();
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    popup_draw_list->AddRectFilled(
                        ImVec2(popup_min.x + 4.0f, popup_min.y + 5.0f),
                        ImVec2(popup_max.x + 4.0f, popup_max.y + 5.0f),
                        IM_COL32(0, 0, 0, static_cast<int>(90.0f * popup_alpha_f)),
                        3.0f);
                    popup_draw_list->AddRectFilled(popup_min, popup_max, IM_COL32(14, 14, 14, popup_alpha), 3.0f);
                    popup_draw_list->AddRect(popup_min, popup_max, IM_COL32(27, 27, 29, static_cast<int>(235.0f * popup_alpha_f)), 3.0f);
                    return ImGui::IsMouseHoveringRect(popup_min, popup_max, false);
                };
                const auto draw_combat_popup_keybind = [&](
                    const char* id_prefix,
                    int popup_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    float y,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const char* value = combat_keybind_waiting[popup_index] ? "..." : combat_keybind_name(combat_keybinds[popup_index]);
                    const char* key_icon = u8"\uf450";
                    const float key_icon_size = 12.0f;
                    const float key_text_size = 11.0f;
                    const ImVec2 key_icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                    const ImVec2 key_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, value);
                    const float key_gap = 7.0f;
                    const float key_content_width = key_icon_measure.x + key_gap + key_text_measure.x;
                    const float key_button_width = key_content_width + 18.0f > 74.0f ? key_content_width + 18.0f : 74.0f;
                    const ImVec2 key_button_size(key_button_width, 32.0f);
                    const ImVec2 key_button_min(popup_min.x + popup_width - key_button_size.x - 16.0f, popup_min.y + y);
                    const ImVec2 key_button_max(key_button_min.x + key_button_size.x, key_button_min.y + key_button_size.y);
                    bool key_hovered = false;
                    bool key_clicked = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        key_hovered = mouse_in_rect(io.MousePos, key_button_min, key_button_max);
                        key_clicked = key_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                        if (key_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    if (key_clicked && !combat_keybind_waiting[popup_index])
                    {
                        combat_keybind_waiting[popup_index] = true;
                        combat_keybind_wait_frame[popup_index] = ImGui::GetFrameCount();
                    }
                    combat_capture_keybind(combat_keybind_waiting[popup_index], combat_keybinds[popup_index], combat_keybind_wait_frame[popup_index]);
                    const char* draw_value = combat_keybind_waiting[popup_index] ? "..." : combat_keybind_name(combat_keybinds[popup_index]);
                    const ImVec2 draw_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, draw_value);
                    const float draw_content_width = key_icon_measure.x + key_gap + draw_text_measure.x;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    combat_key_hover_fade[popup_index] += (((key_hovered || combat_keybind_waiting[popup_index]) ? 1.0f : 0.0f) - combat_key_hover_fade[popup_index]) * (io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f);
                    const float key_hover = smooth_fade(combat_key_hover_fade[popup_index]);
                    const int key_fill = static_cast<int>(24.0f + 8.0f * key_hover);
                    const int key_stroke = static_cast<int>(31.0f + 18.0f * key_hover);
                    popup_draw_list->AddRectFilled(key_button_min, key_button_max, IM_COL32(key_fill, key_fill, key_fill, popup_alpha), 2.0f);
                    popup_draw_list->AddRect(key_button_min, key_button_max, IM_COL32(key_stroke, key_stroke, key_stroke, popup_alpha), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float key_start_x = key_button_min.x + (key_button_size.x - draw_content_width) * 0.5f;
                    const float key_center_y = key_button_min.y + key_button_size.y * 0.5f;
                    popup_draw_list->AddText(icon_font, key_icon_size, ImVec2(key_start_x, key_center_y - key_icon_measure.y * 0.5f), IM_COL32(142, 134, 246, popup_alpha), key_icon);
                    popup_draw_list->AddText(medium_font, key_text_size, ImVec2(key_start_x + key_icon_measure.x + key_gap, key_center_y - draw_text_measure.y * 0.5f), IM_COL32(205, 205, 210, popup_alpha), draw_value);
                };
                const auto draw_combat_popup_slider = [&](
                    const char* id_prefix,
                    int slider_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    const char* label,
                    float label_y,
                    float track_y,
                    float max_value,
                    const char* value_format,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const ImVec2 track_min(popup_min.x + 16.0f, popup_min.y + track_y);
                    const ImVec2 track_max(popup_min.x + popup_width - 16.0f, popup_min.y + track_y + 5.0f);
                    const ImVec2 hit_min(track_min.x, track_min.y - 10.0f);
                    const ImVec2 hit_size(track_max.x - track_min.x, 24.0f);
                    const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                    bool slider_hovered = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        slider_hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                        if (slider_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (slider_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            combat_popup_slider_dragging[slider_index] = true;
                    }
                    if (!io.MouseDown[ImGuiMouseButton_Left])
                        combat_popup_slider_dragging[slider_index] = false;
                    if (combat_popup_slider_dragging[slider_index])
                    {
                        const float width = track_max.x - track_min.x;
                        const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
                        combat_popup_slider_target_value[slider_index] = ImClamp(t, 0.0f, 1.0f) * max_value;
                    }
                    const float smooth_step = io.DeltaTime * 18.0f > 1.0f ? 1.0f : io.DeltaTime * 18.0f;
                    combat_popup_slider_value[slider_index] += (combat_popup_slider_target_value[slider_index] - combat_popup_slider_value[slider_index]) * smooth_step;
                    if (combat_popup_slider_value[slider_index] - combat_popup_slider_target_value[slider_index] < 0.01f && combat_popup_slider_target_value[slider_index] - combat_popup_slider_value[slider_index] < 0.01f)
                        combat_popup_slider_value[slider_index] = combat_popup_slider_target_value[slider_index];
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    char value_text[32];
                    ImFormatString(value_text, IM_ARRAYSIZE(value_text), value_format, combat_popup_slider_value[slider_index]);
                    const ImVec2 value_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, value_text);
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + popup_width - 16.0f - value_size.x, popup_min.y + label_y), IM_COL32(88, 88, 94, popup_alpha), value_text);
                    popup_draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, popup_alpha), 2.5f);
                    const float value_t = max_value > 0.0f ? ImClamp(combat_popup_slider_value[slider_index] / max_value, 0.0f, 1.0f) : 0.0f;
                    const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
                    if (fill_x > track_min.x + 0.5f)
                        popup_draw_list->AddRectFilled(track_min, ImVec2(fill_x, track_max.y), IM_COL32(148, 140, 246, popup_alpha), 2.5f);
                    popup_draw_list->AddCircleFilled(ImVec2(fill_x, track_min.y + 2.5f), slider_hovered || combat_popup_slider_dragging[slider_index] ? 7.5f : 7.0f, IM_COL32(44, 44, 44, popup_alpha), 24);
                };
                const auto draw_combat_popup_toggle = [&](
                    const char* id_prefix,
                    int toggle_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    const char* label,
                    float y,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const ImVec2 hit_min(popup_min.x + 16.0f, popup_min.y + y - 6.0f);
                    const ImVec2 hit_size(popup_width - 32.0f, 26.0f);
                    const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                    bool hovered = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                        if (hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            combat_popup_toggle_enabled[toggle_index] = !combat_popup_toggle_enabled[toggle_index];
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    combat_popup_toggle_fade[toggle_index] += ((combat_popup_toggle_enabled[toggle_index] ? 1.0f : 0.0f) - combat_popup_toggle_fade[toggle_index]) * (io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f);
                    const float enabled_fade = smooth_fade(combat_popup_toggle_fade[toggle_index]);
                    const ImVec2 toggle_min(popup_min.x + popup_width - 41.0f, popup_min.y + y - 2.0f);
                    const ImVec2 toggle_size(25.0f, 15.0f);
                    const int track_r = static_cast<int>(32.0f + (148.0f - 32.0f) * enabled_fade);
                    const int track_g = static_cast<int>(32.0f + (140.0f - 32.0f) * enabled_fade);
                    const int track_b = static_cast<int>(34.0f + (246.0f - 34.0f) * enabled_fade);
                    const int knob_value = static_cast<int>(86.0f + (10.0f - 86.0f) * enabled_fade);
                    const float knob_x = toggle_min.x + 7.5f + (toggle_size.x - 15.0f) * enabled_fade;
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + y), IM_COL32(110, 110, 116, popup_alpha), label);
                    popup_draw_list->AddRectFilled(toggle_min, ImVec2(toggle_min.x + toggle_size.x, toggle_min.y + toggle_size.y), IM_COL32(track_r, track_g, track_b, popup_alpha), toggle_size.y * 0.5f);
                    popup_draw_list->AddCircleFilled(ImVec2(knob_x, toggle_min.y + toggle_size.y * 0.5f), hovered ? 5.0f : 4.6f, IM_COL32(knob_value, knob_value, knob_value, popup_alpha), 24);
                };
                const auto visual_popup_active = [&]() -> bool
                {
                    for (int popup_index = 0; popup_index < IM_ARRAYSIZE(visual_popup_open); ++popup_index)
                    {
                        if (visual_popup_open[popup_index] || visual_popup_fade[popup_index] > 0.005f)
                            return true;
                    }
                    return false;
                };
                const auto close_visual_popups = [&](int keep_index)
                {
                    for (int popup_index = 0; popup_index < IM_ARRAYSIZE(visual_popup_open); ++popup_index)
                    {
                        if (popup_index == keep_index)
                            continue;
                        visual_popup_open[popup_index] = false;
                        visual_keybind_waiting[popup_index] = false;
                    }
                };
                const auto draw_visual_popup_keybind = [&](
                    const char* id_prefix,
                    int popup_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    float y,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const char* value = visual_keybind_waiting[popup_index] ? "..." : combat_keybind_name(visual_keybinds[popup_index]);
                    const char* key_icon = u8"\uf450";
                    const float key_icon_size = 12.0f;
                    const float key_text_size = 11.0f;
                    const ImVec2 key_icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                    const ImVec2 key_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, value);
                    const float key_gap = 7.0f;
                    const float key_content_width = key_icon_measure.x + key_gap + key_text_measure.x;
                    const float key_button_width = key_content_width + 18.0f > 74.0f ? key_content_width + 18.0f : 74.0f;
                    const ImVec2 key_button_size(key_button_width, 32.0f);
                    const ImVec2 key_button_min(popup_min.x + popup_width - key_button_size.x - 16.0f, popup_min.y + y);
                    const ImVec2 key_button_max(key_button_min.x + key_button_size.x, key_button_min.y + key_button_size.y);
                    bool key_hovered = false;
                    bool key_clicked = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        key_hovered = mouse_in_rect(io.MousePos, key_button_min, key_button_max);
                        key_clicked = key_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                        if (key_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    if (key_clicked && !visual_keybind_waiting[popup_index])
                    {
                        visual_keybind_waiting[popup_index] = true;
                        visual_keybind_wait_frame[popup_index] = ImGui::GetFrameCount();
                    }
                    combat_capture_keybind(visual_keybind_waiting[popup_index], visual_keybinds[popup_index], visual_keybind_wait_frame[popup_index]);
                    const char* draw_value = visual_keybind_waiting[popup_index] ? "..." : combat_keybind_name(visual_keybinds[popup_index]);
                    const ImVec2 draw_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, draw_value);
                    const float draw_content_width = key_icon_measure.x + key_gap + draw_text_measure.x;
                    visual_key_hover_fade[popup_index] += (((key_hovered || visual_keybind_waiting[popup_index]) ? 1.0f : 0.0f) - visual_key_hover_fade[popup_index]) * (io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f);
                    const float key_hover = smooth_fade(visual_key_hover_fade[popup_index]);
                    const int key_fill = static_cast<int>(24.0f + 8.0f * key_hover);
                    const int key_stroke = static_cast<int>(31.0f + 18.0f * key_hover);
                    popup_draw_list->AddRectFilled(key_button_min, key_button_max, IM_COL32(key_fill, key_fill, key_fill, popup_alpha), 2.0f);
                    popup_draw_list->AddRect(key_button_min, key_button_max, IM_COL32(key_stroke, key_stroke, key_stroke, popup_alpha), 2.0f);
                    const float key_start_x = key_button_min.x + (key_button_size.x - draw_content_width) * 0.5f;
                    const float key_center_y = key_button_min.y + key_button_size.y * 0.5f;
                    popup_draw_list->AddText(icon_font, key_icon_size, ImVec2(key_start_x, key_center_y - key_icon_measure.y * 0.5f), IM_COL32(142, 134, 246, popup_alpha), key_icon);
                    popup_draw_list->AddText(medium_font, key_text_size, ImVec2(key_start_x + key_icon_measure.x + key_gap, key_center_y - draw_text_measure.y * 0.5f), IM_COL32(205, 205, 210, popup_alpha), draw_value);
                };
                const auto draw_visual_popup_select = [&](
                    const ImVec2& popup_min,
                    float popup_width,
                    const char* label,
                    const char* value,
                    float label_y,
                    float box_y,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const ImVec2 box_min(popup_min.x + 16.0f, popup_min.y + box_y);
                    const ImVec2 box_max(popup_min.x + popup_width - 16.0f, popup_min.y + box_y + 29.0f);
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                    popup_draw_list->AddRectFilled(box_min, box_max, IM_COL32(22, 22, 22, popup_alpha), 2.0f);
                    popup_draw_list->AddText(medium_font, 12.0f, ImVec2(box_min.x + 10.0f, box_min.y + 8.0f), IM_COL32(205, 205, 210, popup_alpha), value);
                    popup_draw_list->AddText(icon_font, 11.0f, ImVec2(box_max.x - 18.0f, box_min.y + 9.0f), IM_COL32(142, 134, 246, popup_alpha), u8"\uf078");
                };
                const auto draw_visual_popup_slider = [&](
                    const char* id_prefix,
                    int slider_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    const char* label,
                    float label_y,
                    float track_y,
                    float max_value,
                    const char* value_format,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const ImVec2 track_min(popup_min.x + 16.0f, popup_min.y + track_y);
                    const ImVec2 track_max(popup_min.x + popup_width - 16.0f, popup_min.y + track_y + 5.0f);
                    const ImVec2 hit_min(track_min.x, track_min.y - 10.0f);
                    const ImVec2 hit_max(track_max.x, track_min.y + 14.0f);
                    bool slider_hovered = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        slider_hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                        if (slider_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (slider_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            visual_popup_slider_dragging[slider_index] = true;
                    }
                    if (!io.MouseDown[ImGuiMouseButton_Left])
                        visual_popup_slider_dragging[slider_index] = false;
                    if (visual_popup_slider_dragging[slider_index])
                    {
                        const float width = track_max.x - track_min.x;
                        const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
                        visual_popup_slider_target_value[slider_index] = ImClamp(t, 0.0f, 1.0f) * max_value;
                    }
                    const float smooth_step = io.DeltaTime * 18.0f > 1.0f ? 1.0f : io.DeltaTime * 18.0f;
                    visual_popup_slider_value[slider_index] += (visual_popup_slider_target_value[slider_index] - visual_popup_slider_value[slider_index]) * smooth_step;
                    if (visual_popup_slider_value[slider_index] - visual_popup_slider_target_value[slider_index] < 0.01f && visual_popup_slider_target_value[slider_index] - visual_popup_slider_value[slider_index] < 0.01f)
                        visual_popup_slider_value[slider_index] = visual_popup_slider_target_value[slider_index];
                    char value_text[32];
                    ImFormatString(value_text, IM_ARRAYSIZE(value_text), value_format, visual_popup_slider_value[slider_index]);
                    const ImVec2 value_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, value_text);
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + popup_width - 16.0f - value_size.x, popup_min.y + label_y), IM_COL32(88, 88, 94, popup_alpha), value_text);
                    popup_draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, popup_alpha), 2.5f);
                    const float value_t = max_value > 0.0f ? ImClamp(visual_popup_slider_value[slider_index] / max_value, 0.0f, 1.0f) : 0.0f;
                    const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
                    if (fill_x > track_min.x + 0.5f)
                        popup_draw_list->AddRectFilled(track_min, ImVec2(fill_x, track_max.y), IM_COL32(148, 140, 246, popup_alpha), 2.5f);
                    popup_draw_list->AddCircleFilled(ImVec2(fill_x, track_min.y + 2.5f), slider_hovered || visual_popup_slider_dragging[slider_index] ? 7.5f : 7.0f, IM_COL32(44, 44, 44, popup_alpha), 24);
                };
                const auto draw_visual_popup_toggle = [&](
                    const char* id_prefix,
                    int toggle_index,
                    const ImVec2& popup_min,
                    float popup_width,
                    const char* label,
                    float y,
                    float popup_alpha_f)
                {
                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                    const ImVec2 hit_min(popup_min.x + 16.0f, popup_min.y + y - 6.0f);
                    const ImVec2 hit_size(popup_width - 32.0f, 26.0f);
                    const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                    bool hovered = false;
                    if (content_interactive)
                    {
                        (void)id_prefix;
                        hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                        if (hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            visual_popup_toggle_enabled[toggle_index] = !visual_popup_toggle_enabled[toggle_index];
                    }
                    visual_popup_toggle_fade[toggle_index] += ((visual_popup_toggle_enabled[toggle_index] ? 1.0f : 0.0f) - visual_popup_toggle_fade[toggle_index]) * (io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f);
                    const float enabled_fade = smooth_fade(visual_popup_toggle_fade[toggle_index]);
                    const ImVec2 toggle_min(popup_min.x + popup_width - 41.0f, popup_min.y + y - 2.0f);
                    const ImVec2 toggle_size(25.0f, 15.0f);
                    const int track_r = static_cast<int>(32.0f + (148.0f - 32.0f) * enabled_fade);
                    const int track_g = static_cast<int>(32.0f + (140.0f - 32.0f) * enabled_fade);
                    const int track_b = static_cast<int>(34.0f + (246.0f - 34.0f) * enabled_fade);
                    const int knob_value = static_cast<int>(86.0f + (10.0f - 86.0f) * enabled_fade);
                    const float knob_x = toggle_min.x + 7.5f + (toggle_size.x - 15.0f) * enabled_fade;
                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + y), IM_COL32(205, 205, 210, popup_alpha), label);
                    popup_draw_list->AddRectFilled(toggle_min, ImVec2(toggle_min.x + toggle_size.x, toggle_min.y + toggle_size.y), IM_COL32(track_r, track_g, track_b, popup_alpha), toggle_size.y * 0.5f);
                    popup_draw_list->AddCircleFilled(ImVec2(knob_x, toggle_min.y + toggle_size.y * 0.5f), hovered ? 5.0f : 4.6f, IM_COL32(knob_value, knob_value, knob_value, popup_alpha), 24);
                };
                if (displayed_top_tab == 0 && render_sub_tab == 0)
                {
                    const ImVec2 self_scroll_view_min(81.0f, 70.0f);
                    const ImVec2 self_scroll_view_max(685.0f, 536.0f);
                    const ImVec2 gameplay_child_size(293.0f, 201.0f);
                    const float self_child_gap = 10.0f;
                    const float auto_heal_step = io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f;
                    auto_heal_expand_fade += ((health_option_enabled[0] ? 1.0f : 0.0f) - auto_heal_expand_fade) * auto_heal_step;
                    if (auto_heal_expand_fade < 0.01f)
                        auto_heal_expand_fade = 0.0f;
                    if (auto_heal_expand_fade > 0.99f)
                        auto_heal_expand_fade = 1.0f;
                    const float auto_heal_eased = smooth_fade(auto_heal_expand_fade);
                    const float auto_heal_extra_height = 154.0f * auto_heal_eased;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float auto_armor_step = io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f;
                    auto_armor_expand_fade += ((health_option_enabled[1] ? 1.0f : 0.0f) - auto_armor_expand_fade) * auto_armor_step;
                    if (auto_armor_expand_fade < 0.01f)
                        auto_armor_expand_fade = 0.0f;
                    if (auto_armor_expand_fade > 0.99f)
                        auto_armor_expand_fade = 1.0f;
                    const float auto_armor_eased = smooth_fade(auto_armor_expand_fade);
                    const float auto_armor_extra_height = 134.0f * auto_armor_eased;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 health_child_size(293.0f, 341.0f + auto_heal_extra_height + auto_armor_extra_height);
                    const ImVec2 self_left_child_size(293.0f, 437.0f);
                    const ImVec2 self_visual_child_size(293.0f, 144.0f);
                    const float self_left_content_height = self_left_child_size.y + self_child_gap + self_visual_child_size.y;
                    const float self_right_content_height = gameplay_child_size.y + self_child_gap + health_child_size.y;
                    const float self_stack_content_height = self_left_content_height > self_right_content_height ? self_left_content_height : self_right_content_height;
                    const float self_view_height = self_scroll_view_max.y - self_scroll_view_min.y;
                    const float self_max_scroll_y = self_stack_content_height > self_view_height ? self_stack_content_height - self_view_height : 0.0f;
                    const ImVec2 self_scroll_hit_min(66.0f, 56.0f);
                    const ImVec2 self_scroll_hit_max(694.0f, 550.0f);
                    const bool self_scroll_hovered =
                        io.MousePos.x >= self_scroll_hit_min.x && io.MousePos.x <= self_scroll_hit_max.x &&
                        io.MousePos.y >= self_scroll_hit_min.y && io.MousePos.y <= self_scroll_hit_max.y;
                    if (self_scroll_hovered && content_alpha > 0.4f && self_max_scroll_y > 0.0f && io.MouseWheel != 0.0f)
                        self_player_scroll_target_y -= io.MouseWheel * 64.0f;
                    if (self_player_scroll_target_y < 0.0f)
                        self_player_scroll_target_y = 0.0f;
                    if (self_player_scroll_target_y > self_max_scroll_y)
                        self_player_scroll_target_y = self_max_scroll_y;
                    const float self_scroll_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
                    self_player_scroll_y += (self_player_scroll_target_y - self_player_scroll_y) * self_scroll_step;
                    if (self_player_scroll_y < 0.01f)
                        self_player_scroll_y = 0.0f;
                    if (self_player_scroll_y > self_max_scroll_y)
                        self_player_scroll_y = self_max_scroll_y;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* self_draw_list = ImGui::GetBackgroundDrawList();
                    self_draw_list->PushClipRect(self_scroll_view_min, self_scroll_view_max, true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto keybind_name = [](ImGuiKey key) -> const char*
                    {
                        switch (key)
                        {
                        case ImGuiKey_None:
                            return "NONE";
                        case ImGuiKey_MouseLeft:
                            return "M1";
                        case ImGuiKey_MouseRight:
                            return "M2";
                        case ImGuiKey_MouseMiddle:
                            return "M3";
                        case ImGuiKey_MouseX1:
                            return "M4";
                        case ImGuiKey_MouseX2:
                            return "M5";
                        default:
                            break;
                        }
                        const char* name = ImGui::GetKeyName(key);
                        return name && name[0] ? name : "NONE";
                    };
                    const auto capture_keybind = [&](bool& waiting, ImGuiKey& keybind, int wait_frame)
                    {
                        if (!waiting || ImGui::GetFrameCount() <= wait_frame)
                            return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        bool captured = false;
                        ImGuiKey captured_key = ImGuiKey_None;
                        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
                        {
                            captured = true;
                            captured_key = ImGuiKey_None;
                        }
                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            captured = true;
                            captured_key = ImGuiKey_MouseLeft;
                        }
                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                        {
                            captured = true;
                            captured_key = ImGuiKey_MouseRight;
                        }
                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
                        {
                            captured = true;
                            captured_key = ImGuiKey_MouseMiddle;
                        }
                        else if (ImGui::IsMouseClicked(3))
                        {
                            captured = true;
                            captured_key = ImGuiKey_MouseX1;
                        }
                        else if (ImGui::IsMouseClicked(4))
                        {
                            captured = true;
                            captured_key = ImGuiKey_MouseX2;
                        }
                        else
                        {
                            for (int key_index = ImGuiKey_NamedKey_BEGIN; key_index < ImGuiKey_NamedKey_END; ++key_index)
                            {
                                const ImGuiKey key = static_cast<ImGuiKey>(key_index);
                                if (key == ImGuiKey_MouseLeft ||
                                    key == ImGuiKey_MouseRight ||
                                    key == ImGuiKey_MouseMiddle ||
                                    key == ImGuiKey_MouseX1 ||
                                    key == ImGuiKey_MouseX2 ||
                                    key == ImGuiKey_MouseWheelX ||
                                    key == ImGuiKey_MouseWheelY)
                                    continue;
                                if (ImGui::IsKeyPressed(key, false))
                                {
                                    captured = true;
                                    captured_key = key;
                                    break;
                                }
                            }
                        }
                        if (captured)
                        {
                            keybind = captured_key;
                            waiting = false;
                        }
                    };
                    const auto close_self_keybind_popups = [&](int keep_index)
                    {
                        for (int close_index = 0; close_index < IM_ARRAYSIZE(self_keybind_popup_open); ++close_index)
                        {
                            if (close_index == keep_index)
                                continue;
                            self_keybind_popup_open[close_index] = false;
                            self_keybind_waiting[close_index] = false;
                        }
                    };
                    const auto close_gameplay_popups = [&](int keep_index)
                    {
                        for (int close_index = 0; close_index < IM_ARRAYSIZE(gameplay_popup_open); ++close_index)
                        {
                            if (close_index == keep_index)
                                continue;
                            gameplay_popup_open[close_index] = false;
                            gameplay_keybind_waiting[close_index] = false;
                        }
                    };
                    const auto draw_gear_button = [&](
                        const char* id_prefix,
                        int option_index,
                        const ImVec2& gear_pos,
                        bool& open,
                        bool& waiting,
                        float& hover_fade,
                        bool self_group,
                        bool& out_hovered,
                        bool& out_clicked)
                    {
                        const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                        const ImVec2 gear_hit_size(24.0f, 24.0f);
                        const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                        const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                        out_hovered = false;
                        out_clicked = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            (void)option_index;
                            out_hovered = mouse_in_rect(io.MousePos, gear_hit_min, gear_hit_max);
                            out_clicked = out_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                            if (out_hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        }
                        else
                        {
                            open = false;
                            waiting = false;
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        if (out_clicked)
                        {
                            const bool next_open = !open;
                            close_self_keybind_popups(self_group ? option_index : -1);
                            close_gameplay_popups(self_group ? -1 : option_index);
                            open = next_open;
                            if (!open)
                                waiting = false;
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                        hover_fade += (((out_hovered || open) ? 1.0f : 0.0f) - hover_fade) * gear_step;
                        const float gear_hover = smooth_fade(hover_fade);
                        const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                        const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                        const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                        const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                        self_draw_list->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
                    };
                    const auto draw_popup_shell = [&](
                        const ImVec2& popup_min,
                        const ImVec2& popup_size,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        popup_draw_list->AddRectFilled(
                            ImVec2(popup_min.x + 4.0f, popup_min.y + 5.0f),
                            ImVec2(popup_max.x + 4.0f, popup_max.y + 5.0f),
                            IM_COL32(0, 0, 0, static_cast<int>(90.0f * popup_alpha_f)),
                            3.0f);
                        popup_draw_list->AddRectFilled(popup_min, popup_max, IM_COL32(14, 14, 14, popup_alpha), 3.0f);
                        popup_draw_list->AddRect(popup_min, popup_max, IM_COL32(27, 27, 29, static_cast<int>(235.0f * popup_alpha_f)), 3.0f);
                    };
                    const auto draw_popup_keybind = [&](
                        const char* id_prefix,
                        int option_index,
                        const ImVec2& popup_min,
                        float popup_width,
                        float y,
                        float popup_alpha_f,
                        bool& waiting,
                        ImGuiKey& keybind,
                        int& wait_frame,
                        float& hover_fade)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const char* value = waiting ? "..." : keybind_name(keybind);
                        const char* key_icon = u8"\uf450";
                        const float key_icon_size = 12.0f;
                        const float key_text_size = 11.0f;
                        const ImVec2 key_icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                        const ImVec2 key_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, value);
                        const float key_gap = 7.0f;
                        const float key_content_width = key_icon_measure.x + key_gap + key_text_measure.x;
                        const float key_button_width = key_content_width + 18.0f > 74.0f ? key_content_width + 18.0f : 74.0f;
                        const ImVec2 key_button_size(key_button_width, 32.0f);
                        const ImVec2 key_button_min(popup_min.x + popup_width - key_button_size.x - 16.0f, popup_min.y + y);
                        const ImVec2 key_button_max(key_button_min.x + key_button_size.x, key_button_min.y + key_button_size.y);
                        bool key_hovered = false;
                        bool key_clicked = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            (void)option_index;
                            key_hovered = mouse_in_rect(io.MousePos, key_button_min, key_button_max);
                            key_clicked = key_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                            if (key_hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        }
                        if (key_clicked && !waiting)
                        {
                            waiting = true;
                            wait_frame = ImGui::GetFrameCount();
                        }
                        capture_keybind(waiting, keybind, wait_frame);
                        const char* draw_value = waiting ? "..." : keybind_name(keybind);
                        const ImVec2 draw_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, draw_value);
                        const float draw_content_width = key_icon_measure.x + key_gap + draw_text_measure.x;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        hover_fade += (((key_hovered || waiting) ? 1.0f : 0.0f) - hover_fade) * (io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f);
                        const float key_hover = smooth_fade(hover_fade);
                        const int key_fill = static_cast<int>(24.0f + 8.0f * key_hover);
                        const int key_stroke = static_cast<int>(31.0f + 18.0f * key_hover);
                        popup_draw_list->AddRectFilled(key_button_min, key_button_max, IM_COL32(key_fill, key_fill, key_fill, popup_alpha), 2.0f);
                        popup_draw_list->AddRect(key_button_min, key_button_max, IM_COL32(key_stroke, key_stroke, key_stroke, popup_alpha), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float key_start_x = key_button_min.x + (key_button_size.x - draw_content_width) * 0.5f;
                        const float key_center_y = key_button_min.y + key_button_size.y * 0.5f;
                        popup_draw_list->AddText(icon_font, key_icon_size, ImVec2(key_start_x, key_center_y - key_icon_measure.y * 0.5f), IM_COL32(142, 134, 246, popup_alpha), key_icon);
                        popup_draw_list->AddText(medium_font, key_text_size, ImVec2(key_start_x + key_icon_measure.x + key_gap, key_center_y - draw_text_measure.y * 0.5f), IM_COL32(205, 205, 210, popup_alpha), draw_value);
                    };
                    const auto draw_popup_slider = [&](
                        const char* id_prefix,
                        int option_index,
                        const ImVec2& popup_min,
                        float popup_width,
                        const char* label,
                        float label_y,
                        float track_y,
                        float& value,
                        float& target_value,
                        bool& dragging,
                        float max_value,
                        const char* value_format,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const ImVec2 track_min(popup_min.x + 16.0f, popup_min.y + track_y);
                        const ImVec2 track_max(popup_min.x + popup_width - 16.0f, popup_min.y + track_y + 5.0f);
                        const ImVec2 hit_min(track_min.x, track_min.y - 10.0f);
                        const ImVec2 hit_size(track_max.x - track_min.x, 24.0f);
                        const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                        bool slider_hovered = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            (void)option_index;
                            slider_hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                            if (slider_hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (slider_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                dragging = true;
                        }
                        if (!io.MouseDown[ImGuiMouseButton_Left])
                            dragging = false;
                        if (dragging)
                        {
                            const float width = track_max.x - track_min.x;
                            const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
                            target_value = ImClamp(t, 0.0f, 1.0f) * max_value;
                        }
                        const float smooth_step = io.DeltaTime * 18.0f > 1.0f ? 1.0f : io.DeltaTime * 18.0f;
                        value += (target_value - value) * smooth_step;
                        if (value - target_value < 0.01f && target_value - value < 0.01f)
                            value = target_value;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        char value_text[32];
                        ImFormatString(value_text, IM_ARRAYSIZE(value_text), value_format, value);
                        const ImVec2 value_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, value_text);
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + popup_width - 16.0f - value_size.x, popup_min.y + label_y), IM_COL32(88, 88, 94, popup_alpha), value_text);
                        popup_draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, popup_alpha), 2.5f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float value_t = max_value > 0.0f ? ImClamp(value / max_value, 0.0f, 1.0f) : 0.0f;
                        const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
                        if (fill_x > track_min.x + 0.5f)
                        {
                            popup_draw_list->AddRectFilled(track_min, ImVec2(fill_x, track_max.y), IM_COL32(148, 140, 246, popup_alpha), 2.5f);
                        }
                        const float knob_radius = slider_hovered || dragging ? 7.5f : 7.0f;
                        popup_draw_list->AddCircleFilled(ImVec2(fill_x, track_min.y + 2.5f), knob_radius, IM_COL32(44, 44, 44, popup_alpha), 24);
                    };
                    const auto draw_popup_toggle = [&](
                        const char* id_prefix,
                        int option_index,
                        const ImVec2& popup_min,
                        float popup_width,
                        const char* label,
                        float y,
                        bool& enabled,
                        float& toggle_fade,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const ImVec2 hit_min(popup_min.x + 16.0f, popup_min.y + y - 6.0f);
                        const ImVec2 hit_size(popup_width - 32.0f, 26.0f);
                        const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                        bool hovered = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            (void)option_index;
                            hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                            if (hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                enabled = !enabled;
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        toggle_fade += ((enabled ? 1.0f : 0.0f) - toggle_fade) * (io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f);
                        const float enabled_fade = smooth_fade(toggle_fade);
                        const ImVec2 toggle_min(popup_min.x + popup_width - 41.0f, popup_min.y + y - 2.0f);
                        const ImVec2 toggle_size(25.0f, 15.0f);
                        const int track_r = static_cast<int>(32.0f + (148.0f - 32.0f) * enabled_fade);
                        const int track_g = static_cast<int>(32.0f + (140.0f - 32.0f) * enabled_fade);
                        const int track_b = static_cast<int>(34.0f + (246.0f - 34.0f) * enabled_fade);
                        const int knob_value = static_cast<int>(86.0f + (10.0f - 86.0f) * enabled_fade);
                        const float knob_x = toggle_min.x + 7.5f + (toggle_size.x - 15.0f) * enabled_fade;
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + y), IM_COL32(110, 110, 116, popup_alpha), label);
                        popup_draw_list->AddRectFilled(toggle_min, ImVec2(toggle_min.x + toggle_size.x, toggle_min.y + toggle_size.y), IM_COL32(track_r, track_g, track_b, popup_alpha), toggle_size.y * 0.5f);
                        popup_draw_list->AddCircleFilled(ImVec2(knob_x, toggle_min.y + toggle_size.y * 0.5f), hovered ? 5.0f : 4.6f, IM_COL32(knob_value, knob_value, knob_value, popup_alpha), 24);
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 self_left_child_min(self_scroll_view_min.x, self_scroll_view_min.y - self_player_scroll_y);
                    sigma::child(self_left_child_min, self_left_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(self_left_child_min, self_left_child_size, u8"\uf53b", "Self Options", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* self_option_labels[] = {
                        "Gosth Mode",
                        "Gosth Mode Bypass",
                        "Invisible",
                        "No RagDoll",
                        "Noclip",
                        "Infinite Stamina",
                        "FastRun",
                        "GodMode",
                        "Freecam",
                        "Tiny Ped",
                        "Anti Headshot",
                        "Infinite Roll",
                        "Car Jack"
                    };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(self_option_labels); ++option_index)
                    {
                        const float row_y = self_left_child_min.y + 57.0f + option_index * 29.0f;
                        if (row_y > self_scroll_view_max.y || row_y + 22.0f < self_scroll_view_min.y)
                            continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 row_min(self_left_child_min.x + 16.0f, row_y - 9.0f);
                        const ImVec2 row_size(267.0f, 29.0f);
                        const ImVec2 label_pos(self_left_child_min.x + 13.0f, row_y);
                        const ImVec2 toggle_min(self_left_child_min.x + 249.0f, row_y - 2.0f);
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##self_option_%d", option_index);
                        const bool gear_row = option_index < 9;
                        if (!gear_row)
                        {
                            sigma::checkbox(
                                option_id,
                                self_option_labels[option_index],
                                row_min,
                                row_size,
                                label_pos,
                                toggle_min,
                                self_option_enabled[option_index],
                                self_option_toggle_fade[option_index],
                                self_option_hover_fade[option_index],
                                13.0f,
                                false,
                                content_alpha,
                                true);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        if (gear_row)
                        {
                            const ImVec2 gear_pos(self_left_child_min.x + 224.0f, row_y - 3.0f);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            draw_gear_button(
                                "self_option",
                                option_index,
                                gear_pos,
                                self_keybind_popup_open[option_index],
                                self_keybind_waiting[option_index],
                                self_gear_hover_fade[option_index],
                                true,
                                gear_hovered,
                                gear_clicked);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const ImVec2 label_hit_min(self_left_child_min.x + 16.0f, row_y - 9.0f);
                            const ImVec2 label_hit_max(self_left_child_min.x + 215.0f, row_y + 20.0f);
                            const ImVec2 toggle_hit_min(self_left_child_min.x + 246.0f, row_y - 8.0f);
                            const ImVec2 toggle_hit_max(self_left_child_min.x + 282.0f, row_y + 19.0f);
                            const bool checkbox_hovered =
                                content_interactive &&
                                !gear_hovered &&
                                ((io.MousePos.x >= label_hit_min.x && io.MousePos.x <= label_hit_max.x &&
                                  io.MousePos.y >= label_hit_min.y && io.MousePos.y <= label_hit_max.y) ||
                                 (io.MousePos.x >= toggle_hit_min.x && io.MousePos.x <= toggle_hit_max.x &&
                                  io.MousePos.y >= toggle_hit_min.y && io.MousePos.y <= toggle_hit_max.y));
                            if (checkbox_hovered)
                            {
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                    self_option_enabled[option_index] = !self_option_enabled[option_index];
                            }
                            const float checkbox_hover_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                            self_option_hover_fade[option_index] += ((checkbox_hovered ? 1.0f : 0.0f) - self_option_hover_fade[option_index]) * checkbox_hover_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            sigma::checkbox(
                                option_id,
                                self_option_labels[option_index],
                                row_min,
                                row_size,
                                label_pos,
                                toggle_min,
                                self_option_enabled[option_index],
                                self_option_toggle_fade[option_index],
                                self_option_hover_fade[option_index],
                                13.0f,
                                false,
                                content_alpha,
                                false,
                                true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            self_keybind_popup_fade[option_index] += ((self_keybind_popup_open[option_index] ? 1.0f : 0.0f) - self_keybind_popup_fade[option_index]) * popup_step;
                            if (self_keybind_popup_fade[option_index] < 0.01f)
                                self_keybind_popup_fade[option_index] = 0.0f;
                            if (self_keybind_popup_fade[option_index] > 0.0f)
                            {
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const float popup_eased = smooth_fade(self_keybind_popup_fade[option_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const ImVec2 popup_size(208.0f, 61.0f);
                                const ImVec2 popup_min(gear_center.x - 14.0f, gear_center.y + 21.0f + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                draw_popup_shell(popup_min, popup_size, popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(
                                    title_font,
                                    13.0f,
                                    ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f),
                                    IM_COL32(214, 214, 220, popup_alpha),
                                    "Keybind");
                                draw_popup_keybind(
                                    "self_option",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    14.5f,
                                    popup_alpha_f,
                                    self_keybind_waiting[option_index],
                                    self_keybinds[option_index],
                                    self_keybind_wait_frame[option_index],
                                    self_key_hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const bool popup_hovered = ImGui::IsMouseHoveringRect(popup_min, popup_max, false);
                                if (self_keybind_popup_open[option_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    self_keybind_popup_open[option_index] = false;
                                    self_keybind_waiting[option_index] = false;
                                }
                            }
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 self_visual_child_min(
                        self_scroll_view_min.x,
                        self_scroll_view_min.y + self_left_child_size.y + self_child_gap - self_player_scroll_y);
                    sigma::child(self_visual_child_min, self_visual_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(self_visual_child_min, self_visual_child_size, u8"\uf010", "Visuals", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* self_visual_option_labels[] = {
                        "Time Changer",
                        "Weather Changer",
                        "Sky Color"
                    };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(self_visual_option_labels); ++option_index)
                    {
                        const float row_y = self_visual_child_min.y + 57.0f + option_index * 29.0f;
                        if (row_y > self_scroll_view_max.y || row_y + 22.0f < self_scroll_view_min.y)
                            continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 row_min(self_visual_child_min.x + 16.0f, row_y - 9.0f);
                        const ImVec2 row_size(267.0f, 29.0f);
                        const ImVec2 label_pos(self_visual_child_min.x + 13.0f, row_y);
                        const ImVec2 toggle_min(self_visual_child_min.x + 249.0f, row_y - 2.0f);
                        char option_id[56];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##self_visual_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            self_visual_option_labels[option_index],
                            row_min,
                            row_size,
                            label_pos,
                            toggle_min,
                            self_visual_option_enabled[option_index],
                            self_visual_option_toggle_fade[option_index],
                            self_visual_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int gear_alpha = static_cast<int>(150.0f * content_alpha);
                        self_draw_list->AddText(
                            icon_font,
                            13.5f,
                            ImVec2(self_visual_child_min.x + 224.0f, row_y - 3.0f),
                            IM_COL32(118, 118, 124, gear_alpha),
                            u8"\uf3dc");
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 self_right_view_min(392.0f, self_scroll_view_min.y);
                    const ImVec2 gameplay_child_min(self_right_view_min.x, self_scroll_view_min.y - self_player_scroll_y);
                    sigma::child(gameplay_child_min, gameplay_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(gameplay_child_min, gameplay_child_size, u8"\uf19a", "Gameplay", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* gameplay_option_labels[] = {
                        "Quick Peek",
                        "Fake Lag",
                        "Desync",
                        "Damage Reducer",
                        "Wheel Silent"
                    };
                    bool any_gameplay_popup_active = false;
                    for (int popup_index = 0; popup_index < IM_ARRAYSIZE(gameplay_popup_open); ++popup_index)
                    {
                        if (gameplay_popup_open[popup_index] || gameplay_popup_fade[popup_index] > 0.005f)
                        {
                            any_gameplay_popup_active = true;
                            break;
                        }
                    }
                    bool right_popup_region_needed = false;
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(gameplay_option_labels); ++option_index)
                    {
                        const float row_y = gameplay_child_min.y + 57.0f + option_index * 29.0f;
                        if (row_y > self_scroll_view_max.y || row_y + 22.0f < self_scroll_view_min.y)
                            continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 row_min(gameplay_child_min.x + 16.0f, row_y - 9.0f);
                        const ImVec2 row_size(267.0f, 29.0f);
                        const ImVec2 label_pos(gameplay_child_min.x + 13.0f, row_y);
                        const ImVec2 toggle_min(gameplay_child_min.x + 249.0f, row_y - 2.0f);
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##gameplay_option_%d", option_index);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 gear_pos(gameplay_child_min.x + 224.0f, row_y - 3.0f);
                        bool gear_hovered = false;
                        bool gear_clicked = false;
                        draw_gear_button(
                            "gameplay_option",
                            option_index,
                            gear_pos,
                            gameplay_popup_open[option_index],
                            gameplay_keybind_waiting[option_index],
                            gameplay_gear_hover_fade[option_index],
                            false,
                            gear_hovered,
                            gear_clicked);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 label_hit_min(gameplay_child_min.x + 16.0f, row_y - 9.0f);
                        const ImVec2 label_hit_max(gameplay_child_min.x + 215.0f, row_y + 20.0f);
                        const ImVec2 toggle_hit_min(gameplay_child_min.x + 246.0f, row_y - 8.0f);
                        const ImVec2 toggle_hit_max(gameplay_child_min.x + 282.0f, row_y + 19.0f);
                        const bool checkbox_hovered =
                            content_interactive &&
                            !any_gameplay_popup_active &&
                            !gear_hovered &&
                            ((io.MousePos.x >= label_hit_min.x && io.MousePos.x <= label_hit_max.x &&
                              io.MousePos.y >= label_hit_min.y && io.MousePos.y <= label_hit_max.y) ||
                             (io.MousePos.x >= toggle_hit_min.x && io.MousePos.x <= toggle_hit_max.x &&
                              io.MousePos.y >= toggle_hit_min.y && io.MousePos.y <= toggle_hit_max.y));
                        if (checkbox_hovered)
                        {
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                gameplay_option_enabled[option_index] = !gameplay_option_enabled[option_index];
                        }
                        const float checkbox_hover_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                        gameplay_option_hover_fade[option_index] += ((checkbox_hovered ? 1.0f : 0.0f) - gameplay_option_hover_fade[option_index]) * checkbox_hover_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        sigma::checkbox(
                            option_id,
                            gameplay_option_labels[option_index],
                            row_min,
                            row_size,
                            label_pos,
                            toggle_min,
                            gameplay_option_enabled[option_index],
                            gameplay_option_toggle_fade[option_index],
                            gameplay_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            false,
                            true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                        gameplay_popup_fade[option_index] += ((gameplay_popup_open[option_index] ? 1.0f : 0.0f) - gameplay_popup_fade[option_index]) * popup_step;
                        if (gameplay_popup_fade[option_index] < 0.01f)
                            gameplay_popup_fade[option_index] = 0.0f;
                        if (gameplay_popup_fade[option_index] > 0.005f)
                            right_popup_region_needed = true;
                        if (gameplay_popup_fade[option_index] > 0.0f)
                        {
                            const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                            const float popup_eased = smooth_fade(gameplay_popup_fade[option_index]);
                            const float popup_alpha_f = content_alpha * popup_eased;
                            const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                            ImVec2 popup_size(208.0f, 61.0f);
                            if (option_index == 1 || option_index == 3)
                                popup_size = ImVec2(228.0f, 112.0f);
                            else if (option_index == 4)
                                popup_size = ImVec2(248.0f, 162.0f);
                            const float gameplay_popup_down_offset = (option_index == 1 || option_index == 3 || option_index == 4) ? 8.0f : 0.0f;
                            const ImVec2 popup_min(gear_center.x - 14.0f, gear_center.y + 21.0f + gameplay_popup_down_offset + popup_slide_y);
                            const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                            const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            draw_popup_shell(popup_min, popup_size, popup_alpha_f);
                            if (option_index == 1)
                            {
                                draw_popup_slider(
                                    "gameplay_fake_lag",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    "Lag Delay (ms)",
                                    20.0f,
                                    52.0f,
                                    fake_lag_delay_value,
                                    fake_lag_delay_target_value,
                                    fake_lag_delay_dragging,
                                    500.0f,
                                    "%.0f",
                                    popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(
                                    title_font,
                                    13.0f,
                                    ImVec2(popup_min.x + 16.0f, popup_min.y + 82.0f),
                                    IM_COL32(214, 214, 220, popup_alpha),
                                    "Keybind");
                                draw_popup_keybind(
                                    "gameplay_fake_lag",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    71.5f,
                                    popup_alpha_f,
                                    gameplay_keybind_waiting[option_index],
                                    gameplay_keybinds[option_index],
                                    gameplay_keybind_wait_frame[option_index],
                                    gameplay_key_hover_fade[option_index]);
                            }
                            else if (option_index == 3)
                            {
                                draw_popup_slider(
                                    "gameplay_damage_reducer",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    "Reduction",
                                    20.0f,
                                    52.0f,
                                    damage_reduction_value,
                                    damage_reduction_target_value,
                                    damage_reduction_dragging,
                                    100.0f,
                                    "%.0f",
                                    popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(
                                    title_font,
                                    13.0f,
                                    ImVec2(popup_min.x + 16.0f, popup_min.y + 82.0f),
                                    IM_COL32(214, 214, 220, popup_alpha),
                                    "Keybind");
                                draw_popup_keybind(
                                    "gameplay_damage_reducer",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    71.5f,
                                    popup_alpha_f,
                                    gameplay_keybind_waiting[option_index],
                                    gameplay_keybinds[option_index],
                                    gameplay_keybind_wait_frame[option_index],
                                    gameplay_key_hover_fade[option_index]);
                            }
                            else if (option_index == 4)
                            {
                                draw_popup_toggle(
                                    "gameplay_wheel_silent",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    "Draw Target Line",
                                    22.0f,
                                    wheel_silent_target_line,
                                    wheel_silent_target_line_fade,
                                    popup_alpha_f);
                                draw_popup_slider(
                                    "gameplay_wheel_silent",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    "Max Distance",
                                    64.0f,
                                    96.0f,
                                    wheel_silent_distance_value,
                                    wheel_silent_distance_target_value,
                                    wheel_silent_distance_dragging,
                                    1000.0f,
                                    "%.0fm",
                                    popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(
                                    title_font,
                                    13.0f,
                                    ImVec2(popup_min.x + 16.0f, popup_min.y + 130.0f),
                                    IM_COL32(214, 214, 220, popup_alpha),
                                    "Keybind");
                                draw_popup_keybind(
                                    "gameplay_wheel_silent",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    119.5f,
                                    popup_alpha_f,
                                    gameplay_keybind_waiting[option_index],
                                    gameplay_keybinds[option_index],
                                    gameplay_keybind_wait_frame[option_index],
                                    gameplay_key_hover_fade[option_index]);
                            }
                            else
                            {
                                ImGui::GetForegroundDrawList()->AddText(
                                    title_font,
                                    13.0f,
                                    ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f),
                                    IM_COL32(214, 214, 220, popup_alpha),
                                    "Keybind");
                                draw_popup_keybind(
                                    "gameplay_keybind",
                                    option_index,
                                    popup_min,
                                    popup_size.x,
                                    14.5f,
                                    popup_alpha_f,
                                    gameplay_keybind_waiting[option_index],
                                    gameplay_keybinds[option_index],
                                    gameplay_keybind_wait_frame[option_index],
                                    gameplay_key_hover_fade[option_index]);
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const bool popup_hovered = ImGui::IsMouseHoveringRect(popup_min, popup_max, false);
                            if (gameplay_popup_open[option_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                            {
                                gameplay_popup_open[option_index] = false;
                                gameplay_keybind_waiting[option_index] = false;
                            }
                        }
                    }
                    if (g_RightPopupRegionActive != right_popup_region_needed)
                    {
                        g_RightPopupRegionActive = right_popup_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const bool health_controls_interactive = content_interactive && !right_popup_region_needed;
                    const ImVec2 health_child_min(self_right_view_min.x, self_scroll_view_min.y + gameplay_child_size.y + self_child_gap - self_player_scroll_y);
                    sigma::child(health_child_min, health_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(health_child_min, health_child_size, u8"\uf02e", "Health Options", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_health_checkbox = [&](
                        int option_index,
                        const char* label,
                        float row_y)
                    {
                        if (row_y > self_scroll_view_max.y || row_y + 22.0f < self_scroll_view_min.y)
                            return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 row_min(health_child_min.x + 16.0f, row_y - 9.0f);
                        const ImVec2 row_size(267.0f, 29.0f);
                        const ImVec2 label_pos(health_child_min.x + 13.0f, row_y);
                        const ImVec2 toggle_min(health_child_min.x + 249.0f, row_y - 2.0f);
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##health_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            label,
                            row_min,
                            row_size,
                            label_pos,
                            toggle_min,
                            health_option_enabled[option_index],
                            health_option_toggle_fade[option_index],
                            health_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            health_controls_interactive);
                    };
                    const float auto_heal_row_y = health_child_min.y + 57.0f;
                    draw_health_checkbox(0, "Auto Heal", auto_heal_row_y);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float auto_armor_row_y = health_child_min.y + 86.0f + auto_heal_extra_height;
                    draw_health_checkbox(1, "Auto Armor", auto_armor_row_y);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_health_slider = [&](
                        const char* label,
                        float label_y,
                        float track_y,
                        float& value,
                        float& target_value,
                        bool& dragging,
                        float max_value,
                        const char* value_format,
                        float alpha_multiplier = 1.0f)
                    {
                        const ImVec2 slider_track_max(health_child_min.x + 274.0f, track_y + 5.0f);
                        const bool slider_visible =
                            slider_track_max.y >= self_scroll_view_min.y && label_y <= self_scroll_view_max.y;
                        if (!slider_visible)
                            return;
                        sigma::slider(
                            label,
                            health_child_min,
                            label_y,
                            track_y,
                            value,
                            target_value,
                            dragging,
                            max_value,
                            value_format,
                            content_alpha,
                            health_controls_interactive,
                            alpha_multiplier);
                    };
                    const float auto_heal_slide_y = (1.0f - auto_heal_eased) * -12.0f;
                    if (auto_heal_expand_fade > 0.001f)
                    {
                        draw_health_slider(
                            "Minimum HP",
                            health_child_min.y + 95.0f + auto_heal_slide_y,
                            health_child_min.y + 117.0f + auto_heal_slide_y,
                            minimum_hp_slider_value,
                            minimum_hp_slider_target_value,
                            minimum_hp_slider_dragging,
                            100.0f,
                            "%.0f",
                            auto_heal_eased);
                        draw_health_slider(
                            "Heal Amount",
                            health_child_min.y + 140.0f + auto_heal_slide_y,
                            health_child_min.y + 162.0f + auto_heal_slide_y,
                            heal_amount_slider_value,
                            heal_amount_slider_target_value,
                            heal_amount_slider_dragging,
                            100.0f,
                            "%.0f",
                            auto_heal_eased);
                        draw_health_slider(
                            "Heal Delay",
                            health_child_min.y + 185.0f + auto_heal_slide_y,
                            health_child_min.y + 207.0f + auto_heal_slide_y,
                            heal_delay_slider_value,
                            heal_delay_slider_target_value,
                            heal_delay_slider_dragging,
                            1.0f,
                            "%.2f min",
                            auto_heal_eased);
                    }
                    const float auto_heal_separator_y = auto_armor_row_y - 13.0f;
                    if (auto_heal_expand_fade > 0.001f && auto_heal_separator_y >= self_scroll_view_min.y && auto_heal_separator_y <= self_scroll_view_max.y)
                    {
                        self_draw_list->AddRectFilled(
                            ImVec2(health_child_min.x + 13.0f, auto_heal_separator_y),
                            ImVec2(health_child_min.x + 274.0f, auto_heal_separator_y + 1.0f),
                            IM_COL32(48, 48, 48, static_cast<int>(255.0f * content_alpha * auto_heal_eased)));
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float auto_armor_slide_y = (1.0f - auto_armor_eased) * -12.0f;
                    if (auto_armor_expand_fade > 0.001f)
                    {
                        draw_health_slider(
                            "Minimum Armor",
                            auto_armor_row_y + 31.0f + auto_armor_slide_y,
                            auto_armor_row_y + 53.0f + auto_armor_slide_y,
                            minimum_armor_slider_value,
                            minimum_armor_slider_target_value,
                            minimum_armor_slider_dragging,
                            100.0f,
                            "%.0f",
                            auto_armor_eased);
                        draw_health_slider(
                            "Armor Amount",
                            auto_armor_row_y + 76.0f + auto_armor_slide_y,
                            auto_armor_row_y + 98.0f + auto_armor_slide_y,
                            armor_amount_slider_value,
                            armor_amount_slider_target_value,
                            armor_amount_slider_dragging,
                            100.0f,
                            "%.0f",
                            auto_armor_eased);
                        draw_health_slider(
                            "Armor Delay",
                            auto_armor_row_y + 121.0f + auto_armor_slide_y,
                            auto_armor_row_y + 143.0f + auto_armor_slide_y,
                            armor_delay_slider_value,
                            armor_delay_slider_target_value,
                            armor_delay_slider_dragging,
                            1.0f,
                            "%.2f min",
                            auto_armor_eased);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float health_separator_y = health_child_min.y + 123.0f + auto_heal_extra_height + auto_armor_extra_height;
                    if (health_separator_y >= self_scroll_view_min.y && health_separator_y <= self_scroll_view_max.y)
                    {
                        self_draw_list->AddRectFilled(
                            ImVec2(health_child_min.x + 13.0f, health_separator_y),
                            ImVec2(health_child_min.x + 274.0f, health_separator_y + 1.0f),
                            IM_COL32(48, 48, 48, static_cast<int>(255.0f * content_alpha)));
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    draw_health_slider(
                        "Health",
                        health_child_min.y + 139.0f + auto_heal_extra_height + auto_armor_extra_height,
                        health_child_min.y + 161.0f + auto_heal_extra_height + auto_armor_extra_height,
                        health_slider_value,
                        health_slider_target_value,
                        health_slider_dragging,
                        200.0f,
                        "%.1f");
                    draw_health_slider(
                        "Armor",
                        health_child_min.y + 190.0f + auto_heal_extra_height + auto_armor_extra_height,
                        health_child_min.y + 212.0f + auto_heal_extra_height + auto_armor_extra_height,
                        armor_slider_value,
                        armor_slider_target_value,
                        armor_slider_dragging,
                        200.0f,
                        "%.1f");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* health_action_labels[] = {
                        "Set Health",
                        "Set Armor"
                    };
                    const ImVec2 health_action_button_size(265.0f, 33.0f);
                    const float health_action_start_y = health_child_min.y + 241.0f + auto_heal_extra_height + auto_armor_extra_height;
                    for (int button_index = 0; button_index < IM_ARRAYSIZE(health_action_labels); ++button_index)
                    {
                        const ImVec2 button_min(
                            health_child_min.x + 14.0f,
                            health_action_start_y + button_index * 47.0f);
                        if (button_min.y > self_scroll_view_max.y || button_min.y + health_action_button_size.y < self_scroll_view_min.y)
                            continue;
                        sigma::button(
                            health_action_labels[button_index],
                            button_min,
                            health_action_button_size,
                            health_action_button_hover_fade[button_index],
                            12.0f,
                            content_alpha,
                            health_controls_interactive);
                    }
                    self_draw_list->PopClipRect();
                }
                else if (displayed_top_tab == 1 && render_sub_tab == 0)
                {
                    const ImVec2 aim_scroll_view_min(81.0f, 70.0f);
                    const ImVec2 aim_scroll_view_max(685.0f, 536.0f);
                    const float aim_acceleration_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
                    aimlock_acceleration_curve_fade += ((aimlock_option_enabled[11] ? 1.0f : 0.0f) - aimlock_acceleration_curve_fade) * aim_acceleration_step;
                    if (aimlock_acceleration_curve_fade < 0.01f)
                        aimlock_acceleration_curve_fade = 0.0f;
                    if (aimlock_acceleration_curve_fade > 0.99f)
                        aimlock_acceleration_curve_fade = 1.0f;
                    const float aim_acceleration_curve_reveal = smooth_fade(aimlock_acceleration_curve_fade);
                    const ImVec2 aim_left_size(293.0f, 407.0f + 158.0f * aim_acceleration_curve_reveal);
                    const ImVec2 aim_right_size(293.0f, 650.0f);
                    const float aim_content_height = aim_right_size.y > aim_left_size.y ? aim_right_size.y : aim_left_size.y;
                    const float aim_view_height = aim_scroll_view_max.y - aim_scroll_view_min.y;
                    const float aim_max_scroll_y = aim_content_height > aim_view_height ? aim_content_height - aim_view_height : 0.0f;
                    const bool aim_scroll_hovered =
                        io.MousePos.x >= aim_scroll_view_min.x && io.MousePos.x <= aim_scroll_view_max.x &&
                        io.MousePos.y >= aim_scroll_view_min.y && io.MousePos.y <= aim_scroll_view_max.y;
                    const bool aim_combat_dropdown_active =
                        combat_dropdown_open[0] || combat_dropdown_fade[0] > 0.005f ||
                        combat_dropdown_open[1] || combat_dropdown_fade[1] > 0.005f ||
                        combat_dropdown_open[2] || combat_dropdown_fade[2] > 0.005f;
                    if (aim_scroll_hovered && content_alpha > 0.4f && aim_max_scroll_y > 0.0f && io.MouseWheel != 0.0f && !aim_combat_dropdown_active && !combat_popup_active())
                        aimlock_settings_scroll_target_y -= io.MouseWheel * 64.0f;
                    if (aimlock_settings_scroll_target_y < 0.0f)
                        aimlock_settings_scroll_target_y = 0.0f;
                    if (aimlock_settings_scroll_target_y > aim_max_scroll_y)
                        aimlock_settings_scroll_target_y = aim_max_scroll_y;
                    const float aim_scroll_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
                    aimlock_settings_scroll_y += (aimlock_settings_scroll_target_y - aimlock_settings_scroll_y) * aim_scroll_step;
                    if (aimlock_settings_scroll_y < 0.01f)
                        aimlock_settings_scroll_y = 0.0f;
                    if (aimlock_settings_scroll_y > aim_max_scroll_y)
                        aimlock_settings_scroll_y = aim_max_scroll_y;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 aim_left_min(aim_scroll_view_min.x, aim_scroll_view_min.y - aimlock_settings_scroll_y);
                    const ImVec2 aim_right_min(392.0f, aim_scroll_view_min.y - aimlock_settings_scroll_y);
                    ImDrawList* aim_draw_list = ImGui::GetBackgroundDrawList();
                    aim_draw_list->PushClipRect(aim_scroll_view_min, aim_scroll_view_max, true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        aim_left_min,
                        aim_left_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(aim_left_min, aim_left_size, u8"\uf572", "Aim Assistance", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* aim_option_labels[] = {
                        "Enable",
                        "Draw Fov Circle",
                        "Magic Bullets",
                        "Ignore NPCs",
                        "Only Visible",
                        "Ignore Dead",
                        "Prediction",
                        "Third Person",
                        "Sticky Aimbot",
                        "Humanization",
                        "Stop At Health",
                        "Aim Acceleration"
                    };
                    const bool aim_popup_active = combat_popup_active();
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(aim_option_labels); ++option_index)
                    {
                        const float row_y = aim_left_min.y + 57.0f + option_index * 29.0f;
                        const int popup_index =
                            option_index == 0 ? 0 :
                            option_index == 8 ? 1 :
                            option_index == 9 ? 2 :
                            option_index == 10 ? 3 : -1;
                        const bool gear_row = popup_index >= 0;
                        const bool row_visible = row_y <= aim_scroll_view_max.y && row_y + 22.0f >= aim_scroll_view_min.y;
                        const bool keep_popup_visible = gear_row && (combat_popup_open[popup_index] || combat_popup_fade[popup_index] > 0.005f);
                        if (!row_visible && !keep_popup_visible)
                            continue;
                        const bool row_controls_interactive = content_interactive && aim_scroll_hovered && !aim_popup_active;
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##aimlock_option_%d", option_index);
                        const bool option_clicked = sigma::checkbox(
                            option_id,
                            aim_option_labels[option_index],
                            ImVec2(aim_left_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(aim_left_min.x + 13.0f, row_y),
                            ImVec2(aim_left_min.x + 249.0f, row_y - 2.0f),
                            aimlock_option_enabled[option_index],
                            aimlock_option_toggle_fade[option_index],
                            aimlock_option_hover_fade[option_index],
                            13.0f,
                            false,
                            row_visible ? content_alpha : 0.0f,
                            row_visible && row_controls_interactive && !gear_row);
                        if (option_index == 11 && option_clicked && aimlock_option_enabled[option_index])
                            aimlock_settings_scroll_target_y = aim_max_scroll_y;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int marker_alpha = static_cast<int>(150.0f * content_alpha);
                        if (gear_row)
                        {
                            const ImVec2 gear_pos(aim_left_min.x + 224.0f, row_y - 3.0f);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            if (row_visible)
                            {
                                draw_combat_gear_button("aimlock", popup_index, gear_pos, true, gear_hovered, gear_clicked);
                                handle_combat_checkbox_hit(
                                    aim_left_min,
                                    row_y,
                                    row_controls_interactive,
                                    gear_hovered,
                                    aimlock_option_enabled[option_index],
                                    aimlock_option_hover_fade[option_index]);
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            combat_popup_fade[popup_index] += ((combat_popup_open[popup_index] ? 1.0f : 0.0f) - combat_popup_fade[popup_index]) * popup_step;
                            if (combat_popup_fade[popup_index] < 0.01f)
                                combat_popup_fade[popup_index] = 0.0f;
                            if (combat_popup_fade[popup_index] > 0.0f)
                            {
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const float popup_eased = smooth_fade(combat_popup_fade[popup_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const bool keybind_only = popup_index == 0;
                                const ImVec2 popup_size =
                                    popup_index == 1 ? ImVec2(208.0f, 104.0f) :
                                    popup_index == 2 ? ImVec2(260.0f, 184.0f) :
                                    popup_index == 3 ? ImVec2(208.0f, 75.0f) :
                                    ImVec2(208.0f, 61.0f);
                                const ImVec2 popup_min(
                                    gear_center.x + (keybind_only ? -14.0f : 10.0f),
                                    gear_center.y + (keybind_only ? 21.0f : 10.0f) + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const bool popup_hovered = draw_combat_popup_shell("aimlock", popup_index, popup_min, popup_size, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (popup_index == 0)
                                {
                                    ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                    draw_combat_popup_keybind("aimlock", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
                                }
                                else if (popup_index == 1)
                                {
                                    draw_combat_popup_slider("aimlock_sticky", 0, popup_min, popup_size.x, "Target switch delay", 18.0f, 40.0f, 500.0f, "%.0fms", popup_alpha_f);
                                    draw_combat_popup_slider("aimlock_sticky", 1, popup_min, popup_size.x, "Soft abandonment", 62.0f, 84.0f, 500.0f, "%.0fms", popup_alpha_f);
                                }
                                else if (popup_index == 2)
                                {
                                    draw_combat_popup_slider("aimlock_human", 2, popup_min, popup_size.x, "Micro reacquire min", 18.0f, 41.0f, 500.0f, "%.0fms", popup_alpha_f);
                                    draw_combat_popup_slider("aimlock_human", 3, popup_min, popup_size.x, "Micro reacquire max", 58.0f, 81.0f, 500.0f, "%.0fms", popup_alpha_f);
                                    draw_combat_popup_slider("aimlock_human", 4, popup_min, popup_size.x, "Sharp turn pause min", 98.0f, 121.0f, 500.0f, "%.0fms", popup_alpha_f);
                                    draw_combat_popup_slider("aimlock_human", 5, popup_min, popup_size.x, "Sharp turn pause max", 138.0f, 161.0f, 500.0f, "%.0fms", popup_alpha_f);
                                }
                                else if (popup_index == 3)
                                {
                                    draw_combat_popup_slider("aimlock_health", 6, popup_min, popup_size.x, "Health", 18.0f, 40.0f, 100.0f, "%.0f%%", popup_alpha_f);
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (combat_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    combat_popup_open[popup_index] = false;
                                    combat_keybind_waiting[popup_index] = false;
                                }
                        }
                        }
                        else if (option_index == 1)
                        {
                            const ImVec2 fov_marker_center(aim_left_min.x + 233.0f, row_y + 4.0f);
                            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(61, 62, 63, marker_alpha),
                                24);
                            ImGui::GetBackgroundDrawList()->AddCircle(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(202, 202, 210, marker_alpha),
                                24,
                                1.5f);
                        }
                    }
                    const bool aim_popup_region_needed = combat_popup_active();
                    const bool aim_bottom_region_needed = g_BottomProfilePopupRegionActive || aim_popup_region_needed;
                    if (g_RightPopupRegionActive != aim_popup_region_needed || g_BottomOptionPopupRegionActive != aim_popup_region_needed || g_BottomPopupRegionActive != aim_bottom_region_needed)
                    {
                        g_RightPopupRegionActive = aim_popup_region_needed;
                        g_BottomOptionPopupRegionActive = aim_popup_region_needed;
                        g_BottomPopupRegionActive = aim_bottom_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const float curve_alpha_f = content_alpha * aim_acceleration_curve_reveal;
                    if (curve_alpha_f > 0.01f)
                    {
                        const ImVec2 child_clip_max(aim_left_min.x + aim_left_size.x, aim_left_min.y + aim_left_size.y);
                        aim_draw_list->PushClipRect(aim_left_min, child_clip_max, true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int curve_alpha = static_cast<int>(255.0f * curve_alpha_f);
                        const int curve_muted_alpha = static_cast<int>(155.0f * curve_alpha_f);
                        const int curve_grid_alpha = static_cast<int>(88.0f * curve_alpha_f);
                        const float curve_label_y = aim_left_min.y + 416.0f;
                        const ImVec2 plot_min(aim_left_min.x + 13.0f, aim_left_min.y + 442.0f);
                        const ImVec2 plot_size(267.0f, 86.0f);
                        const ImVec2 plot_max(plot_min.x + plot_size.x, plot_min.y + plot_size.y);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        aim_draw_list->AddText(
                            title_font,
                            13.0f,
                            ImVec2(aim_left_min.x + 13.0f, curve_label_y),
                            IM_COL32(205, 205, 210, curve_alpha),
                            "Aim Acceleration Curve");
                        aim_draw_list->AddRectFilled(plot_min, plot_max, IM_COL32(12, 12, 12, static_cast<int>(190.0f * curve_alpha_f)), 2.0f);
                        aim_draw_list->AddRect(plot_min, plot_max, IM_COL32(46, 46, 48, curve_muted_alpha), 2.0f, 0, 1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        for (int grid_x = 1; grid_x < 4; ++grid_x)
                        {
                            const float x = plot_min.x + plot_size.x * (static_cast<float>(grid_x) / 4.0f);
                            aim_draw_list->AddLine(ImVec2(x, plot_min.y), ImVec2(x, plot_max.y), IM_COL32(64, 64, 66, curve_grid_alpha), 1.0f);
                        }
                        for (int grid_y = 1; grid_y < 4; ++grid_y)
                        {
                            const float y = plot_min.y + plot_size.y * (static_cast<float>(grid_y) / 4.0f);
                            aim_draw_list->AddLine(ImVec2(plot_min.x, y), ImVec2(plot_max.x, y), IM_COL32(64, 64, 66, curve_grid_alpha), 1.0f);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        ImVec2 curve_points[5];
                        for (int point_index = 0; point_index < IM_ARRAYSIZE(curve_points); ++point_index)
                        {
                            const float x_t = static_cast<float>(point_index) / static_cast<float>(IM_ARRAYSIZE(curve_points) - 1);
                            const float value = ImClamp(aimlock_acceleration_curve_values[point_index], 0.0f, 1.0f);
                            curve_points[point_index] = ImVec2(
                                plot_min.x + plot_size.x * x_t,
                                plot_max.y - plot_size.y * value);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        for (int point_index = 0; point_index < IM_ARRAYSIZE(curve_points) - 1; ++point_index)
                        {
                            aim_draw_list->AddLine(
                                curve_points[point_index],
                                curve_points[point_index + 1],
                                IM_COL32(235, 204, 104, curve_alpha),
                                2.0f);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        for (int point_index = 0; point_index < IM_ARRAYSIZE(curve_points); ++point_index)
                        {
                            const bool active_point = aimlock_acceleration_drag_point == point_index;
                            const float radius = active_point ? 5.1f : 4.2f;
                            aim_draw_list->AddCircleFilled(curve_points[point_index], radius + 1.5f, IM_COL32(12, 12, 12, curve_alpha), 24);
                            aim_draw_list->AddCircleFilled(curve_points[point_index], radius, IM_COL32(244, 217, 132, curve_alpha), 24);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        aim_draw_list->AddText(title_font, 12.0f, ImVec2(plot_min.x, plot_max.y + 7.0f), IM_COL32(205, 205, 210, curve_alpha), "0%");
                        aim_draw_list->AddText(title_font, 12.0f, ImVec2(plot_min.x + plot_size.x * 0.5f - 9.0f, plot_max.y + 7.0f), IM_COL32(205, 205, 210, curve_alpha), "50%");
                        aim_draw_list->AddText(title_font, 12.0f, ImVec2(plot_max.x - 27.0f, plot_max.y + 7.0f), IM_COL32(205, 205, 210, curve_alpha), "100%");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        aim_draw_list->PopClipRect();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool curve_visible_for_input =
                            content_interactive &&
                            aim_scroll_hovered &&
                            aim_acceleration_curve_reveal > 0.85f &&
                            plot_max.y >= aim_scroll_view_min.y &&
                            plot_min.y <= aim_scroll_view_max.y;
                        if (curve_visible_for_input)
                        {
                            const bool curve_hovered = mouse_in_rect(io.MousePos, plot_min, plot_max);
                            if (curve_hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (curve_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            {
                                int nearest_point = -1;
                                float nearest_distance_sq = 144.0f;
                                for (int point_index = 0; point_index < IM_ARRAYSIZE(curve_points); ++point_index)
                                {
                                    const float dx = io.MousePos.x - curve_points[point_index].x;
                                    const float dy = io.MousePos.y - curve_points[point_index].y;
                                    const float distance_sq = dx * dx + dy * dy;
                                    if (distance_sq <= nearest_distance_sq)
                                    {
                                        nearest_distance_sq = distance_sq;
                                        nearest_point = point_index;
                                    }
                                }
                                aimlock_acceleration_drag_point = nearest_point;
                            }
                            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                                aimlock_acceleration_drag_point = -1;
                            if (aimlock_acceleration_drag_point >= 0 && aimlock_acceleration_drag_point < IM_ARRAYSIZE(aimlock_acceleration_curve_values))
                            {
                                const float y_t = (io.MousePos.y - plot_min.y) / plot_size.y;
                                aimlock_acceleration_curve_values[aimlock_acceleration_drag_point] = ImClamp(1.0f - y_t, 0.0f, 1.0f);
                            }
                        }
                        else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                        {
                            aimlock_acceleration_drag_point = -1;
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        aim_right_min,
                        aim_right_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(aim_right_min, aim_right_size, u8"\uf3dc", "Aim Settings", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_aim_slider = [&](
                        const char* label,
                        float label_y,
                        float& value,
                        float& target_value,
                        bool& dragging,
                        float max_value,
                        const char* format)
                    {
                        const float actual_label_y = aim_right_min.y + label_y;
                        const float actual_track_y = actual_label_y + 22.0f;
                        if (actual_track_y + 12.0f < aim_scroll_view_min.y || actual_label_y > aim_scroll_view_max.y)
                            return;
                        sigma::slider(
                            label,
                            aim_right_min,
                            actual_label_y,
                            actual_track_y,
                            value,
                            target_value,
                            dragging,
                            max_value,
                            format,
                            content_alpha,
                            content_interactive && aim_scroll_hovered && !aim_popup_active);
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    draw_aim_slider("Field of View", 57.0f, aimlock_fov_value, aimlock_fov_target_value, aimlock_fov_dragging, 180.0f, "%.0f");
                    draw_aim_slider("Speed X", 101.0f, aimlock_speed_x_value, aimlock_speed_x_target_value, aimlock_speed_x_dragging, 100.0f, "%.0f%%");
                    draw_aim_slider("Speed Y", 145.0f, aimlock_speed_y_value, aimlock_speed_y_target_value, aimlock_speed_y_dragging, 100.0f, "%.0f%%");
                    draw_aim_slider("Speed Max", 189.0f, aimlock_speed_max_value, aimlock_speed_max_target_value, aimlock_speed_max_dragging, 100.0f, "%.0f%%");
                    draw_aim_slider("Deadzone", 233.0f, aimlock_deadzone_value, aimlock_deadzone_target_value, aimlock_deadzone_dragging, 100.0f, "%.0f%%");
                    draw_aim_slider("Vertical Target Offset", 277.0f, aimlock_vertical_offset_value, aimlock_vertical_offset_target_value, aimlock_vertical_offset_dragging, 100.0f, "%.0f%%");
                    draw_aim_slider("Retarget Time", 321.0f, aimlock_retarget_time_value, aimlock_retarget_time_target_value, aimlock_retarget_time_dragging, 1000.0f, "%.0fms");
                    draw_aim_slider("Detach Time", 365.0f, aimlock_detach_time_value, aimlock_detach_time_target_value, aimlock_detach_time_dragging, 500.0f, "%.0fms");
                    draw_aim_slider("Max Distance", 409.0f, aimlock_max_distance_value, aimlock_max_distance_target_value, aimlock_max_distance_dragging, 500.0f, "%.0fm");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const int aim_alpha = static_cast<int>(255.0f * content_alpha);
                    const char* combat_bone_items[] = {
                        "Head",
                        "Neck",
                        "Chest",
                        "Stomach",
                        "Pelvis"
                    };
                    const char* aim_priority_items[] = {
                        "Crosshair",
                        "Distance",
                        "Lowest Health"
                    };
                    const char* mouse_memory_items[] = {
                        "Mouse",
                        "Memory"
                    };
                    const auto draw_aim_combo = [&](
                        const char* label,
                        const char* const* items,
                        int item_count,
                        float label_y,
                        int dropdown_id,
                        bool multi_select,
                        int single_slot,
                        int multi_slot)
                    {
                        const float actual_label_y = aim_right_min.y + label_y;
                        const ImVec2 combo_min(aim_right_min.x + 13.0f, actual_label_y + 22.0f);
                        if (combo_min.y + 29.0f < aim_scroll_view_min.y || actual_label_y > aim_scroll_view_max.y)
                            return;
                        aim_draw_list->AddText(
                            title_font,
                            13.0f,
                            ImVec2(aim_right_min.x + 13.0f, actual_label_y),
                            IM_COL32(205, 205, 210, aim_alpha),
                            label);
                        draw_combat_dropdown(
                            dropdown_id,
                            items,
                            item_count,
                            combo_min,
                            multi_select,
                            single_slot,
                            multi_slot,
                            content_interactive && aim_scroll_hovered && !aim_popup_active,
                            true,
                            aim_scroll_view_min,
                            aim_scroll_view_max);
                    };
                    draw_aim_combo("Target Priority", aim_priority_items, IM_ARRAYSIZE(aim_priority_items), 453.0f, 0, false, 0, -1);
                    draw_aim_combo("Aimbot Bone", combat_bone_items, IM_ARRAYSIZE(combat_bone_items), 518.0f, 1, true, -1, 0);
                    draw_aim_combo("Aimbot Type", mouse_memory_items, IM_ARRAYSIZE(mouse_memory_items), 583.0f, 2, false, 1, -1);
                    aim_draw_list->PopClipRect();
                }
                else if (displayed_top_tab == 1 && render_sub_tab == 1)
                {
                    const ImVec2 silent_left_min(81.0f, 70.0f);
                    const ImVec2 silent_left_size(293.0f, 328.0f);
                    const ImVec2 silent_right_min(392.0f, 70.0f);
                    const ImVec2 silent_right_size(293.0f, 393.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        silent_left_min,
                        silent_left_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(silent_left_min, silent_left_size, u8"\uf677", "Silent Assistance", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* silent_option_labels[] = {
                        "Enable",
                        "Draw Fov Circle",
                        "Dynamic Field of view",
                        "Magic Bullets",
                        "Sticky Silent",
                        "Force Driver",
                        "Target Line",
                        "Dual FOV",
                        "Automatic Distance"
                    };
                    const bool silent_popup_active = combat_popup_active();
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(silent_option_labels); ++option_index)
                    {
                        const float row_y = silent_left_min.y + 57.0f + option_index * 29.0f;
                        const bool gear_row = option_index == 0;
                        const int popup_index = 4;
                        const bool row_controls_interactive = content_interactive && !silent_popup_active;
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##silent_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            silent_option_labels[option_index],
                            ImVec2(silent_left_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(silent_left_min.x + 13.0f, row_y),
                            ImVec2(silent_left_min.x + 249.0f, row_y - 2.0f),
                            silent_option_enabled[option_index],
                            silent_option_toggle_fade[option_index],
                            silent_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            row_controls_interactive && !gear_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int marker_alpha = static_cast<int>(150.0f * content_alpha);
                        if (gear_row)
                        {
                            const ImVec2 gear_pos(silent_left_min.x + 224.0f, row_y - 3.0f);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            draw_combat_gear_button("silent", popup_index, gear_pos, true, gear_hovered, gear_clicked);
                            handle_combat_checkbox_hit(
                                silent_left_min,
                                row_y,
                                row_controls_interactive,
                                gear_hovered,
                                silent_option_enabled[option_index],
                                silent_option_hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            combat_popup_fade[popup_index] += ((combat_popup_open[popup_index] ? 1.0f : 0.0f) - combat_popup_fade[popup_index]) * popup_step;
                            if (combat_popup_fade[popup_index] < 0.01f)
                                combat_popup_fade[popup_index] = 0.0f;
                            if (combat_popup_fade[popup_index] > 0.0f)
                            {
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const float popup_eased = smooth_fade(combat_popup_fade[popup_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const ImVec2 popup_size(208.0f, 61.0f);
                                const ImVec2 popup_min(gear_center.x - 14.0f, gear_center.y + 21.0f + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const bool popup_hovered = draw_combat_popup_shell("silent", popup_index, popup_min, popup_size, popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                draw_combat_popup_keybind("silent", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (combat_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    combat_popup_open[popup_index] = false;
                                    combat_keybind_waiting[popup_index] = false;
                                }
                            }
                        }
                        else if (option_index == 1)
                        {
                            const ImVec2 fov_marker_center(silent_left_min.x + 232.0f, row_y + 4.0f);
                            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(76, 32, 34, marker_alpha),
                                24);
                            ImGui::GetBackgroundDrawList()->AddCircle(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(206, 82, 86, marker_alpha),
                                24,
                                1.5f);
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        silent_right_min,
                        silent_right_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(silent_right_min, silent_right_size, u8"\uf3dc", "Silent Settings", content_alpha);
                    sigma::slider(
                        "Field of View",
                        silent_right_min,
                        silent_right_min.y + 57.0f,
                        silent_right_min.y + 79.0f,
                        silent_fov_value,
                        silent_fov_target_value,
                        silent_fov_dragging,
                        100.0f,
                        "%.0fpx",
                        content_alpha,
                        content_interactive && !silent_popup_active);
                    sigma::slider(
                        "Miss Chance",
                        silent_right_min,
                        silent_right_min.y + 101.0f,
                        silent_right_min.y + 123.0f,
                        silent_miss_chance_value,
                        silent_miss_chance_target_value,
                        silent_miss_chance_dragging,
                        100.0f,
                        "%.0f%%",
                        content_alpha,
                        content_interactive && !silent_popup_active);
                    sigma::slider(
                        "Max Distance",
                        silent_right_min,
                        silent_right_min.y + 145.0f,
                        silent_right_min.y + 167.0f,
                        silent_max_distance_value,
                        silent_max_distance_target_value,
                        silent_max_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !silent_popup_active);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* silent_draw_list = ImGui::GetBackgroundDrawList();
                    const int silent_alpha = static_cast<int>(255.0f * content_alpha);
                    const char* silent_filter_items[] = {
                        "Peds",
                        "Dead",
                        "Combat Rolling",
                        "In Corner",
                        "GodMode",
                        "Invisible",
                        "Not Visible"
                    };
                    const char* silent_priority_items[] = {
                        "FOV",
                        "Alternative FOV",
                        "Closest",
                        "Lowest Health"
                    };
                    const char* silent_bone_items[] = {
                        "Head",
                        "Neck",
                        "Chest",
                        "Stomach",
                        "Pelvis"
                    };
                    silent_draw_list->AddText(title_font, 13.0f, ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 189.0f), IM_COL32(205, 205, 210, silent_alpha), "Target Filters");
                    draw_combat_dropdown(3, silent_filter_items, IM_ARRAYSIZE(silent_filter_items), ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 211.0f), true, -1, 1, content_interactive && !silent_popup_active, false, ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f));
                    silent_draw_list->AddText(title_font, 13.0f, ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 254.0f), IM_COL32(205, 205, 210, silent_alpha), "Target Priority");
                    draw_combat_dropdown(4, silent_priority_items, IM_ARRAYSIZE(silent_priority_items), ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 276.0f), false, 2, -1, content_interactive && !silent_popup_active, false, ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f));
                    silent_draw_list->AddText(title_font, 13.0f, ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 319.0f), IM_COL32(205, 205, 210, silent_alpha), "Silent Bone");
                    draw_combat_dropdown(5, silent_bone_items, IM_ARRAYSIZE(silent_bone_items), ImVec2(silent_right_min.x + 13.0f, silent_right_min.y + 341.0f), true, -1, 2, content_interactive && !silent_popup_active, true, silent_right_min, ImVec2(silent_right_min.x + silent_right_size.x, silent_right_min.y + silent_right_size.y));
                }
                else if (displayed_top_tab == 1 && render_sub_tab == 2)
                {
                    const ImVec2 trigger_left_min(81.0f, 70.0f);
                    const ImVec2 trigger_left_size(293.0f, 386.0f);
                    const ImVec2 trigger_right_min(392.0f, 70.0f);
                    const ImVec2 trigger_right_size(293.0f, 466.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        trigger_left_min,
                        trigger_left_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(trigger_left_min, trigger_left_size, u8"\uf05f", "Trigger Assistance", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* trigger_option_labels[] = {
                        "Enable",
                        "Ignore NPCs",
                        "Ignore Dead",
                        "Only Visible",
                        "Force Driver",
                        "Ignore In vehicle",
                        "Ignore GodMode",
                        "Show Fov",
                        "Magnet Trigger",
                        "Dynamic FOV",
                        "Double FOV"
                    };
                    const bool trigger_popup_active = combat_popup_active();
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(trigger_option_labels); ++option_index)
                    {
                        const float row_y = trigger_left_min.y + 57.0f + option_index * 29.0f;
                        const int popup_index =
                            option_index == 0 ? 5 :
                            option_index == 8 ? 6 :
                            option_index == 9 ? 7 :
                            option_index == 10 ? 8 : -1;
                        const bool gear_row = popup_index >= 0;
                        const bool row_controls_interactive = content_interactive && !trigger_popup_active;
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##trigger_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            trigger_option_labels[option_index],
                            ImVec2(trigger_left_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(trigger_left_min.x + 13.0f, row_y),
                            ImVec2(trigger_left_min.x + 249.0f, row_y - 2.0f),
                            trigger_option_enabled[option_index],
                            trigger_option_toggle_fade[option_index],
                            trigger_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            row_controls_interactive && !gear_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int marker_alpha = static_cast<int>(150.0f * content_alpha);
                        if (gear_row)
                        {
                            const ImVec2 gear_pos(trigger_left_min.x + 224.0f, row_y - 3.0f);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            draw_combat_gear_button("trigger", popup_index, gear_pos, true, gear_hovered, gear_clicked);
                            handle_combat_checkbox_hit(
                                trigger_left_min,
                                row_y,
                                row_controls_interactive,
                                gear_hovered,
                                trigger_option_enabled[option_index],
                                trigger_option_hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            combat_popup_fade[popup_index] += ((combat_popup_open[popup_index] ? 1.0f : 0.0f) - combat_popup_fade[popup_index]) * popup_step;
                            if (combat_popup_fade[popup_index] < 0.01f)
                                combat_popup_fade[popup_index] = 0.0f;
                            if (combat_popup_fade[popup_index] > 0.0f)
                            {
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const float popup_eased = smooth_fade(combat_popup_fade[popup_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const bool keybind_only = popup_index == 5;
                                const ImVec2 popup_size =
                                    popup_index == 6 ? ImVec2(208.0f, 134.0f) :
                                    popup_index == 7 ? ImVec2(208.0f, 164.0f) :
                                    popup_index == 8 ? ImVec2(208.0f, 126.0f) :
                                    ImVec2(208.0f, 61.0f);
                                const ImVec2 popup_min(
                                    gear_center.x + (keybind_only ? -14.0f : 10.0f),
                                    gear_center.y + (keybind_only ? 21.0f : 10.0f) + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const bool popup_hovered = draw_combat_popup_shell("trigger", popup_index, popup_min, popup_size, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (popup_index == 5)
                                {
                                    ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                    draw_combat_popup_keybind("trigger", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
                                }
                                else if (popup_index == 6)
                                {
                                    draw_combat_popup_slider("trigger_magnet", 7, popup_min, popup_size.x, "Magnet FOV", 16.0f, 38.0f, 100.0f, "%.1fpx", popup_alpha_f);
                                    draw_combat_popup_slider("trigger_magnet", 8, popup_min, popup_size.x, "Magnet Speed", 60.0f, 82.0f, 100.0f, "%.0fpx", popup_alpha_f);
                                    ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 106.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                    draw_combat_popup_keybind("trigger", popup_index, popup_min, popup_size.x, 95.5f, popup_alpha_f);
                                }
                                else if (popup_index == 7)
                                {
                                    draw_combat_popup_slider("trigger_dynamic", 9, popup_min, popup_size.x, "Base FOV", 16.0f, 38.0f, 100.0f, "%.1fpx", popup_alpha_f);
                                    draw_combat_popup_slider("trigger_dynamic", 10, popup_min, popup_size.x, "Min FOV", 54.0f, 76.0f, 100.0f, "%.0fpx", popup_alpha_f);
                                    draw_combat_popup_slider("trigger_dynamic", 11, popup_min, popup_size.x, "Multiplier", 92.0f, 114.0f, 5.0f, "%.1f", popup_alpha_f);
                                    draw_combat_popup_toggle("trigger_dynamic", 0, popup_min, popup_size.x, "Show Dynamic FOV", 138.0f, popup_alpha_f);
                                }
                                else if (popup_index == 8)
                                {
                                    draw_combat_popup_slider("trigger_double", 12, popup_min, popup_size.x, "FOV Value", 16.0f, 38.0f, 100.0f, "%.0fpx", popup_alpha_f);
                                    draw_combat_popup_toggle("trigger_double", 1, popup_min, popup_size.x, "Toggle Mode", 72.0f, popup_alpha_f);
                                    draw_combat_popup_toggle("trigger_double", 2, popup_min, popup_size.x, "Show Double FOV", 104.0f, popup_alpha_f);
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (combat_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    combat_popup_open[popup_index] = false;
                                    combat_keybind_waiting[popup_index] = false;
                                }
                            }
                        }
                        else if (option_index == 7)
                        {
                            const ImVec2 fov_marker_center(trigger_left_min.x + 232.0f, row_y + 4.0f);
                            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(61, 62, 63, marker_alpha),
                                24);
                            ImGui::GetBackgroundDrawList()->AddCircle(
                                fov_marker_center,
                                5.0f,
                                IM_COL32(202, 202, 210, marker_alpha),
                                24,
                                1.5f);
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        trigger_right_min,
                        trigger_right_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(trigger_right_min, trigger_right_size, u8"\uf3dc", "Trigger Settings", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::slider(
                        "Field of View",
                        trigger_right_min,
                        trigger_right_min.y + 57.0f,
                        trigger_right_min.y + 79.0f,
                        trigger_fov_value,
                        trigger_fov_target_value,
                        trigger_fov_dragging,
                        100.0f,
                        "%.0fpx",
                        content_alpha,
                        content_interactive && !trigger_popup_active);
                    sigma::slider(
                        "Max Distance",
                        trigger_right_min,
                        trigger_right_min.y + 101.0f,
                        trigger_right_min.y + 123.0f,
                        trigger_max_distance_value,
                        trigger_max_distance_target_value,
                        trigger_max_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !trigger_popup_active);
                    sigma::slider(
                        "Min Distance",
                        trigger_right_min,
                        trigger_right_min.y + 145.0f,
                        trigger_right_min.y + 167.0f,
                        trigger_min_distance_value,
                        trigger_min_distance_target_value,
                        trigger_min_distance_dragging,
                        100.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !trigger_popup_active);
                    sigma::slider(
                        "Reaction Time",
                        trigger_right_min,
                        trigger_right_min.y + 189.0f,
                        trigger_right_min.y + 211.0f,
                        trigger_reaction_time_value,
                        trigger_reaction_time_target_value,
                        trigger_reaction_time_dragging,
                        500.0f,
                        "%.0fms",
                        content_alpha,
                        content_interactive && !trigger_popup_active);
                    sigma::slider(
                        "Delay Between Shots",
                        trigger_right_min,
                        trigger_right_min.y + 233.0f,
                        trigger_right_min.y + 255.0f,
                        trigger_delay_value,
                        trigger_delay_target_value,
                        trigger_delay_dragging,
                        500.0f,
                        "%.0fms",
                        content_alpha,
                        content_interactive && !trigger_popup_active);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* trigger_draw_list = ImGui::GetBackgroundDrawList();
                    const int trigger_alpha = static_cast<int>(255.0f * content_alpha);
                    const char* trigger_input_items[] = {
                        "Mouse",
                        "Memory"
                    };
                    const char* trigger_type_items[] = {
                        "Default",
                        "Head RayCast"
                    };
                    const char* trigger_bone_items[] = {
                        "Head",
                        "Neck",
                        "Chest",
                        "Stomach",
                        "Pelvis"
                    };
                    trigger_draw_list->AddText(title_font, 13.0f, ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 277.0f), IM_COL32(205, 205, 210, trigger_alpha), "Input Type");
                    draw_combat_dropdown(6, trigger_input_items, IM_ARRAYSIZE(trigger_input_items), ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 299.0f), false, 3, -1, content_interactive && !trigger_popup_active, false, ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f));
                    trigger_draw_list->AddText(title_font, 13.0f, ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 342.0f), IM_COL32(205, 205, 210, trigger_alpha), "TriggerBot Type");
                    draw_combat_dropdown(7, trigger_type_items, IM_ARRAYSIZE(trigger_type_items), ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 364.0f), false, 4, -1, content_interactive && !trigger_popup_active, false, ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f));
                    trigger_draw_list->AddText(title_font, 13.0f, ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 407.0f), IM_COL32(205, 205, 210, trigger_alpha), "Target Bone");
                    draw_combat_dropdown(8, trigger_bone_items, IM_ARRAYSIZE(trigger_bone_items), ImVec2(trigger_right_min.x + 13.0f, trigger_right_min.y + 429.0f), true, -1, 3, content_interactive && !trigger_popup_active, true, trigger_right_min, ImVec2(trigger_right_min.x + trigger_right_size.x, trigger_right_min.y + trigger_right_size.y));
                }
                else if (displayed_top_tab == 1 && render_sub_tab == 3)
                {
                    const ImVec2 flickbot_left_min(81.0f, 70.0f);
                    const ImVec2 flickbot_left_size(293.0f, 268.0f);
                    const ImVec2 flickbot_right_min(392.0f, 70.0f);
                    const ImVec2 flickbot_right_size(293.0f, 300.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        flickbot_left_min,
                        flickbot_left_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(flickbot_left_min, flickbot_left_size, u8"\uf219", "Flick Bot", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* flickbot_option_labels[] = {
                        "Enable",
                        "Draw Fov Circle",
                        "Only Visible",
                        "Ignore NPCs",
                        "Ignore Dead",
                        "Burst Mode",
                        "Flick Curve"
                    };
                    const bool flickbot_popup_active = combat_popup_active();
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(flickbot_option_labels); ++option_index)
                    {
                        const float row_y = flickbot_left_min.y + 57.0f + option_index * 29.0f;
                        const bool gear_row = option_index == 0;
                        const int popup_index = 9;
                        const bool row_controls_interactive = content_interactive && !flickbot_popup_active;
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##flickbot_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            flickbot_option_labels[option_index],
                            ImVec2(flickbot_left_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(flickbot_left_min.x + 13.0f, row_y),
                            ImVec2(flickbot_left_min.x + 249.0f, row_y - 2.0f),
                            flickbot_option_enabled[option_index],
                            flickbot_option_toggle_fade[option_index],
                            flickbot_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            row_controls_interactive && !gear_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const int marker_alpha = static_cast<int>(150.0f * content_alpha);
                        if (gear_row)
                        {
                            const ImVec2 flickbot_marker_center(flickbot_left_min.x + 235.0f, row_y + 4.0f);
                            const ImVec2 gear_pos(flickbot_marker_center.x - 6.0f, flickbot_marker_center.y - 7.0f);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            draw_combat_gear_button("flickbot", popup_index, gear_pos, true, gear_hovered, gear_clicked);
                            handle_combat_checkbox_hit(
                                flickbot_left_min,
                                row_y,
                                row_controls_interactive,
                                gear_hovered,
                                flickbot_option_enabled[option_index],
                                flickbot_option_hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            combat_popup_fade[popup_index] += ((combat_popup_open[popup_index] ? 1.0f : 0.0f) - combat_popup_fade[popup_index]) * popup_step;
                            if (combat_popup_fade[popup_index] < 0.01f)
                                combat_popup_fade[popup_index] = 0.0f;
                            if (combat_popup_fade[popup_index] > 0.0f)
                            {
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const float popup_eased = smooth_fade(combat_popup_fade[popup_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const ImVec2 popup_size(208.0f, 61.0f);
                                const ImVec2 popup_min(gear_center.x - 14.0f, gear_center.y + 21.0f + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const bool popup_hovered = draw_combat_popup_shell("flickbot", popup_index, popup_min, popup_size, popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                draw_combat_popup_keybind("flickbot", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (combat_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    combat_popup_open[popup_index] = false;
                                    combat_keybind_waiting[popup_index] = false;
                                }
                            }
                        }
                        else if (option_index == 1)
                        {
                            const ImVec2 flickbot_marker_center(flickbot_left_min.x + 236.0f, row_y + 4.0f);
                            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                                flickbot_marker_center,
                                5.0f,
                                IM_COL32(94, 72, 28, marker_alpha),
                                24);
                            ImGui::GetBackgroundDrawList()->AddCircle(
                                flickbot_marker_center,
                                5.0f,
                                IM_COL32(220, 170, 58, marker_alpha),
                                24,
                                1.5f);
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        flickbot_right_min,
                        flickbot_right_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(flickbot_right_min, flickbot_right_size, u8"\uf219", "Flick Bot Settings", content_alpha);
                    sigma::slider(
                        "Field of View",
                        flickbot_right_min,
                        flickbot_right_min.y + 57.0f,
                        flickbot_right_min.y + 79.0f,
                        flickbot_fov_value,
                        flickbot_fov_target_value,
                        flickbot_fov_dragging,
                        120.0f,
                        "%.0f",
                        content_alpha,
                        content_interactive && !flickbot_popup_active);
                    sigma::slider(
                        "Max Distance",
                        flickbot_right_min,
                        flickbot_right_min.y + 101.0f,
                        flickbot_right_min.y + 123.0f,
                        flickbot_distance_value,
                        flickbot_distance_target_value,
                        flickbot_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !flickbot_popup_active);
                    sigma::slider(
                        "Flick Speed",
                        flickbot_right_min,
                        flickbot_right_min.y + 145.0f,
                        flickbot_right_min.y + 167.0f,
                        flickbot_speed_value,
                        flickbot_speed_target_value,
                        flickbot_speed_dragging,
                        100.0f,
                        "%.0f%%",
                        content_alpha,
                        content_interactive && !flickbot_popup_active);
                    sigma::slider(
                        "Reached Threshold (px)",
                        flickbot_right_min,
                        flickbot_right_min.y + 189.0f,
                        flickbot_right_min.y + 211.0f,
                        flickbot_threshold_value,
                        flickbot_threshold_target_value,
                        flickbot_threshold_dragging,
                        50.0f,
                        "%.0f",
                        content_alpha,
                        content_interactive && !flickbot_popup_active);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* flickbot_draw_list = ImGui::GetBackgroundDrawList();
                    const int target_alpha = static_cast<int>(255.0f * content_alpha);
                    const char* flickbot_bone_items[] = {
                        "Head",
                        "Neck",
                        "Chest",
                        "Stomach",
                        "Pelvis"
                    };
                    flickbot_draw_list->AddText(
                        title_font,
                        13.0f,
                        ImVec2(flickbot_right_min.x + 13.0f, flickbot_right_min.y + 233.0f),
                        IM_COL32(205, 205, 210, target_alpha),
                        "Target Bone");
                    draw_combat_dropdown(
                        9,
                        flickbot_bone_items,
                        IM_ARRAYSIZE(flickbot_bone_items),
                        ImVec2(flickbot_right_min.x + 13.0f, flickbot_right_min.y + 255.0f),
                        true,
                        -1,
                        4,
                        content_interactive && !flickbot_popup_active,
                        false,
                        ImVec2(0.0f, 0.0f),
                        ImVec2(0.0f, 0.0f));
                }
                else if (displayed_top_tab == 1 && render_sub_tab == 4)
                {
                    const ImVec2 combat_custom_left_min(81.0f, 70.0f);
                    const ImVec2 combat_custom_left_size(293.0f, 252.0f);
                    const ImVec2 combat_custom_right_min(392.0f, 70.0f);
                    const float hitbox_preview_step = io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f;
                    hitbox_preview_expand_fade += ((hitbox_option_enabled[1] ? 1.0f : 0.0f) - hitbox_preview_expand_fade) * hitbox_preview_step;
                    if (hitbox_preview_expand_fade < 0.01f)
                        hitbox_preview_expand_fade = 0.0f;
                    if (hitbox_preview_expand_fade > 0.99f)
                        hitbox_preview_expand_fade = 1.0f;
                    const float hitbox_preview_eased = smooth_fade(hitbox_preview_expand_fade);
                    const float hitbox_preview_shift = 280.0f * hitbox_preview_eased;
                    const ImVec2 combat_custom_right_size(293.0f, 150.0f + hitbox_preview_shift);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        combat_custom_left_min,
                        combat_custom_left_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(combat_custom_left_min, combat_custom_left_size, u8"\uf3dc", "Hitbox Assistance", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* hitbox_option_labels[] = {
                        "Enable",
                        "Draw Preview"
                    };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(hitbox_option_labels); ++option_index)
                    {
                        const float row_y = combat_custom_left_min.y + 57.0f + option_index * 29.0f;
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##hitbox_assist_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            hitbox_option_labels[option_index],
                            ImVec2(combat_custom_left_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(combat_custom_left_min.x + 13.0f, row_y),
                            ImVec2(combat_custom_left_min.x + 249.0f, row_y - 2.0f),
                            hitbox_option_enabled[option_index],
                            hitbox_option_toggle_fade[option_index],
                            hitbox_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::slider(
                        "Height",
                        combat_custom_left_min,
                        combat_custom_left_min.y + 110.0f,
                        combat_custom_left_min.y + 132.0f,
                        hitbox_height_value,
                        hitbox_height_target_value,
                        hitbox_height_dragging,
                        1.0f,
                        "%.2fpx",
                        content_alpha,
                        content_interactive);
                    sigma::slider(
                        "Width",
                        combat_custom_left_min,
                        combat_custom_left_min.y + 156.0f,
                        combat_custom_left_min.y + 178.0f,
                        hitbox_width_value,
                        hitbox_width_target_value,
                        hitbox_width_dragging,
                        1.0f,
                        "%.2fpx",
                        content_alpha,
                        content_interactive);
                    if (sigma::button(
                        "Quick presets",
                        ImVec2(combat_custom_left_min.x + 13.0f, combat_custom_left_min.y + 202.0f),
                        ImVec2(265.0f, 33.0f),
                        hitbox_button_hover_fade[0],
                        12.0f,
                        content_alpha,
                        content_interactive))
                    {
                        quick_presets_overlay_fade = 0.0f;
                        quick_presets_overlay_open = true;
                        quick_presets_overlay_click_lock = true;
                        g_BlurCacheValid = false;
                        g_BlurCaptureRequested = true;
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        combat_custom_right_min,
                        combat_custom_right_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(combat_custom_right_min, combat_custom_right_size, u8"\uf0f0", "Hitbox Settings", content_alpha);
                    if (hitbox_preview_eased > 0.005f)
                    {
                        ImDrawList* preview_draw_list = ImGui::GetBackgroundDrawList();
                        const float preview_alpha_f = content_alpha * hitbox_preview_eased;
                        const int preview_alpha = static_cast<int>(255.0f * preview_alpha_f);
                        const ImVec2 preview_min(combat_custom_right_min.x + 13.0f, combat_custom_right_min.y + 57.0f);
                        const ImVec2 preview_size(265.0f, 260.0f);
                        const ImVec2 preview_max(preview_min.x + preview_size.x, preview_min.y + preview_size.y);
                        preview_draw_list->PushClipRect(combat_custom_right_min, ImVec2(combat_custom_right_min.x + combat_custom_right_size.x, combat_custom_right_min.y + combat_custom_right_size.y), true);
                        preview_draw_list->AddRectFilled(
                            preview_min,
                            preview_max,
                            IM_COL32(17, 20, 25, static_cast<int>(238.0f * preview_alpha_f)),
                            4.0f);
                        preview_draw_list->AddRect(
                            preview_min,
                            preview_max,
                            IM_COL32(42, 47, 58, static_cast<int>(190.0f * preview_alpha_f)),
                            4.0f);
                        const ImVec2 head_center(preview_min.x + preview_size.x * 0.5f, preview_min.y + 103.0f);
                        const float hitbox_radius_x = 29.0f + hitbox_width_value * 16.0f;
                        const float hitbox_radius_y = 31.0f + hitbox_height_value * 16.0f;
                        const ImU32 muted_line = IM_COL32(75, 83, 96, static_cast<int>(190.0f * preview_alpha_f));
                        const ImU32 accent_line = IM_COL32(218, 94, 84, static_cast<int>(220.0f * preview_alpha_f));
                        preview_draw_list->AddRectFilled(
                            ImVec2(head_center.x - 34.0f, head_center.y - 20.0f),
                            ImVec2(head_center.x - 24.0f, head_center.y + 20.0f),
                            IM_COL32(58, 66, 78, static_cast<int>(145.0f * preview_alpha_f)),
                            11.0f);
                        preview_draw_list->AddRectFilled(
                            ImVec2(head_center.x + 24.0f, head_center.y - 20.0f),
                            ImVec2(head_center.x + 34.0f, head_center.y + 20.0f),
                            IM_COL32(58, 66, 78, static_cast<int>(145.0f * preview_alpha_f)),
                            11.0f);
                        preview_draw_list->AddCircleFilled(head_center, 27.0f, IM_COL32(62, 70, 82, static_cast<int>(170.0f * preview_alpha_f)), 48);
                        preview_draw_list->AddCircle(head_center, 27.0f, accent_line, 48, 1.4f);
                        preview_draw_list->AddEllipse(head_center, ImVec2(hitbox_radius_x, hitbox_radius_y), accent_line, 0.0f, 48, 1.3f);
                        preview_draw_list->AddLine(ImVec2(head_center.x - hitbox_radius_x, head_center.y), ImVec2(head_center.x + hitbox_radius_x, head_center.y), muted_line, 1.0f);
                        preview_draw_list->AddLine(ImVec2(head_center.x, head_center.y - hitbox_radius_y), ImVec2(head_center.x, head_center.y + hitbox_radius_y), muted_line, 1.0f);
                        preview_draw_list->AddCircleFilled(ImVec2(head_center.x - 11.0f, head_center.y - 6.0f), 3.0f, IM_COL32(12, 18, 27, preview_alpha), 16);
                        preview_draw_list->AddCircle(ImVec2(head_center.x - 11.0f, head_center.y - 6.0f), 5.8f, muted_line, 16, 1.1f);
                        preview_draw_list->AddCircleFilled(ImVec2(head_center.x + 11.0f, head_center.y - 6.0f), 3.0f, IM_COL32(12, 18, 27, preview_alpha), 16);
                        preview_draw_list->AddCircle(ImVec2(head_center.x + 11.0f, head_center.y - 6.0f), 5.8f, muted_line, 16, 1.1f);
                        preview_draw_list->AddCircleFilled(head_center, 3.0f, IM_COL32(224, 92, 82, preview_alpha), 16);
                        preview_draw_list->AddRectFilled(ImVec2(head_center.x - 9.0f, head_center.y + 28.0f), ImVec2(head_center.x + 9.0f, head_center.y + 60.0f), IM_COL32(70, 78, 88, static_cast<int>(140.0f * preview_alpha_f)), 2.0f);
                        preview_draw_list->AddLine(ImVec2(head_center.x - 9.0f, head_center.y + 28.0f), ImVec2(head_center.x + 9.0f, head_center.y + 28.0f), muted_line, 1.0f);
                        preview_draw_list->AddTriangleFilled(
                            ImVec2(head_center.x, head_center.y + 52.0f),
                            ImVec2(head_center.x - 62.0f, head_center.y + 81.0f),
                            ImVec2(head_center.x + 62.0f, head_center.y + 81.0f),
                            IM_COL32(53, 60, 70, static_cast<int>(145.0f * preview_alpha_f)));
                        preview_draw_list->AddTriangle(
                            ImVec2(head_center.x, head_center.y + 52.0f),
                            ImVec2(head_center.x - 62.0f, head_center.y + 81.0f),
                            ImVec2(head_center.x + 62.0f, head_center.y + 81.0f),
                            muted_line,
                            1.2f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        char width_text[24];
                        char height_text[24];
                        ImFormatString(width_text, IM_ARRAYSIZE(width_text), "W: %.2f", hitbox_width_value);
                        ImFormatString(height_text, IM_ARRAYSIZE(height_text), "H: %.2f", hitbox_height_value);
                        const float stat_text_size = 11.0f;
                        const ImVec2 stat_box_min(preview_min.x + 18.0f, preview_max.y - 58.0f);
                        const ImVec2 stat_box_max(preview_max.x - 18.0f, preview_max.y - 14.0f);
                        preview_draw_list->AddRectFilled(
                            stat_box_min,
                            stat_box_max,
                            IM_COL32(20, 24, 30, static_cast<int>(220.0f * preview_alpha_f)),
                            2.0f);
                        const float stat_label_x = stat_box_min.x + 14.0f;
                        const float stat_value_right = stat_box_max.x - 14.0f;
                        const float stat_width_y = stat_box_min.y + 9.0f;
                        const float stat_height_y = stat_box_min.y + 26.0f;
                        preview_draw_list->AddText(title_font, stat_text_size, ImVec2(stat_label_x, stat_width_y), IM_COL32(155, 161, 175, preview_alpha), "WIDTH");
                        preview_draw_list->AddText(title_font, stat_text_size, ImVec2(stat_label_x, stat_height_y), IM_COL32(155, 161, 175, preview_alpha), "HEIGHT");
                        const ImVec2 width_value_size = title_font->CalcTextSizeA(stat_text_size, 1000.0f, 0.0f, width_text);
                        const ImVec2 height_value_size = title_font->CalcTextSizeA(stat_text_size, 1000.0f, 0.0f, height_text);
                        preview_draw_list->AddText(title_font, stat_text_size, ImVec2(stat_value_right - width_value_size.x, stat_width_y), IM_COL32(234, 111, 100, preview_alpha), width_text);
                        preview_draw_list->AddText(title_font, stat_text_size, ImVec2(stat_value_right - height_value_size.x, stat_height_y), IM_COL32(234, 111, 100, preview_alpha), height_text);
                        preview_draw_list->PopClipRect();
                    }
                    if (sigma::button(
                        "Reset to Default",
                        ImVec2(combat_custom_right_min.x + 13.0f, combat_custom_right_min.y + 57.0f + hitbox_preview_shift),
                        ImVec2(265.0f, 33.0f),
                        hitbox_button_hover_fade[1],
                        12.0f,
                        content_alpha,
                        content_interactive))
                    {
                        hitbox_option_enabled[0] = false;
                        hitbox_option_enabled[1] = false;
                        hitbox_height_target_value = 0.10f;
                        hitbox_width_target_value = 0.13f;
                    }
                    sigma::button(
                        "Apply",
                        ImVec2(combat_custom_right_min.x + 13.0f, combat_custom_right_min.y + 104.0f + hitbox_preview_shift),
                        ImVec2(265.0f, 33.0f),
                        hitbox_button_hover_fade[2],
                        12.0f,
                        content_alpha,
                        content_interactive);
                }
                else if (displayed_top_tab == 2 && render_sub_tab == 0)
                {
                    const ImVec2 player_visuals_min(81.0f, 70.0f);
                    const ImVec2 player_visuals_size(293.0f, 466.0f);
                    const ImVec2 player_settings_min(392.0f, 70.0f);
                    const ImVec2 player_settings_size(293.0f, 335.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_player_checkbox_rows = [&](
                        const ImVec2& child_min,
                        const char* id_prefix,
                        const char* const* labels,
                        int label_count,
                        bool* enabled,
                        float* toggle_fade,
                        float* hover_fade,
                        const bool* show_marker,
                        const bool* show_ring,
                        bool enable_popups)
                    {
                        for (int option_index = 0; option_index < label_count; ++option_index)
                        {
                            const float row_y = child_min.y + 57.0f + option_index * 29.0f;
                            int popup_index = -1;
                            if (enable_popups)
                            {
                                if (option_index == 0)
                                    popup_index = 0;
                                else if (option_index == 1)
                                    popup_index = 1;
                                else if (option_index == 2)
                                    popup_index = 2;
                                else if (option_index == 3)
                                    popup_index = 3;
                                else if (option_index == 10)
                                    popup_index = 4;
                                else if (option_index == 13)
                                    popup_index = 5;
                            }
                            const bool popup_row = popup_index >= 0;
                            const bool any_visual_popup_active = visual_popup_active();
                            const bool this_popup_active =
                                popup_row &&
                                (visual_popup_open[popup_index] || visual_popup_fade[popup_index] > 0.005f);
                            const bool row_controls_interactive = content_interactive && (!any_visual_popup_active || this_popup_active);
                            char option_id[64];
                            ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##%s_%d", id_prefix, option_index);
                            sigma::checkbox(
                                option_id,
                                labels[option_index],
                                ImVec2(child_min.x + 16.0f, row_y - 9.0f),
                                ImVec2(267.0f, 29.0f),
                                ImVec2(child_min.x + 13.0f, row_y),
                                ImVec2(child_min.x + 249.0f, row_y - 2.0f),
                                enabled[option_index],
                                toggle_fade[option_index],
                                hover_fade[option_index],
                                13.0f,
                                false,
                                content_alpha,
                                row_controls_interactive && !popup_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const int marker_alpha = static_cast<int>(150.0f * content_alpha);
                            if (show_marker && show_marker[option_index])
                            {
                                const ImVec2 gear_pos(child_min.x + 224.0f, row_y - 3.0f);
                                if (popup_row)
                                {
                                    const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                    const ImVec2 gear_hit_size(24.0f, 24.0f);
                                    const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                                    const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                                    bool gear_hovered = false;
                                    bool gear_clicked = false;
                                    if (content_interactive)
                                    {
                                        gear_hovered =
                                            io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                            io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                        gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                        if (gear_hovered)
                                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                    }
                                    else
                                    {
                                        visual_popup_open[popup_index] = false;
                                        visual_keybind_waiting[popup_index] = false;
                                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    if (gear_clicked)
                                    {
                                        const bool next_open = !visual_popup_open[popup_index];
                                        close_visual_popups(popup_index);
                                        visual_popup_open[popup_index] = next_open;
                                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const ImVec2 label_hit_min(child_min.x + 16.0f, row_y - 9.0f);
                                    const ImVec2 label_hit_max(child_min.x + 214.0f, row_y + 20.0f);
                                    const ImVec2 toggle_hit_min(child_min.x + 246.0f, row_y - 8.0f);
                                    const ImVec2 toggle_hit_max(child_min.x + 282.0f, row_y + 19.0f);
                                    const bool checkbox_hovered =
                                        row_controls_interactive &&
                                        !gear_hovered &&
                                        ((io.MousePos.x >= label_hit_min.x && io.MousePos.x <= label_hit_max.x &&
                                          io.MousePos.y >= label_hit_min.y && io.MousePos.y <= label_hit_max.y) ||
                                         (io.MousePos.x >= toggle_hit_min.x && io.MousePos.x <= toggle_hit_max.x &&
                                          io.MousePos.y >= toggle_hit_min.y && io.MousePos.y <= toggle_hit_max.y));
                                    if (checkbox_hovered)
                                    {
                                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                            enabled[option_index] = !enabled[option_index];
                                    }
                                    const float checkbox_hover_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                    hover_fade[option_index] += ((checkbox_hovered ? 1.0f : 0.0f) - hover_fade[option_index]) * checkbox_hover_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                    visual_gear_hover_fade[popup_index] += (((gear_hovered || visual_popup_open[popup_index]) ? 1.0f : 0.0f) - visual_gear_hover_fade[popup_index]) * gear_step;
                                    const float gear_hover = smooth_fade(visual_gear_hover_fade[popup_index]);
                                    const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                    const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                    const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                                    const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                                    ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                                    visual_popup_fade[popup_index] += ((visual_popup_open[popup_index] ? 1.0f : 0.0f) - visual_popup_fade[popup_index]) * popup_step;
                                    if (visual_popup_fade[popup_index] < 0.01f)
                                        visual_popup_fade[popup_index] = 0.0f;
                                    if (visual_popup_fade[popup_index] > 0.0f)
                                    {
                                        const float popup_eased = smooth_fade(visual_popup_fade[popup_index]);
                                        const float popup_alpha_f = content_alpha * popup_eased;
                                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                        const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                        const ImVec2 popup_size =
                                            popup_index == 1 ? ImVec2(248.0f, 214.0f) :
                                            popup_index == 3 ? ImVec2(212.0f, 246.0f) :
                                            popup_index == 4 ? ImVec2(212.0f, 82.0f) :
                                            popup_index == 5 ? ImVec2(212.0f, 206.0f) :
                                            ImVec2(208.0f, 61.0f);
                                        const ImVec2 popup_min(gear_center.x + 10.0f, gear_center.y + 10.0f + popup_slide_y);
                                        const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                        const bool popup_hovered = draw_combat_popup_shell("player_visual", popup_index, popup_min, popup_size, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                        if (popup_index == 0 || popup_index == 2)
                                        {
                                            ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                            draw_visual_popup_keybind("player_visual", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
                                        }
                                        else if (popup_index == 1)
                                        {
                                            draw_visual_popup_toggle("player_box", 0, popup_min, popup_size.x, "Outline", 22.0f, popup_alpha_f);
                                            draw_visual_popup_toggle("player_box", 1, popup_min, popup_size.x, "Filled", 52.0f, popup_alpha_f);
                                            draw_visual_popup_select(popup_min, popup_size.x, "Box Type", "Normal Box", 92.0f, 118.0f, popup_alpha_f);
                                            draw_visual_popup_slider("player_box", 0, popup_min, popup_size.x, "Rounding Box", 166.0f, 190.0f, 10.0f, "%.1f", popup_alpha_f);
                                        }
                                        else if (popup_index == 3)
                                        {
                                            ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                                            const char* skeleton_color_labels[] = { "Visible", "Invisible", "Friend", "Godmode" };
                                            const ImU32 skeleton_colors[] = {
                                                IM_COL32(202, 202, 210, popup_alpha),
                                                IM_COL32(128, 10, 10, popup_alpha),
                                                IM_COL32(181, 145, 20, popup_alpha),
                                                IM_COL32(12, 82, 160, popup_alpha)
                                            };
                                            for (int color_index = 0; color_index < 4; ++color_index)
                                            {
                                                const float y = popup_min.y + 20.0f + color_index * 30.0f;
                                                popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, y), IM_COL32(205, 205, 210, popup_alpha), skeleton_color_labels[color_index]);
                                                popup_draw_list->AddCircle(ImVec2(popup_max.x - 23.0f, y + 5.0f), 7.8f, skeleton_colors[color_index], 24, 1.5f);
                                            }
                                            draw_visual_popup_select(popup_min, popup_size.x, "Skeleton Type", "Normal", 140.0f, 162.0f, popup_alpha_f);
                                            draw_visual_popup_slider("player_skeleton", 1, popup_min, popup_size.x, "Skeleton Thickness", 204.0f, 226.0f, 4.0f, "%.1f", popup_alpha_f);
                                        }
                                        else if (popup_index == 4)
                                        {
                                            draw_visual_popup_select(popup_min, popup_size.x, "Chams Type", "White", 18.0f, 40.0f, popup_alpha_f);
                                        }
                                        else if (popup_index == 5)
                                        {
                                            draw_visual_popup_toggle("player_look", 2, popup_min, popup_size.x, "Show Name", 18.0f, popup_alpha_f);
                                            draw_visual_popup_toggle("player_look", 3, popup_min, popup_size.x, "Show Distance", 48.0f, popup_alpha_f);
                                            draw_visual_popup_toggle("player_look", 4, popup_min, popup_size.x, "Show Percentage", 78.0f, popup_alpha_f);
                                            draw_visual_popup_slider("player_look", 2, popup_min, popup_size.x, "Sensitivity", 112.0f, 134.0f, 100.0f, "%.0f", popup_alpha_f);
                                            draw_visual_popup_slider("player_look", 3, popup_min, popup_size.x, "Detection Range", 166.0f, 188.0f, 300.0f, "%.0fm", popup_alpha_f);
                                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                        if (visual_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                        {
                                            visual_popup_open[popup_index] = false;
                                            visual_keybind_waiting[popup_index] = false;
                                        }
                                    }
                                }
                                else
                                {
                                    ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(118, 118, 124, marker_alpha), u8"\uf3dc");
                                }
                            }
                            if (show_ring && show_ring[option_index])
                            {
                                ImGui::GetBackgroundDrawList()->AddCircleFilled(
                                    ImVec2(child_min.x + 233.0f, row_y + 4.5f),
                                    4.4f,
                                    IM_COL32(61, 61, 61, marker_alpha),
                                    24);
                                ImGui::GetBackgroundDrawList()->AddCircle(
                                    ImVec2(child_min.x + 233.0f, row_y + 4.5f),
                                    5.2f,
                                    IM_COL32(255, 255, 255, marker_alpha),
                                    24,
                                    1.4f);
                            }
                        }
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(player_visuals_min, player_visuals_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(player_visuals_min, player_visuals_size, u8"\uf3dc", "Visuals Assistance", content_alpha);
                    const char* player_visual_labels[] = {
                        "Toggle",
                        "Box",
                        "Lines",
                        "Skeleton",
                        "Health Bar",
                        "Armor Bar",
                        "Player ID",
                        "Player Names",
                        "Weapon Names",
                        "Distance",
                        "Chams",
                        "Hat",
                        "OOF Indicators",
                        "Looking At Me"
                    };
                    const bool player_visual_markers[] = {
                        true, true, true, true, false, false, false, false, false, false, true, false, false, true
                    };
                    const bool player_visual_rings[] = {
                        false, false, false, false, false, false, false, false, false, false, false, true, true, false
                    };
                    draw_player_checkbox_rows(
                        player_visuals_min,
                        "player_visual",
                        player_visual_labels,
                        IM_ARRAYSIZE(player_visual_labels),
                        player_visuals_enabled,
                        player_visuals_toggle_fade,
                        player_visuals_hover_fade,
                        player_visual_markers,
                        player_visual_rings,
                        true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(player_settings_min, player_settings_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(player_settings_min, player_settings_size, u8"\uf0f0", "Visuals Settings", content_alpha);
                    const char* player_setting_labels[] = {
                        "Show Players",
                        "Show Friends",
                        "Show Peds",
                        "Show Animals",
                        "Show Invisible",
                        "Show Visible Only",
                        "Show Dead",
                        "Show Self"
                    };
                    draw_player_checkbox_rows(
                        player_settings_min,
                        "player_setting",
                        player_setting_labels,
                        IM_ARRAYSIZE(player_setting_labels),
                        player_settings_enabled,
                        player_settings_toggle_fade,
                        player_settings_hover_fade,
                        nullptr,
                        nullptr,
                        false);
                    sigma::slider(
                        "Max Distance",
                        player_settings_min,
                        player_settings_min.y + 284.0f,
                        player_settings_min.y + 310.0f,
                        player_max_distance_value,
                        player_max_distance_target_value,
                        player_max_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !visual_popup_active());
                    const bool player_visual_popup_region_needed = visual_popup_active();
                    const bool player_bottom_region_needed = g_BottomProfilePopupRegionActive || player_visual_popup_region_needed;
                    if (g_BottomOptionPopupRegionActive != player_visual_popup_region_needed || g_BottomPopupRegionActive != player_bottom_region_needed || g_RightPopupRegionActive != player_visual_popup_region_needed)
                    {
                        g_BottomOptionPopupRegionActive = player_visual_popup_region_needed;
                        g_BottomPopupRegionActive = player_bottom_region_needed;
                        g_RightPopupRegionActive = player_visual_popup_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                }
                else if (displayed_top_tab == 2 && render_sub_tab == 1)
                {
                    const ImVec2 vehicle_visuals_min(81.0f, 70.0f);
                    const ImVec2 vehicle_visuals_size(293.0f, 238.0f);
                    const ImVec2 vehicle_esp_min(392.0f, 70.0f);
                    const ImVec2 vehicle_esp_size(293.0f, 191.0f);
                    const ImVec2 vehicle_health_min(81.0f, 322.0f);
                    const ImVec2 vehicle_health_size(293.0f, 122.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_vehicle_checkbox = [&](
                        const ImVec2& child_min,
                        const char* id_prefix,
                        const char* const* labels,
                        int label_count,
                        bool* enabled,
                        float* toggle_fade,
                        float* hover_fade,
                        const bool* show_marker,
                        int popup_base_index)
                    {
                        for (int option_index = 0; option_index < label_count; ++option_index)
                        {
                            const float row_y = child_min.y + 57.0f + option_index * 29.0f;
                            const bool popup_row = show_marker && show_marker[option_index] && popup_base_index >= 0;
                            const int popup_index = popup_row ? popup_base_index + option_index : -1;
                            const bool any_visual_popup_active = visual_popup_active();
                            const bool this_popup_active =
                                popup_row &&
                                (visual_popup_open[popup_index] || visual_popup_fade[popup_index] > 0.005f);
                            const bool row_controls_interactive = content_interactive && (!any_visual_popup_active || this_popup_active);
                            char option_id[64];
                            ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##%s_%d", id_prefix, option_index);
                            sigma::checkbox(
                                option_id,
                                labels[option_index],
                                ImVec2(child_min.x + 16.0f, row_y - 9.0f),
                                ImVec2(267.0f, 29.0f),
                                ImVec2(child_min.x + 13.0f, row_y),
                                ImVec2(child_min.x + 249.0f, row_y - 2.0f),
                                enabled[option_index],
                                toggle_fade[option_index],
                                hover_fade[option_index],
                                13.0f,
                                false,
                                content_alpha,
                                row_controls_interactive && !popup_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            if (show_marker && show_marker[option_index])
                            {
                                const ImVec2 gear_pos(child_min.x + 224.0f, row_y - 3.0f);
                                if (popup_row && popup_index >= 0 && popup_index < IM_ARRAYSIZE(visual_popup_open))
                                {
                                    const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                    const ImVec2 gear_hit_size(24.0f, 24.0f);
                                    const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                                    const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                                    bool gear_hovered = false;
                                    bool gear_clicked = false;
                                    if (content_interactive)
                                    {
                                        gear_hovered =
                                            io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                            io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                        gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                        if (gear_hovered)
                                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                    }
                                    else
                                    {
                                        visual_popup_open[popup_index] = false;
                                        visual_keybind_waiting[popup_index] = false;
                                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    if (gear_clicked)
                                    {
                                        const bool next_open = !visual_popup_open[popup_index];
                                        close_visual_popups(popup_index);
                                        visual_popup_open[popup_index] = next_open;
                                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    handle_combat_checkbox_hit(child_min, row_y, row_controls_interactive, gear_hovered, enabled[option_index], hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                    visual_gear_hover_fade[popup_index] += (((gear_hovered || visual_popup_open[popup_index]) ? 1.0f : 0.0f) - visual_gear_hover_fade[popup_index]) * gear_step;
                                    const float gear_hover = smooth_fade(visual_gear_hover_fade[popup_index]);
                                    const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                    const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                    const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                                    const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                                    ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                                    visual_popup_fade[popup_index] += ((visual_popup_open[popup_index] ? 1.0f : 0.0f) - visual_popup_fade[popup_index]) * popup_step;
                                    if (visual_popup_fade[popup_index] < 0.01f)
                                        visual_popup_fade[popup_index] = 0.0f;
                                    if (visual_popup_fade[popup_index] > 0.0f)
                                    {
                                        const float popup_eased = smooth_fade(visual_popup_fade[popup_index]);
                                        const float popup_alpha_f = content_alpha * popup_eased;
                                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                        const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                        const ImVec2 popup_size(popup_index == 6 ? 208.0f : 208.0f, popup_index == 6 ? 61.0f : 82.0f);
                                        const ImVec2 popup_min(gear_center.x + 10.0f, gear_center.y + 10.0f + popup_slide_y);
                                        const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                        const bool popup_hovered = draw_combat_popup_shell("vehicle_visual", popup_index, popup_min, popup_size, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                        if (popup_index == 6)
                                        {
                                            ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "KeyBind");
                                            draw_visual_popup_keybind("vehicle_visual", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
                                        }
                                        else
                                        {
                                            draw_visual_popup_select(popup_min, popup_size.x, "Type", "Text, Bar", 18.0f, 40.0f, popup_alpha_f);
                                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                        if (visual_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                        {
                                            visual_popup_open[popup_index] = false;
                                            visual_keybind_waiting[popup_index] = false;
                                        }
                                    }
                                }
                                else
                                {
                                    ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(118, 118, 124, static_cast<int>(150.0f * content_alpha)), u8"\uf3dc");
                                }
                            }
                        }
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(vehicle_visuals_min, vehicle_visuals_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_visuals_min, vehicle_visuals_size, u8"\uf3dc", "Vehicle Visuals", content_alpha);
                    const char* vehicle_visual_labels[] = {
                        "Enable",
                        "Car Snapline",
                        "Show Model",
                        "Show Lock Status",
                        "Show Distance",
                        "Show Speed"
                    };
                    const bool vehicle_visual_markers[] = { true, false, false, false, false, false };
                    draw_vehicle_checkbox(
                        vehicle_visuals_min,
                        "vehicle_visual",
                        vehicle_visual_labels,
                        IM_ARRAYSIZE(vehicle_visual_labels),
                        vehicle_visuals_enabled,
                        vehicle_visuals_toggle_fade,
                        vehicle_visuals_hover_fade,
                        vehicle_visual_markers,
                        6);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(vehicle_esp_min, vehicle_esp_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_esp_min, vehicle_esp_size, u8"\uf0f0", "Vehicle ESP Settings", content_alpha);
                    const char* vehicle_esp_labels[] = {
                        "Empty Only",
                        "Ignore Empty",
                        "Ignore Current Vehicle"
                    };
                    draw_vehicle_checkbox(
                        vehicle_esp_min,
                        "vehicle_esp",
                        vehicle_esp_labels,
                        IM_ARRAYSIZE(vehicle_esp_labels),
                        vehicle_esp_enabled,
                        vehicle_esp_toggle_fade,
                        vehicle_esp_hover_fade,
                        nullptr,
                        -1);
                    sigma::slider(
                        "Distance",
                        vehicle_esp_min,
                        vehicle_esp_min.y + 142.0f,
                        vehicle_esp_min.y + 163.0f,
                        vehicle_distance_value,
                        vehicle_distance_target_value,
                        vehicle_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !visual_popup_active());
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(vehicle_health_min, vehicle_health_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_health_min, vehicle_health_size, u8"\uf3dc", "Vehicle Health", content_alpha);
                    const char* vehicle_health_labels[] = {
                        "Show Body Health",
                        "Show Engine Health"
                    };
                    const bool vehicle_health_markers[] = { true, true };
                    draw_vehicle_checkbox(
                        vehicle_health_min,
                        "vehicle_health",
                        vehicle_health_labels,
                        IM_ARRAYSIZE(vehicle_health_labels),
                        vehicle_health_enabled,
                        vehicle_health_toggle_fade,
                        vehicle_health_hover_fade,
                        vehicle_health_markers,
                        7);
                    const bool vehicle_visual_popup_region_needed = visual_popup_active();
                    const bool vehicle_bottom_region_needed = g_BottomProfilePopupRegionActive || vehicle_visual_popup_region_needed;
                    if (g_BottomOptionPopupRegionActive != vehicle_visual_popup_region_needed || g_BottomPopupRegionActive != vehicle_bottom_region_needed || g_RightPopupRegionActive != vehicle_visual_popup_region_needed)
                    {
                        g_BottomOptionPopupRegionActive = vehicle_visual_popup_region_needed;
                        g_BottomPopupRegionActive = vehicle_bottom_region_needed;
                        g_RightPopupRegionActive = vehicle_visual_popup_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
                }
                else if (displayed_top_tab == 2 && render_sub_tab == 2)
                {
                    const ImVec2 object_visuals_min(81.0f, 70.0f);
                    const ImVec2 object_visuals_size(293.0f, 258.0f);
                    const ImVec2 object_list_min(392.0f, 70.0f);
                    const ImVec2 object_list_size(293.0f, 466.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        object_visuals_min,
                        object_visuals_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(object_visuals_min, object_visuals_size, u8"\uf3dc", "Object Visuals", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* object_visual_labels[] = {
                        "Toggle",
                        "Hash",
                        "Marker",
                        "Lines",
                        "Saved Only"
                    };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(object_visual_labels); ++option_index)
                    {
                        const float row_y = object_visuals_min.y + 57.0f + option_index * 29.0f;
                        const bool popup_row = option_index == 0;
                        const int popup_index = 9;
                        const bool any_visual_popup_active = visual_popup_active();
                        const bool this_popup_active = popup_row && (visual_popup_open[popup_index] || visual_popup_fade[popup_index] > 0.005f);
                        const bool row_controls_interactive = content_interactive && (!any_visual_popup_active || this_popup_active);
                        char option_id[48];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##object_visual_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            object_visual_labels[option_index],
                            ImVec2(object_visuals_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(object_visuals_min.x + 13.0f, row_y),
                            ImVec2(object_visuals_min.x + 249.0f, row_y - 2.0f),
                            object_visuals_enabled[option_index],
                            object_visuals_toggle_fade[option_index],
                            object_visuals_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha,
                            row_controls_interactive && !popup_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        if (option_index == 0)
                        {
                            const ImVec2 gear_pos(object_visuals_min.x + 224.0f, row_y - 3.0f);
                            const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                            const ImVec2 gear_hit_size(24.0f, 24.0f);
                            const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                            const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                            bool gear_hovered = false;
                            bool gear_clicked = false;
                            if (content_interactive)
                            {
                                gear_hovered =
                                    io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                    io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                if (gear_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            }
                            else
                            {
                                visual_popup_open[popup_index] = false;
                                visual_keybind_waiting[popup_index] = false;
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            if (gear_clicked)
                            {
                                const bool next_open = !visual_popup_open[popup_index];
                                close_visual_popups(popup_index);
                                visual_popup_open[popup_index] = next_open;
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            handle_combat_checkbox_hit(
                                object_visuals_min,
                                row_y,
                                row_controls_interactive,
                                gear_hovered,
                                object_visuals_enabled[option_index],
                                object_visuals_hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                            visual_gear_hover_fade[popup_index] += (((gear_hovered || visual_popup_open[popup_index]) ? 1.0f : 0.0f) - visual_gear_hover_fade[popup_index]) * gear_step;
                            const float gear_hover = smooth_fade(visual_gear_hover_fade[popup_index]);
                            const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                            const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                            const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                            const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                            ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                            visual_popup_fade[popup_index] += ((visual_popup_open[popup_index] ? 1.0f : 0.0f) - visual_popup_fade[popup_index]) * popup_step;
                            if (visual_popup_fade[popup_index] < 0.01f)
                                visual_popup_fade[popup_index] = 0.0f;
                            if (visual_popup_fade[popup_index] > 0.0f)
                            {
                                const float popup_eased = smooth_fade(visual_popup_fade[popup_index]);
                                const float popup_alpha_f = content_alpha * popup_eased;
                                const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                const ImVec2 popup_size(208.0f, 61.0f);
                                const ImVec2 popup_min(gear_center.x + 10.0f, gear_center.y + 10.0f + popup_slide_y);
                                const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                const bool popup_hovered = draw_combat_popup_shell("object_visual", popup_index, popup_min, popup_size, popup_alpha_f);
                                ImGui::GetForegroundDrawList()->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 25.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                draw_visual_popup_keybind("object_visual", popup_index, popup_min, popup_size.x, 14.5f, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (visual_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                {
                                    visual_popup_open[popup_index] = false;
                                    visual_keybind_waiting[popup_index] = false;
                                }
                            }
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::slider(
                        "Max Distance",
                        object_visuals_min,
                        object_visuals_min.y + 204.0f,
                        object_visuals_min.y + 230.0f,
                        object_max_distance_value,
                        object_max_distance_target_value,
                        object_max_distance_dragging,
                        500.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive && !visual_popup_active());
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        object_list_min,
                        object_list_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(object_list_min, object_list_size, u8"\uf0f0", "Object List", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 object_button_size(126.0f, 33.0f);
                    const float object_button_gap = 13.0f;
                    const float object_buttons_total_width = object_button_size.x * 2.0f + object_button_gap;
                    const ImVec2 add_object_button_min(
                        object_list_min.x + (object_list_size.x - object_buttons_total_width) * 0.5f,
                        object_list_min.y + 57.0f);
                    if (sigma::button(
                        "Add Object",
                        u8"\uf317",
                        add_object_button_min,
                        object_button_size,
                        object_list_button_hover_fade[0],
                        12.0f,
                        13.0f,
                        content_alpha,
                        content_interactive && !visual_popup_active()))
                    {
                        object_add_hash_input[0] = '\0';
                        object_add_name_input[0] = '\0';
                        object_add_overlay_fade = 0.0f;
                        object_add_overlay_open = true;
                        object_add_overlay_click_lock = true;
                        g_BlurCacheValid = false;
                        g_BlurCaptureRequested = true;
                    }
                    if (sigma::button(
                        "Clear All",
                        u8"\uf182",
                        ImVec2(add_object_button_min.x + object_button_size.x + object_button_gap, add_object_button_min.y),
                        object_button_size,
                        object_list_button_hover_fade[1],
                        12.0f,
                        13.0f,
                        content_alpha,
                        content_interactive && !visual_popup_active()))
                    {
                        object_entry_count = 0;
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 object_box_min(object_list_min.x + 13.0f, add_object_button_min.y + object_button_size.y + 12.0f);
                    const ImVec2 object_box_size(267.0f, 348.0f);
                    sigma::child(
                        object_box_min,
                        object_box_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(16, 16, 16, 255),
                        IM_COL32(6, 6, 6, 255),
                        content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* object_draw_list = ImGui::GetBackgroundDrawList();
                    if (object_entry_count <= 0)
                    {
                        const char* empty_object_text = "No objects added yet";
                        const float empty_object_text_size = 12.0f;
                        const ImVec2 empty_object_text_measure = title_font->CalcTextSizeA(empty_object_text_size, 1000.0f, 0.0f, empty_object_text);
                        object_draw_list->AddText(
                            title_font,
                            empty_object_text_size,
                            ImVec2(
                                object_box_min.x + (object_box_size.x - empty_object_text_measure.x) * 0.5f,
                                object_box_min.y + 39.0f),
                            IM_COL32(74, 74, 78, static_cast<int>(255.0f * content_alpha)),
                            empty_object_text);
                    }
                    else
                    {
                        object_draw_list->PushClipRect(object_box_min, ImVec2(object_box_min.x + object_box_size.x, object_box_min.y + object_box_size.y), true);
                        const ImVec2 object_item_size(object_box_size.x - 16.0f, 54.0f);
                        const float object_item_gap = 8.0f;
                        for (int object_index = 0; object_index < object_entry_count; ++object_index)
                        {
                            ObjectEntry& object_entry = object_entries[object_index];
                            const ImVec2 item_min(object_box_min.x + 8.0f, object_box_min.y + 8.0f + object_index * (object_item_size.y + object_item_gap));
                            const ImVec2 item_max(item_min.x + object_item_size.x, item_min.y + object_item_size.y);
                            if (item_min.y > object_box_min.y + object_box_size.y)
                                break;
                            object_draw_list->AddRectFilled(item_min, item_max, IM_COL32(14, 14, 14, static_cast<int>(255.0f * content_alpha)), 2.0f);
                            object_draw_list->AddRect(item_min, item_max, IM_COL32(17, 17, 17, static_cast<int>(255.0f * content_alpha)), 2.0f, 0, 1.0f);
                            object_draw_list->AddText(
                                title_font,
                                12.0f,
                                ImVec2(item_min.x + 12.0f, item_min.y + 9.0f),
                                IM_COL32(205, 205, 210, static_cast<int>(255.0f * content_alpha)),
                                object_entry.name[0] != '\0' ? object_entry.name : object_entry.hash);
                            char hash_text[48];
                            ImFormatString(hash_text, IM_ARRAYSIZE(hash_text), "Hash: %.32s", object_entry.hash);
                            object_draw_list->AddText(
                                title_font,
                                12.0f,
                                ImVec2(item_min.x + 12.0f, item_min.y + 30.0f),
                                IM_COL32(74, 74, 78, static_cast<int>(255.0f * content_alpha)),
                                hash_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            const ImVec2 delete_min(item_max.x - 31.0f, item_min.y + 7.0f);
                            const ImVec2 delete_size(24.0f, 24.0f);
                            char delete_id[48];
                            ImFormatString(delete_id, IM_ARRAYSIZE(delete_id), "##object_delete_%d", object_index);
                            if (sigma::icon_action(
                                delete_id,
                                u8"\uf182",
                                ImVec2(delete_min.x + delete_size.x * 0.5f, delete_min.y + delete_size.y * 0.5f),
                                delete_size,
                                icon_font,
                                12.0f,
                                object_entry.delete_hover_fade,
                                io.DeltaTime,
                                content_alpha,
                                content_interactive && !visual_popup_active()))
                            {
                                for (int move_index = object_index; move_index < object_entry_count - 1; ++move_index)
                                    object_entries[move_index] = object_entries[move_index + 1];
                                --object_entry_count;
                                --object_index;
                            }
                        }
                        object_draw_list->PopClipRect();
                    }
                    const bool object_visual_popup_region_needed = visual_popup_active();
                    const bool object_bottom_region_needed = g_BottomProfilePopupRegionActive || object_visual_popup_region_needed;
                    if (g_BottomOptionPopupRegionActive != object_visual_popup_region_needed || g_BottomPopupRegionActive != object_bottom_region_needed || g_RightPopupRegionActive != object_visual_popup_region_needed)
                    {
                        g_BottomOptionPopupRegionActive = object_visual_popup_region_needed;
                        g_BottomPopupRegionActive = object_bottom_region_needed;
                        g_RightPopupRegionActive = object_visual_popup_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
                }
                else if (displayed_top_tab == 2 && render_sub_tab == 3)
                {
                    const ImVec2 bullet_tracer_min(81.0f, 70.0f);
                    const ImVec2 bullet_tracer_size(293.0f, 207.0f);
                    const ImVec2 hit_data_min(392.0f, 70.0f);
                    const ImVec2 hit_data_size(293.0f, 151.0f);
                    const ImVec2 extra_visuals_min(81.0f, 291.0f);
                    const ImVec2 extra_visuals_size(293.0f, 151.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto world_popup_active = [&]() -> bool
                    {
                        for (int popup_index = 0; popup_index < IM_ARRAYSIZE(world_popup_open); ++popup_index)
                        {
                            if (world_popup_open[popup_index] || world_popup_fade[popup_index] > 0.005f)
                                return true;
                        }
                        return false;
                    };
                    const auto close_world_popups = [&](int keep_index)
                    {
                        for (int popup_index = 0; popup_index < IM_ARRAYSIZE(world_popup_open); ++popup_index)
                        {
                            if (popup_index != keep_index)
                                world_popup_open[popup_index] = false;
                        }
                    };
                    const auto draw_world_popup_slider = [&](
                        const char* id_prefix,
                        int slider_index,
                        const ImVec2& popup_min,
                        float popup_width,
                        const char* label,
                        float label_y,
                        float track_y,
                        float max_value,
                        const char* value_format,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const ImVec2 track_min(popup_min.x + 16.0f, popup_min.y + track_y);
                        const ImVec2 track_max(popup_min.x + popup_width - 16.0f, popup_min.y + track_y + 5.0f);
                        const ImVec2 hit_min(track_min.x, track_min.y - 10.0f);
                        const ImVec2 hit_size(track_max.x - track_min.x, 24.0f);
                        const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                        bool slider_hovered = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            slider_hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                            if (slider_hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (slider_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                world_popup_slider_dragging[slider_index] = true;
                        }
                        if (!io.MouseDown[ImGuiMouseButton_Left])
                            world_popup_slider_dragging[slider_index] = false;
                        if (world_popup_slider_dragging[slider_index])
                        {
                            const float width = track_max.x - track_min.x;
                            const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
                            world_popup_slider_target_value[slider_index] = ImClamp(t, 0.0f, 1.0f) * max_value;
                        }
                        const float smooth_step = io.DeltaTime * 18.0f > 1.0f ? 1.0f : io.DeltaTime * 18.0f;
                        world_popup_slider_value[slider_index] += (world_popup_slider_target_value[slider_index] - world_popup_slider_value[slider_index]) * smooth_step;
                        if (world_popup_slider_value[slider_index] - world_popup_slider_target_value[slider_index] < 0.01f && world_popup_slider_target_value[slider_index] - world_popup_slider_value[slider_index] < 0.01f)
                            world_popup_slider_value[slider_index] = world_popup_slider_target_value[slider_index];
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        char value_text[32];
                        ImFormatString(value_text, IM_ARRAYSIZE(value_text), value_format, world_popup_slider_value[slider_index]);
                        const ImVec2 value_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, value_text);
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + popup_width - 16.0f - value_size.x, popup_min.y + label_y), IM_COL32(88, 88, 94, popup_alpha), value_text);
                        popup_draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, popup_alpha), 2.5f);
                        const float value_t = max_value > 0.0f ? ImClamp(world_popup_slider_value[slider_index] / max_value, 0.0f, 1.0f) : 0.0f;
                        const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
                        if (fill_x > track_min.x + 0.5f)
                            popup_draw_list->AddRectFilled(track_min, ImVec2(fill_x, track_max.y), IM_COL32(148, 140, 246, popup_alpha), 2.5f);
                        popup_draw_list->AddCircleFilled(ImVec2(fill_x, track_min.y + 2.5f), slider_hovered || world_popup_slider_dragging[slider_index] ? 7.5f : 7.0f, IM_COL32(44, 44, 44, popup_alpha), 24);
                    };
                    const auto draw_world_popup_select = [&](
                        const ImVec2& popup_min,
                        float popup_width,
                        const char* label,
                        const char* value,
                        float label_y,
                        float box_y,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const ImVec2 box_min(popup_min.x + 16.0f, popup_min.y + box_y);
                        const ImVec2 box_max(popup_min.x + popup_width - 16.0f, popup_min.y + box_y + 29.0f);
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + label_y), IM_COL32(205, 205, 210, popup_alpha), label);
                        popup_draw_list->AddRectFilled(box_min, box_max, IM_COL32(22, 22, 22, popup_alpha), 2.0f);
                        popup_draw_list->AddText(medium_font, 12.0f, ImVec2(box_min.x + 10.0f, box_min.y + 8.0f), IM_COL32(205, 205, 210, popup_alpha), value);
                        popup_draw_list->AddText(icon_font, 11.0f, ImVec2(box_max.x - 18.0f, box_min.y + 9.0f), IM_COL32(142, 134, 246, popup_alpha), u8"\uf078");
                    };
                    const auto draw_world_popup_toggle = [&](
                        const char* id_prefix,
                        int toggle_index,
                        const ImVec2& popup_min,
                        float popup_width,
                        const char* label,
                        float y,
                        float popup_alpha_f)
                    {
                        ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                        const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                        const ImVec2 hit_min(popup_min.x + 16.0f, popup_min.y + y - 6.0f);
                        const ImVec2 hit_size(popup_width - 32.0f, 26.0f);
                        const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                        bool hovered = false;
                        if (content_interactive)
                        {
                            (void)id_prefix;
                            hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                            if (hovered)
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                            if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                world_popup_toggle_enabled[toggle_index] = !world_popup_toggle_enabled[toggle_index];
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        world_popup_toggle_fade[toggle_index] += ((world_popup_toggle_enabled[toggle_index] ? 1.0f : 0.0f) - world_popup_toggle_fade[toggle_index]) * (io.DeltaTime * 12.0f > 1.0f ? 1.0f : io.DeltaTime * 12.0f);
                        const float enabled_fade = smooth_fade(world_popup_toggle_fade[toggle_index]);
                        const ImVec2 toggle_min(popup_min.x + popup_width - 41.0f, popup_min.y + y - 2.0f);
                        const ImVec2 toggle_size(25.0f, 15.0f);
                        const int track_r = static_cast<int>(32.0f + (148.0f - 32.0f) * enabled_fade);
                        const int track_g = static_cast<int>(32.0f + (140.0f - 32.0f) * enabled_fade);
                        const int track_b = static_cast<int>(34.0f + (246.0f - 34.0f) * enabled_fade);
                        const int knob_value = static_cast<int>(86.0f + (10.0f - 86.0f) * enabled_fade);
                        const float knob_x = toggle_min.x + 7.5f + (toggle_size.x - 15.0f) * enabled_fade;
                        popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + y), IM_COL32(205, 205, 210, popup_alpha), label);
                        popup_draw_list->AddRectFilled(toggle_min, ImVec2(toggle_min.x + toggle_size.x, toggle_min.y + toggle_size.y), IM_COL32(track_r, track_g, track_b, popup_alpha), toggle_size.y * 0.5f);
                        popup_draw_list->AddCircleFilled(ImVec2(knob_x, toggle_min.y + toggle_size.y * 0.5f), hovered ? 5.0f : 4.6f, IM_COL32(knob_value, knob_value, knob_value, popup_alpha), 24);
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const auto draw_world_panel = [&](
                        const ImVec2& child_min,
                        const ImVec2& child_size,
                        const char* icon,
                        const char* title,
                        const char* id_prefix,
                        const char* const* labels,
                        int label_count,
                        bool* enabled,
                        float* toggle_fade,
                        float* hover_fade,
                        bool settings_marker_for_all,
                        int marker_only_index,
                        int popup_base_index)
                    {
                        sigma::child(
                            child_min,
                            child_size,
                            3.0f,
                            IM_COL32(14, 14, 14, 255),
                            IM_COL32(22, 22, 22, 255),
                            IM_COL32(5, 5, 5, 255),
                            content_alpha);
                        sigma::child_header(child_min, child_size, icon, title, content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool any_world_popup_active = world_popup_active();
                        for (int option_index = 0; option_index < label_count; ++option_index)
                        {
                            const float row_y = child_min.y + 57.0f + option_index * 29.0f;
                            const bool gear_row = settings_marker_for_all || option_index == marker_only_index;
                            const int popup_index = gear_row ? (settings_marker_for_all ? popup_base_index + option_index : popup_base_index) : -1;
                            const bool this_row_popup_active =
                                gear_row &&
                                popup_index >= 0 &&
                                popup_index < IM_ARRAYSIZE(world_popup_open) &&
                                (world_popup_open[popup_index] || world_popup_fade[popup_index] > 0.005f);
                            const bool row_controls_interactive = content_interactive && (!any_world_popup_active || this_row_popup_active);
                            char option_id[64];
                            ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##%s_%d", id_prefix, option_index);
                            sigma::checkbox(
                                option_id,
                                labels[option_index],
                                ImVec2(child_min.x + 16.0f, row_y - 9.0f),
                                ImVec2(267.0f, 29.0f),
                                ImVec2(child_min.x + 13.0f, row_y),
                                ImVec2(child_min.x + 249.0f, row_y - 2.0f),
                                enabled[option_index],
                                toggle_fade[option_index],
                                hover_fade[option_index],
                                13.0f,
                                false,
                                content_alpha,
                                row_controls_interactive && !gear_row);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            if (gear_row && popup_index >= 0 && popup_index < IM_ARRAYSIZE(world_popup_open))
                            {
                                const ImVec2 gear_pos(child_min.x + 224.0f, row_y - 3.0f);
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const ImVec2 gear_hit_size(24.0f, 24.0f);
                                const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                                const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                                bool gear_hovered = false;
                                bool gear_clicked = false;
                                if (content_interactive)
                                {
                                    gear_hovered =
                                        io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                        io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                    gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                    if (gear_hovered)
                                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                }
                                else
                                {
                                    world_popup_open[popup_index] = false;
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (gear_clicked)
                                {
                                    const bool next_open = !world_popup_open[popup_index];
                                    close_world_popups(popup_index);
                                    world_popup_open[popup_index] = next_open;
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                handle_combat_checkbox_hit(
                                    child_min,
                                    row_y,
                                    row_controls_interactive,
                                    gear_hovered,
                                    enabled[option_index],
                                    hover_fade[option_index]);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                world_gear_hover_fade[popup_index] += (((gear_hovered || world_popup_open[popup_index]) ? 1.0f : 0.0f) - world_gear_hover_fade[popup_index]) * gear_step;
                                const float gear_hover = smooth_fade(world_gear_hover_fade[popup_index]);
                                const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                                const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                                ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                                world_popup_fade[popup_index] += ((world_popup_open[popup_index] ? 1.0f : 0.0f) - world_popup_fade[popup_index]) * popup_step;
                                if (world_popup_fade[popup_index] < 0.01f)
                                    world_popup_fade[popup_index] = 0.0f;
                                if (world_popup_fade[popup_index] > 0.0f)
                                {
                                    const float popup_eased = smooth_fade(world_popup_fade[popup_index]);
                                    const float popup_alpha_f = content_alpha * popup_eased;
                                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                    const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                    const ImVec2 popup_size =
                                        popup_index == 0 ? ImVec2(220.0f, 350.0f) :
                                        popup_index == 1 ? ImVec2(242.0f, 164.0f) :
                                        popup_index == 2 ? ImVec2(242.0f, 164.0f) :
                                        popup_index == 3 ? ImVec2(208.0f, 75.0f) :
                                        popup_index == 4 ? ImVec2(208.0f, 176.0f) :
                                        ImVec2(212.0f, 250.0f);
                                    const ImVec2 popup_min(gear_center.x + 10.0f, gear_center.y + 10.0f + popup_slide_y);
                                    const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                    const bool popup_hovered = draw_combat_popup_shell("world", popup_index, popup_min, popup_size, popup_alpha_f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    if (popup_index == 0)
                                    {
                                        draw_world_popup_select(popup_min, popup_size.x, "Bullet Tracer Source", "Local", 18.0f, 40.0f, popup_alpha_f);
                                        draw_world_popup_slider("world_line", 0, popup_min, popup_size.x, "Expiration Time", 84.0f, 106.0f, 10.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_line", 1, popup_min, popup_size.x, "Max Bullets", 128.0f, 150.0f, 100.0f, "%.0f", popup_alpha_f);
                                        draw_world_popup_slider("world_line", 2, popup_min, popup_size.x, "Thickness", 172.0f, 194.0f, 5.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_line", 3, popup_min, popup_size.x, "Segments", 216.0f, 238.0f, 32.0f, "%.0f", popup_alpha_f);
                                        draw_world_popup_select(popup_min, popup_size.x, "Style", "Solid", 262.0f, 284.0f, popup_alpha_f);
                                        ImGui::GetForegroundDrawList()->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 328.0f), IM_COL32(205, 205, 210, popup_alpha), "Color");
                                        ImGui::GetForegroundDrawList()->AddCircle(ImVec2(popup_max.x - 23.0f, popup_min.y + 333.0f), 7.8f, IM_COL32(202, 202, 210, popup_alpha), 24, 1.5f);
                                    }
                                    else if (popup_index == 1)
                                    {
                                        draw_world_popup_slider("world_impact", 4, popup_min, popup_size.x, "Inner Size", 18.0f, 40.0f, 5.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_impact", 5, popup_min, popup_size.x, "Size", 62.0f, 84.0f, 10.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_select(popup_min, popup_size.x, "Style", "Circle", 106.0f, 126.0f, popup_alpha_f);
                                    }
                                    else if (popup_index == 2)
                                    {
                                        draw_world_popup_slider("world_glow", 6, popup_min, popup_size.x, "Intensity", 18.0f, 40.0f, 2.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_glow", 7, popup_min, popup_size.x, "Size", 62.0f, 84.0f, 5.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_select(popup_min, popup_size.x, "Style", "None", 106.0f, 126.0f, popup_alpha_f);
                                    }
                                    else if (popup_index == 3)
                                    {
                                        draw_world_popup_slider("world_fade", 8, popup_min, popup_size.x, "Duration", 18.0f, 40.0f, 5.0f, "%.1fs", popup_alpha_f);
                                    }
                                    else if (popup_index == 4)
                                    {
                                        draw_world_popup_slider("world_particles", 9, popup_min, popup_size.x, "Count", 18.0f, 40.0f, 20.0f, "%.0f", popup_alpha_f);
                                        draw_world_popup_slider("world_particles", 10, popup_min, popup_size.x, "Size", 58.0f, 80.0f, 5.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_particles", 11, popup_min, popup_size.x, "Spread", 98.0f, 120.0f, 10.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_particles", 12, popup_min, popup_size.x, "Speed", 138.0f, 160.0f, 5.0f, "%.1f", popup_alpha_f);
                                    }
                                    else if (popup_index == 5)
                                    {
                                        draw_world_popup_toggle("world_crosshair", 0, popup_min, popup_size.x, "Draw on top of scopes", 22.0f, popup_alpha_f);
                                        draw_world_popup_toggle("world_crosshair", 1, popup_min, popup_size.x, "Outline", 52.0f, popup_alpha_f);
                                        draw_world_popup_slider("world_crosshair", 13, popup_min, popup_size.x, "Crosshair Size", 84.0f, 106.0f, 10.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_crosshair", 14, popup_min, popup_size.x, "Line Width", 128.0f, 150.0f, 5.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_crosshair", 15, popup_min, popup_size.x, "Gap Size", 172.0f, 194.0f, 12.0f, "%.1f", popup_alpha_f);
                                        draw_world_popup_slider("world_crosshair", 16, popup_min, popup_size.x, "Dot Size", 204.0f, 226.0f, 8.0f, "%.1f", popup_alpha_f);
                                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    if (world_popup_open[popup_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                        world_popup_open[popup_index] = false;
                                }
                            }
                        }
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* bullet_tracer_labels[] = {
                        "Enable Line",
                        "Enable Impact",
                        "Enable Glow",
                        "Enable Fade",
                        "Enable Particles"
                    };
                    const char* hit_data_labels[] = {
                        "Enable Hit Log",
                        "Hit Sound",
                        "Kill Sound"
                    };
                    const char* extra_visuals_labels[] = {
                        "Spectators List",
                        "Keybind List",
                        "Draw crosshair"
                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    draw_world_panel(
                        bullet_tracer_min,
                        bullet_tracer_size,
                        u8"\uf3dc",
                        "Bullet Tracer",
                        "world_bullet_tracer",
                        bullet_tracer_labels,
                        IM_ARRAYSIZE(bullet_tracer_labels),
                        world_bullet_tracer_enabled,
                        world_bullet_tracer_toggle_fade,
                        world_bullet_tracer_hover_fade,
                        true,
                        -1,
                        0);
                    draw_world_panel(
                        hit_data_min,
                        hit_data_size,
                        u8"\uf0f0",
                        "Hit Data",
                        "world_hit_data",
                        hit_data_labels,
                        IM_ARRAYSIZE(hit_data_labels),
                        world_hit_data_enabled,
                        world_hit_data_toggle_fade,
                        world_hit_data_hover_fade,
                        false,
                        -1,
                        -1);
                    draw_world_panel(
                        extra_visuals_min,
                        extra_visuals_size,
                        u8"\uf407",
                        "Extra Visuals",
                        "world_extra_visuals",
                        extra_visuals_labels,
                        IM_ARRAYSIZE(extra_visuals_labels),
                        world_extra_visuals_enabled,
                        world_extra_visuals_toggle_fade,
                        world_extra_visuals_hover_fade,
                        false,
                        2,
                        5);
                    const bool world_popup_region_needed = world_popup_active();
                    const bool world_bottom_region_needed = g_BottomProfilePopupRegionActive || world_popup_region_needed;
                    if (g_RightPopupRegionActive != world_popup_region_needed || g_BottomOptionPopupRegionActive != world_popup_region_needed || g_BottomPopupRegionActive != world_bottom_region_needed)
                    {
                        g_RightPopupRegionActive = world_popup_region_needed;
                        g_BottomOptionPopupRegionActive = world_popup_region_needed;
                        g_BottomPopupRegionActive = world_bottom_region_needed;
                        UpdateWindowRegion(hwnd, g_BottomPopupRegionActive, g_RightPopupRegionActive);
                    }
                }
                else if (displayed_top_tab == 3 && (render_sub_tab == 0 || render_sub_tab == 1))
                {
                    const bool entity_vehicle_view = render_sub_tab == 1;
                    const char* entity_list_title = entity_vehicle_view ? "Vehicle List" : "Players";
                    const char* entity_info_title = entity_vehicle_view ? "Vehicle Info" : "Player Action";
                    const char* entity_search_placeholder = entity_vehicle_view ? "Search vehicle..." : "Search player...";
                    const char* entity_empty_text = entity_vehicle_view ? "No vehicles nearby" : "No players nearby";
                    const char* entity_icon = entity_vehicle_view ? u8"\uf142" : u8"\uf040";
                    char* entity_search = entity_vehicle_view ? entity_vehicle_search : entity_player_search;
                    const ImVec2 entity_list_min(81.0f, 70.0f);
                    const ImVec2 entity_info_min(392.0f, 70.0f);
                    const ImVec2 entity_child_size(293.0f, 426.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        entity_list_min,
                        entity_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(entity_list_min, entity_child_size, entity_icon, entity_list_title, content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::search_box_input(
                        entity_vehicle_view ? "##entity_vehicle_search_input" : "##entity_player_search_input",
                        ImVec2(entity_list_min.x + 13.0f, entity_list_min.y + 57.0f),
                        ImVec2(267.0f, 36.0f),
                        entity_search,
                        64,
                        entity_search_placeholder,
                        u8"\uf5ae",
                        content_alpha,
                        content_interactive,
                        20,
                        20,
                        20);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 entity_box_min(entity_list_min.x + 13.0f, entity_list_min.y + 105.0f);
                    const ImVec2 entity_box_size(267.0f, 308.0f);
                    const ImVec2 entity_box_max(entity_box_min.x + entity_box_size.x, entity_box_min.y + entity_box_size.y);
                    ImDrawList* entity_draw_list = ImGui::GetBackgroundDrawList();
                    const int entity_alpha = static_cast<int>(255.0f * content_alpha);
                    entity_draw_list->AddRectFilled(entity_box_min, entity_box_max, IM_COL32(14, 14, 14, entity_alpha), 2.0f);
                    entity_draw_list->AddRect(entity_box_min, entity_box_max, IM_COL32(5, 5, 5, entity_alpha), 2.0f, 0, 1.0f);
                    entity_draw_list->AddRect(
                        ImVec2(entity_box_min.x + 1.0f, entity_box_min.y + 1.0f),
                        ImVec2(entity_box_max.x - 1.0f, entity_box_max.y - 1.0f),
                        IM_COL32(17, 17, 17, entity_alpha),
                        1.0f,
                        0,
                        1.0f);
                    const ImVec2 entity_empty_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, entity_empty_text);
                    entity_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(entity_box_min.x + (entity_box_size.x - entity_empty_size.x) * 0.5f, entity_box_min.y + 24.0f),
                        IM_COL32(74, 74, 78, entity_alpha),
                        entity_empty_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        entity_info_min,
                        entity_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(entity_info_min, entity_child_size, entity_icon, entity_info_title, content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 info_empty_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, entity_empty_text);
                    entity_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(entity_info_min.x + (entity_child_size.x - info_empty_size.x) * 0.5f, entity_info_min.y + 75.0f),
                        IM_COL32(74, 74, 78, entity_alpha),
                        entity_empty_text);
                }
                else if (displayed_top_tab == 3 && render_sub_tab == 2)
                {
                    const ImVec2 entity_object_list_min(81.0f, 70.0f);
                    const ImVec2 entity_object_info_min(392.0f, 70.0f);
                    const ImVec2 entity_object_child_size(293.0f, 426.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        entity_object_list_min,
                        entity_object_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(entity_object_list_min, entity_object_child_size, u8"\uf0f0", "Object List", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::search_box_input(
                        "##entity_object_search_input",
                        ImVec2(entity_object_list_min.x + 13.0f, entity_object_list_min.y + 57.0f),
                        ImVec2(267.0f, 36.0f),
                        entity_object_search,
                        IM_ARRAYSIZE(entity_object_search),
                        "Search object...",
                        u8"\uf5ae",
                        content_alpha,
                        content_interactive,
                        20,
                        20,
                        20);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 entity_object_box_min(entity_object_list_min.x + 13.0f, entity_object_list_min.y + 105.0f);
                    const ImVec2 entity_object_box_size(267.0f, 308.0f);
                    const ImVec2 entity_object_box_max(entity_object_box_min.x + entity_object_box_size.x, entity_object_box_min.y + entity_object_box_size.y);
                    ImDrawList* entity_object_draw_list = ImGui::GetBackgroundDrawList();
                    const int entity_object_alpha = static_cast<int>(255.0f * content_alpha);
                    entity_object_draw_list->AddRectFilled(entity_object_box_min, entity_object_box_max, IM_COL32(14, 14, 14, entity_object_alpha), 2.0f);
                    entity_object_draw_list->AddRect(entity_object_box_min, entity_object_box_max, IM_COL32(5, 5, 5, entity_object_alpha), 2.0f, 0, 1.0f);
                    entity_object_draw_list->AddRect(
                        ImVec2(entity_object_box_min.x + 1.0f, entity_object_box_min.y + 1.0f),
                        ImVec2(entity_object_box_max.x - 1.0f, entity_object_box_max.y - 1.0f),
                        IM_COL32(17, 17, 17, entity_object_alpha),
                        1.0f,
                        0,
                        1.0f);
                    const char* no_objects_nearby_text = "No objects nearby";
                    const ImVec2 no_objects_nearby_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, no_objects_nearby_text);
                    entity_object_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(entity_object_box_min.x + (entity_object_box_size.x - no_objects_nearby_size.x) * 0.5f, entity_object_box_min.y + 24.0f),
                        IM_COL32(74, 74, 78, entity_object_alpha),
                        no_objects_nearby_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        entity_object_info_min,
                        entity_object_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(entity_object_info_min, entity_object_child_size, u8"\uf0f0", "Object Info", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 info_text_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, no_objects_nearby_text);
                    entity_object_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(entity_object_info_min.x + (entity_object_child_size.x - info_text_size.x) * 0.5f, entity_object_info_min.y + 75.0f),
                        IM_COL32(74, 74, 78, entity_object_alpha),
                        no_objects_nearby_text);
                }
                else if (displayed_top_tab == 3 && render_sub_tab == 3)
                {
                    const ImVec2 friend_list_min(81.0f, 70.0f);
                    const ImVec2 friend_helper_min(392.0f, 70.0f);
                    const ImVec2 friend_child_size(293.0f, 426.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        friend_list_min,
                        friend_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(friend_list_min, friend_child_size, u8"\uf040", "Friend List", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 friend_box_min(friend_list_min.x + 13.0f, friend_list_min.y + 57.0f);
                    const ImVec2 friend_box_size(267.0f, 356.0f);
                    const ImVec2 friend_box_max(friend_box_min.x + friend_box_size.x, friend_box_min.y + friend_box_size.y);
                    ImDrawList* friend_draw_list = ImGui::GetBackgroundDrawList();
                    const int friend_alpha = static_cast<int>(255.0f * content_alpha);
                    friend_draw_list->AddRectFilled(friend_box_min, friend_box_max, IM_COL32(14, 14, 14, friend_alpha), 2.0f);
                    friend_draw_list->AddRect(friend_box_min, friend_box_max, IM_COL32(5, 5, 5, friend_alpha), 2.0f, 0, 1.0f);
                    friend_draw_list->AddRect(
                        ImVec2(friend_box_min.x + 1.0f, friend_box_min.y + 1.0f),
                        ImVec2(friend_box_max.x - 1.0f, friend_box_max.y - 1.0f),
                        IM_COL32(17, 17, 17, friend_alpha),
                        1.0f,
                        0,
                        1.0f);
                    const char* no_friends_text = "No friends added";
                    const ImVec2 no_friends_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, no_friends_text);
                    friend_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(friend_box_min.x + (friend_box_size.x - no_friends_size.x) * 0.5f, friend_box_min.y + 30.0f),
                        IM_COL32(74, 74, 78, static_cast<int>(255.0f * content_alpha)),
                        no_friends_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(
                        friend_helper_min,
                        friend_child_size,
                        3.0f,
                        IM_COL32(14, 14, 14, 255),
                        IM_COL32(22, 22, 22, 255),
                        IM_COL32(5, 5, 5, 255),
                        content_alpha);
                    sigma::child_header(friend_helper_min, friend_child_size, u8"\uf040", "Friend Helper", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* friend_helper_labels[] = {
                        "Add Friend by Distance",
                        "Show Distance Circle",
                        "Show Player Flags"
                    };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(friend_helper_labels); ++option_index)
                    {
                        const float row_y = friend_helper_min.y + 57.0f + option_index * 29.0f;
                        char option_id[56];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##friend_helper_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            friend_helper_labels[option_index],
                            ImVec2(friend_helper_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(friend_helper_min.x + 13.0f, row_y),
                            ImVec2(friend_helper_min.x + 249.0f, row_y - 2.0f),
                            friend_helper_enabled[option_index],
                            friend_helper_toggle_fade[option_index],
                            friend_helper_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::slider(
                        "Distance",
                        friend_helper_min,
                        friend_helper_min.y + 143.0f,
                        friend_helper_min.y + 166.0f,
                        friend_helper_distance_value,
                        friend_helper_distance_target_value,
                        friend_helper_distance_dragging,
                        200.0f,
                        "%.0fm",
                        content_alpha,
                        content_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 friend_button_size(265.0f, 33.0f);
                    const ImVec2 add_by_proximity_min(friend_helper_min.x + 13.0f, friend_helper_min.y + 191.0f);
                    sigma::button(
                        "Add by Proximity",
                        add_by_proximity_min,
                        friend_button_size,
                        friend_helper_button_hover_fade[0],
                        12.0f,
                        content_alpha,
                        content_interactive);
                    sigma::button(
                        "Remove by Proximity",
                        ImVec2(add_by_proximity_min.x, add_by_proximity_min.y + friend_button_size.y + 10.0f),
                        friend_button_size,
                        friend_helper_button_hover_fade[1],
                        12.0f,
                        content_alpha,
                        content_interactive);
                }
                else if (displayed_top_tab == 4 && render_sub_tab == 0)
                {
                    const ImVec2 vehicle_assist_min(81.0f, 70.0f);
                    const ImVec2 vehicle_settings_min(392.0f, 70.0f);
                    const ImVec2 current_vehicle_size(293.0f, 82.0f);
                    sigma::child(vehicle_assist_min, current_vehicle_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_assist_min, current_vehicle_size, u8"\uf142", "Vehicle Assistance", content_alpha);
                    sigma::child(vehicle_settings_min, current_vehicle_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_settings_min, current_vehicle_size, u8"\uf142", "Vehicle Settings", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* current_vehicle_draw_list = ImGui::GetBackgroundDrawList();
                    const int current_vehicle_alpha = static_cast<int>(255.0f * content_alpha);
                    current_vehicle_draw_list->AddText(title_font, 13.0f, ImVec2(vehicle_assist_min.x + 14.0f, vehicle_assist_min.y + 56.0f), IM_COL32(235, 235, 235, current_vehicle_alpha), "You must be in a vehicle");
                    current_vehicle_draw_list->AddText(title_font, 13.0f, ImVec2(vehicle_settings_min.x + 14.0f, vehicle_settings_min.y + 56.0f), IM_COL32(235, 235, 235, current_vehicle_alpha), "You must be in a vehicle");
                }
                else if (displayed_top_tab == 4 && render_sub_tab == 1)
                {
                    const ImVec2 vehicle_list_min(81.0f, 70.0f);
                    const ImVec2 vehicle_list_size(293.0f, 426.0f);
                    sigma::child(vehicle_list_min, vehicle_list_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_list_min, vehicle_list_size, u8"\uf142", "Vehicle List", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::combobox("All", ImVec2(vehicle_list_min.x + 13.0f, vehicle_list_min.y + 56.0f), ImVec2(267.0f, 29.0f), content_alpha, content_interactive);
                    sigma::search_box_input(
                        "##vehicle_spawner_search_input",
                        ImVec2(vehicle_list_min.x + 13.0f, vehicle_list_min.y + 100.0f),
                        ImVec2(267.0f, 36.0f),
                        vehicle_spawner_search,
                        IM_ARRAYSIZE(vehicle_spawner_search),
                        "Search for vehicle",
                        u8"\uf5ae",
                        content_alpha,
                        content_interactive,
                        20,
                        20,
                        20);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 vehicle_box_min(vehicle_list_min.x + 13.0f, vehicle_list_min.y + 149.0f);
                    const ImVec2 vehicle_box_size(267.0f, 264.0f);
                    sigma::child(vehicle_box_min, vehicle_box_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(17, 17, 17, 255), IM_COL32(5, 5, 5, 255), content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const char* vehicle_names[] = {
                        "Asbo",
                        "Blista",
                        "Brioso R/A",
                        "Brioso 300",
                        "Brioso 300 Widebody",
                        "Club",
                        "Dilettante"
                    };
                    const char* vehicle_categories[] = {
                        "Compacts",
                        "Compacts",
                        "Compacts",
                        "Compacts",
                        "Compacts",
                        "Compacts",
                        "Compacts"
                    };
                    const float vehicle_row_height = 31.0f;
                    const float vehicle_row_gap = 5.0f;
                    const float vehicle_row_x = vehicle_box_min.x + 7.0f;
                    const float vehicle_row_width = vehicle_box_size.x - 14.0f;
                    const float vehicle_first_row_y = vehicle_box_min.y + 10.0f;
                    float vehicle_visible_y[IM_ARRAYSIZE(vehicle_names)] = {};
                    bool vehicle_visible[IM_ARRAYSIZE(vehicle_names)] = {};
                    int vehicle_visible_index = 0;
                    for (int vehicle_index = 0; vehicle_index < IM_ARRAYSIZE(vehicle_names); ++vehicle_index)
                    {
                        if (!contains_case_insensitive(vehicle_names[vehicle_index], vehicle_spawner_search))
                            continue;
                        vehicle_visible[vehicle_index] = true;
                        vehicle_visible_y[vehicle_index] = vehicle_first_row_y + vehicle_visible_index * (vehicle_row_height + vehicle_row_gap);
                        ++vehicle_visible_index;
                    }
                    if (selected_spawn_vehicle_index >= 0 && !vehicle_visible[selected_spawn_vehicle_index])
                        selected_spawn_vehicle_index = -1;
                    const bool selected_vehicle_visible = selected_spawn_vehicle_index >= 0 && vehicle_visible[selected_spawn_vehicle_index];
                    const float vehicle_select_step = io.DeltaTime * 11.5f > 1.0f ? 1.0f : io.DeltaTime * 11.5f;
                    ImDrawList* vehicle_spawn_draw_list = ImGui::GetBackgroundDrawList();
                    if (selected_vehicle_visible)
                    {
                        const float target_y = vehicle_visible_y[selected_spawn_vehicle_index];
                        if (vehicle_spawn_selection_y <= 0.0f)
                            vehicle_spawn_selection_y = target_y;
                        vehicle_spawn_selection_y += (target_y - vehicle_spawn_selection_y) * vehicle_select_step;
                        vehicle_spawn_selection_slide += (1.0f - vehicle_spawn_selection_slide) * vehicle_select_step;
                        vehicle_spawn_selection_alpha += (1.0f - vehicle_spawn_selection_alpha) * vehicle_select_step;
                        const float eased_slide = smooth_fade(vehicle_spawn_selection_slide);
                        const float selection_x = vehicle_row_x - 18.0f + 18.0f * eased_slide;
                        vehicle_spawn_draw_list->AddRectFilled(
                            ImVec2(selection_x, vehicle_spawn_selection_y),
                            ImVec2(vehicle_row_x + vehicle_row_width, vehicle_spawn_selection_y + vehicle_row_height),
                            IM_COL32(23, 23, 23, static_cast<int>(255.0f * smooth_fade(vehicle_spawn_selection_alpha) * content_alpha)),
                            3.0f);
                    }
                    else
                    {
                        vehicle_spawn_selection_alpha += (0.0f - vehicle_spawn_selection_alpha) * vehicle_select_step;
                        vehicle_spawn_selection_slide += (0.0f - vehicle_spawn_selection_slide) * vehicle_select_step;
                    }
                    for (int vehicle_index = 0; vehicle_index < IM_ARRAYSIZE(vehicle_names); ++vehicle_index)
                    {
                        if (!vehicle_visible[vehicle_index])
                            continue;
                        const ImVec2 row_min(vehicle_row_x, vehicle_visible_y[vehicle_index]);
                        const ImVec2 row_max(vehicle_row_x + vehicle_row_width, vehicle_visible_y[vehicle_index] + vehicle_row_height);
                        const bool row_hovered =
                            io.MousePos.x >= row_min.x && io.MousePos.x <= row_max.x &&
                            io.MousePos.y >= row_min.y && io.MousePos.y <= row_max.y;
                        if (content_interactive && row_hovered && io.MouseClicked[0])
                        {
                            if (selected_spawn_vehicle_index != vehicle_index)
                            {
                                vehicle_spawn_selection_slide = 0.0f;
                                vehicle_spawn_selection_alpha = 0.35f;
                            }
                            selected_spawn_vehicle_index = vehicle_index;
                        }
                        if (content_interactive && row_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        const bool selected = vehicle_index == selected_spawn_vehicle_index && selected_vehicle_visible;
                        vehicle_spawn_item_text_fade[vehicle_index] += ((selected ? 1.0f : 0.0f) - vehicle_spawn_item_text_fade[vehicle_index]) * vehicle_select_step;
                        const float active_text = smooth_fade(vehicle_spawn_item_text_fade[vehicle_index]);
                        const int muted = static_cast<int>(74.0f + (235.0f - 74.0f) * active_text);
                        const int icon_alpha = static_cast<int>(255.0f * content_alpha * active_text);
                        if (icon_alpha > 0)
                        {
                            vehicle_spawn_draw_list->AddText(
                                icon_font,
                                12.0f,
                                ImVec2(row_min.x + 10.0f, row_min.y + 9.0f),
                                IM_COL32(142, 134, 246, icon_alpha),
                                u8"\uf313");
                        }
                        vehicle_spawn_draw_list->AddText(
                            medium_font,
                            13.0f,
                            ImVec2(row_min.x + 10.0f + 22.0f * active_text, row_min.y + 8.0f),
                            IM_COL32(muted, muted, muted + 4 > 255 ? 255 : muted + 4, static_cast<int>(255.0f * content_alpha)),
                            vehicle_names[vehicle_index]);
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 vehicle_info_min(392.0f, 70.0f);
                    const ImVec2 vehicle_info_size(293.0f, 120.0f);
                    sigma::child(vehicle_info_min, vehicle_info_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_info_min, vehicle_info_size, u8"\uf142", "Vehicle Info", content_alpha);
                    const int vehicle_info_alpha = static_cast<int>(255.0f * content_alpha);
                    const char* selected_vehicle_name = selected_vehicle_visible ? vehicle_names[selected_spawn_vehicle_index] : "-";
                    const char* selected_vehicle_category = selected_vehicle_visible ? vehicle_categories[selected_spawn_vehicle_index] : "-";
                    vehicle_spawn_draw_list->AddText(title_font, 12.0f, ImVec2(vehicle_info_min.x + 14.0f, vehicle_info_min.y + 61.0f), IM_COL32(78, 78, 82, vehicle_info_alpha), "Name:");
                    vehicle_spawn_draw_list->AddText(title_font, 12.0f, ImVec2(vehicle_info_min.x + 14.0f, vehicle_info_min.y + 89.0f), IM_COL32(78, 78, 82, vehicle_info_alpha), "Category:");
                    vehicle_spawn_draw_list->AddText(title_font, 12.0f, ImVec2(vehicle_info_min.x + 93.0f, vehicle_info_min.y + 61.0f), IM_COL32(232, 232, 236, vehicle_info_alpha), selected_vehicle_name);
                    vehicle_spawn_draw_list->AddText(title_font, 12.0f, ImVec2(vehicle_info_min.x + 93.0f, vehicle_info_min.y + 89.0f), IM_COL32(232, 232, 236, vehicle_info_alpha), selected_vehicle_category);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 spawn_options_min(392.0f, 204.0f);
                    const ImVec2 spawn_options_size(293.0f, 202.0f);
                    sigma::child(spawn_options_min, spawn_options_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(spawn_options_min, spawn_options_size, u8"\uf142", "Spawn Options", content_alpha);
                    const char* spawn_option_labels[] = { "Spawn in Vehicle", "Max Vehicle", "Hook into Resource" };
                    for (int option_index = 0; option_index < IM_ARRAYSIZE(spawn_option_labels); ++option_index)
                    {
                        const float row_y = spawn_options_min.y + 57.0f + option_index * 29.0f;
                        char option_id[56];
                        ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##vehicle_spawn_option_%d", option_index);
                        sigma::checkbox(
                            option_id,
                            spawn_option_labels[option_index],
                            ImVec2(spawn_options_min.x + 16.0f, row_y - 9.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(spawn_options_min.x + 13.0f, row_y),
                            ImVec2(spawn_options_min.x + 249.0f, row_y - 2.0f),
                            vehicle_spawn_option_enabled[option_index],
                            vehicle_spawn_option_toggle_fade[option_index],
                            vehicle_spawn_option_hover_fade[option_index],
                            13.0f,
                            false,
                            content_alpha);
                    }
                    sigma::button(
                        "Spawn Vehicle",
                        ImVec2(spawn_options_min.x + 13.0f, spawn_options_min.y + 147.0f),
                        ImVec2(267.0f, 37.0f),
                        vehicle_spawn_button_hover_fade[0],
                        12.0f,
                        content_alpha,
                        content_interactive && selected_vehicle_visible);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 spawn_by_name_min(392.0f, 419.0f);
                    const ImVec2 spawn_by_name_size(293.0f, 93.0f);
                    sigma::child(spawn_by_name_min, spawn_by_name_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(spawn_by_name_min, spawn_by_name_size, u8"\uf142", "Spawn by Name", content_alpha);
                    sigma::button(
                        "Spawn vehicle by name",
                        ImVec2(spawn_by_name_min.x + 13.0f, spawn_by_name_min.y + 49.0f),
                        ImVec2(267.0f, 31.0f),
                        vehicle_spawn_button_hover_fade[1],
                        12.0f,
                        content_alpha,
                        content_interactive);
                }
                else if (displayed_top_tab == 4 && render_sub_tab == 2)
                {
                    const ImVec2 vehicle_mods_min(81.0f, 70.0f);
                    const ImVec2 vehicle_mods_size(293.0f, 426.0f);
                    sigma::child(vehicle_mods_min, vehicle_mods_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(vehicle_mods_min, vehicle_mods_size, u8"\uf142", "Vehicle Mods", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 vehicle_mods_box_min(vehicle_mods_min.x + 13.0f, vehicle_mods_min.y + 57.0f);
                    const ImVec2 vehicle_mods_box_size(267.0f, 356.0f);
                    const ImVec2 vehicle_mods_box_max(vehicle_mods_box_min.x + vehicle_mods_box_size.x, vehicle_mods_box_min.y + vehicle_mods_box_size.y);
                    ImDrawList* vehicle_mods_draw_list = ImGui::GetBackgroundDrawList();
                    const int vehicle_mods_alpha = static_cast<int>(255.0f * content_alpha);
                    vehicle_mods_draw_list->AddRectFilled(vehicle_mods_box_min, vehicle_mods_box_max, IM_COL32(14, 14, 14, vehicle_mods_alpha), 2.0f);
                    vehicle_mods_draw_list->AddRect(vehicle_mods_box_min, vehicle_mods_box_max, IM_COL32(5, 5, 5, vehicle_mods_alpha), 2.0f, 0, 1.0f);
                    vehicle_mods_draw_list->AddRect(
                        ImVec2(vehicle_mods_box_min.x + 1.0f, vehicle_mods_box_min.y + 1.0f),
                        ImVec2(vehicle_mods_box_max.x - 1.0f, vehicle_mods_box_max.y - 1.0f),
                        IM_COL32(17, 17, 17, vehicle_mods_alpha),
                        1.0f,
                        0,
                        1.0f);
                    const char* vehicle_mods_empty_text = "Enter vehicle to use mods";
                    const ImVec2 vehicle_mods_empty_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, vehicle_mods_empty_text);
                    vehicle_mods_draw_list->AddText(
                        title_font,
                        12.0f,
                        ImVec2(vehicle_mods_box_min.x + (vehicle_mods_box_size.x - vehicle_mods_empty_size.x) * 0.5f, vehicle_mods_box_min.y + 28.0f),
                        IM_COL32(74, 74, 78, vehicle_mods_alpha),
                        vehicle_mods_empty_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 mods_options_min(392.0f, 70.0f);
                    const ImVec2 mods_options_size(293.0f, 96.0f);
                    sigma::child(mods_options_min, mods_options_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(mods_options_min, mods_options_size, u8"\uf142", "Mods Options", content_alpha);
                    vehicle_mods_draw_list->AddText(title_font, 14.0f, ImVec2(mods_options_min.x + 14.0f, mods_options_min.y + 59.0f), IM_COL32(74, 74, 78, vehicle_mods_alpha), "Enter vehicle to use mods");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 quick_apply_min(392.0f, 179.0f);
                    const ImVec2 quick_apply_size(293.0f, 112.0f);
                    sigma::child(quick_apply_min, quick_apply_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                    sigma::child_header(quick_apply_min, quick_apply_size, u8"\uf142", "Quick Apply", content_alpha);
                    sigma::button(
                        "Max Everything",
                        ImVec2(quick_apply_min.x + 13.0f, quick_apply_min.y + 57.0f),
                        ImVec2(267.0f, 37.0f),
                        vehicle_mods_quick_button_hover_fade,
                        12.0f,
                        content_alpha,
                        content_interactive);
                }
                else
                {
                    const bool weapon_subtab = displayed_top_tab == 0 && render_sub_tab == 1;
                    const bool macro_subtab = displayed_top_tab == 0 && render_sub_tab == 3;
                    const ImVec2 fallback_left_child_size = weapon_subtab ? ImVec2(293.0f, 432.0f) : (macro_subtab ? ImVec2(293.0f, 334.0f) : ImVec2(293.0f, 314.0f));
                    const ImVec2 fallback_right_child_size = weapon_subtab ? ImVec2(293.0f, 384.0f) : (macro_subtab ? ImVec2(293.0f, 254.0f) : ImVec2(293.0f, 183.0f));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    const ImVec2 fallback_scroll_view_min(81.0f, 70.0f);
                    const ImVec2 fallback_scroll_view_max(685.0f, 536.0f);
                    ImVec2 fallback_left_child_min = fallback_scroll_view_min;
                    ImVec2 fallback_right_child_min(392.0f, 70.0f);
                    if (macro_subtab)
                    {
                        const float macro_child_gap = 13.0f;
                        const float macro_tuning_height = 245.0f;
                        const float macro_left_content_height = fallback_left_child_size.y + macro_child_gap + macro_tuning_height;
                        const float macro_right_content_height = fallback_right_child_size.y;
                        const float macro_content_height = (macro_left_content_height > macro_right_content_height ? macro_left_content_height : macro_right_content_height) + 14.0f;
                        const float macro_view_height = fallback_scroll_view_max.y - fallback_scroll_view_min.y;
                        const float macro_max_scroll_y = macro_content_height > macro_view_height ? macro_content_height - macro_view_height : 0.0f;
                        const bool macro_scroll_hovered =
                            io.MousePos.x >= fallback_scroll_view_min.x && io.MousePos.x <= fallback_scroll_view_max.x &&
                            io.MousePos.y >= fallback_scroll_view_min.y && io.MousePos.y <= fallback_scroll_view_max.y;
                        if (macro_scroll_hovered && content_alpha > 0.4f && macro_max_scroll_y > 0.0f && io.MouseWheel != 0.0f && !macro_preset_dropdown_open && !macro_tuning_mode_dropdown_open)
                            macro_scroll_target_y -= io.MouseWheel * 64.0f;
                        if (macro_scroll_target_y < 0.0f)
                            macro_scroll_target_y = 0.0f;
                        if (macro_scroll_target_y > macro_max_scroll_y)
                            macro_scroll_target_y = macro_max_scroll_y;
                        const float macro_scroll_step = io.DeltaTime * 10.0f > 1.0f ? 1.0f : io.DeltaTime * 10.0f;
                        macro_scroll_y += (macro_scroll_target_y - macro_scroll_y) * macro_scroll_step;
                        if (macro_scroll_y < 0.01f)
                            macro_scroll_y = 0.0f;
                        if (macro_scroll_y > macro_max_scroll_y)
                            macro_scroll_y = macro_max_scroll_y;
                        fallback_left_child_min.y -= macro_scroll_y;
                        fallback_right_child_min.y -= macro_scroll_y;
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    ImDrawList* fallback_draw_list = ImGui::GetBackgroundDrawList();
                    if (macro_subtab)
                        fallback_draw_list->PushClipRect(fallback_scroll_view_min, fallback_scroll_view_max, true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(fallback_left_child_min, fallback_left_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (weapon_subtab)
                    {
                        const ImVec2 weapon_left_min(81.0f, 70.0f);
                        sigma::child_header(weapon_left_min, fallback_left_child_size, u8"\uf677", "Weapon Options", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const char* weapon_option_labels[] = {
                            "No Reload",
                            "Remove Range Limit",
                            "Infinite Ammo",
                            "Remove Recoil",
                            "Rapid Fire",
                            "Remove Spread",
                            "Damage Boost"
                        };
                        const char* weapon_tuning_labels[] = {
                            "Recoil Value",
                            "Rapid Fire Value",
                            "Spread Value",
                            "Damage Value"
                        };
                        bool weapon_tuning_popup_active = false;
                        for (int tuning_index = 0; tuning_index < IM_ARRAYSIZE(weapon_tuning_popup_open); ++tuning_index)
                        {
                            if (weapon_tuning_popup_open[tuning_index] || weapon_tuning_popup_fade[tuning_index] > 0.005f)
                                weapon_tuning_popup_active = true;
                        }
                        const bool weapon_option_controls_interactive = content_interactive && !weapon_tuning_popup_active;
                        for (int option_index = 0; option_index < IM_ARRAYSIZE(weapon_option_labels); ++option_index)
                        {
                            const float row_y = weapon_left_min.y + 57.0f + option_index * 29.0f;
                            const bool tuning_row = option_index >= 3 && option_index <= 6;
                            char option_id[48];
                            ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##weapon_option_%d", option_index);
                            const ImVec2 row_min(weapon_left_min.x + 16.0f, row_y - 9.0f);
                            const ImVec2 row_size(267.0f, 29.0f);
                            const ImVec2 label_pos(weapon_left_min.x + 13.0f, row_y);
                            const ImVec2 toggle_min(weapon_left_min.x + 249.0f, row_y - 2.0f);
                            if (!tuning_row)
                            {
                                sigma::checkbox(
                                    option_id,
                                    weapon_option_labels[option_index],
                                    row_min,
                                    row_size,
                                    label_pos,
                                    toggle_min,
                                    weapon_option_enabled[option_index],
                                    weapon_option_toggle_fade[option_index],
                                    weapon_option_hover_fade[option_index],
                                    13.0f,
                                    false,
                                    content_alpha,
                                    weapon_option_controls_interactive);
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            if (tuning_row)
                            {
                                const int tuning_index = option_index - 3;
                                const ImVec2 gear_pos(weapon_left_min.x + 224.0f, row_y - 3.0f);
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const ImVec2 gear_hit_size(24.0f, 24.0f);
                                const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                                const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                                bool gear_hovered = false;
                                bool gear_clicked = false;
                                if (content_interactive)
                                {
                                    gear_hovered =
                                        io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                        io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                    gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                    if (gear_hovered)
                                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                }
                                else
                                {
                                    weapon_tuning_popup_open[tuning_index] = false;
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (gear_clicked)
                                {
                                    const bool next_open = !weapon_tuning_popup_open[tuning_index];
                                    for (int close_index = 0; close_index < IM_ARRAYSIZE(weapon_tuning_popup_open); ++close_index)
                                    {
                                        if (close_index != tuning_index)
                                            weapon_tuning_popup_open[close_index] = false;
                                    }
                                    weapon_tuning_popup_open[tuning_index] = next_open;
                                    weapon_attachment_dropdown_open = false;
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const ImVec2 label_hit_min(weapon_left_min.x + 16.0f, row_y - 9.0f);
                                const ImVec2 label_hit_max(weapon_left_min.x + 215.0f, row_y + 20.0f);
                                const ImVec2 toggle_hit_min(weapon_left_min.x + 246.0f, row_y - 8.0f);
                                const ImVec2 toggle_hit_max(weapon_left_min.x + 282.0f, row_y + 19.0f);
                                const bool checkbox_hovered =
                                    weapon_option_controls_interactive &&
                                    !gear_hovered &&
                                    ((io.MousePos.x >= label_hit_min.x && io.MousePos.x <= label_hit_max.x &&
                                      io.MousePos.y >= label_hit_min.y && io.MousePos.y <= label_hit_max.y) ||
                                     (io.MousePos.x >= toggle_hit_min.x && io.MousePos.x <= toggle_hit_max.x &&
                                      io.MousePos.y >= toggle_hit_min.y && io.MousePos.y <= toggle_hit_max.y));
                                if (checkbox_hovered)
                                {
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                        weapon_option_enabled[option_index] = !weapon_option_enabled[option_index];
                                }
                                const float checkbox_hover_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                weapon_option_hover_fade[option_index] += ((checkbox_hovered ? 1.0f : 0.0f) - weapon_option_hover_fade[option_index]) * checkbox_hover_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                sigma::checkbox(
                                    option_id,
                                    weapon_option_labels[option_index],
                                    row_min,
                                    row_size,
                                    label_pos,
                                    toggle_min,
                                    weapon_option_enabled[option_index],
                                    weapon_option_toggle_fade[option_index],
                                    weapon_option_hover_fade[option_index],
                                    13.0f,
                                    false,
                                    content_alpha,
                                    false,
                                    true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                weapon_tuning_gear_hover_fade[tuning_index] += (((gear_hovered || weapon_tuning_popup_open[tuning_index]) ? 1.0f : 0.0f) - weapon_tuning_gear_hover_fade[tuning_index]) * gear_step;
                                const float gear_hover = smooth_fade(weapon_tuning_gear_hover_fade[tuning_index]);
                                const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                                const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                                ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                                weapon_tuning_popup_fade[tuning_index] += ((weapon_tuning_popup_open[tuning_index] ? 1.0f : 0.0f) - weapon_tuning_popup_fade[tuning_index]) * popup_step;
                                if (weapon_tuning_popup_fade[tuning_index] < 0.01f)
                                    weapon_tuning_popup_fade[tuning_index] = 0.0f;
                                if (weapon_tuning_popup_fade[tuning_index] > 0.0f)
                                {
                                    const float popup_eased = smooth_fade(weapon_tuning_popup_fade[tuning_index]);
                                    const float popup_alpha_f = content_alpha * popup_eased;
                                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                    const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                    const ImVec2 popup_size(228.0f, 75.0f);
                                    const ImVec2 popup_min(gear_center.x + 10.0f, gear_center.y + 10.0f + popup_slide_y);
                                    const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                                    popup_draw_list->AddRectFilled(
                                        ImVec2(popup_min.x + 4.0f, popup_min.y + 5.0f),
                                        ImVec2(popup_max.x + 4.0f, popup_max.y + 5.0f),
                                        IM_COL32(0, 0, 0, static_cast<int>(90.0f * popup_alpha_f)),
                                        3.0f);
                                    popup_draw_list->AddRectFilled(popup_min, popup_max, IM_COL32(14, 14, 14, popup_alpha), 3.0f);
                                    popup_draw_list->AddRect(popup_min, popup_max, IM_COL32(27, 27, 29, static_cast<int>(235.0f * popup_alpha_f)), 3.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const ImVec2 track_min(popup_min.x + 16.0f, popup_min.y + 50.0f);
                                    const ImVec2 track_max(popup_max.x - 16.0f, track_min.y + 5.0f);
                                    const ImVec2 hit_min(track_min.x, track_min.y - 10.0f);
                                    const ImVec2 hit_size(track_max.x - track_min.x, 24.0f);
                                    const ImVec2 hit_max(hit_min.x + hit_size.x, hit_min.y + hit_size.y);
                                    bool slider_hovered = false;
                                    if (content_interactive)
                                    {
                                        slider_hovered = mouse_in_rect(io.MousePos, hit_min, hit_max);
                                        if (slider_hovered)
                                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                        if (slider_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                            weapon_tuning_dragging[tuning_index] = true;
                                    }
                                    if (!io.MouseDown[ImGuiMouseButton_Left])
                                        weapon_tuning_dragging[tuning_index] = false;
                                    if (weapon_tuning_dragging[tuning_index])
                                    {
                                        const float width = track_max.x - track_min.x;
                                        const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
                                        weapon_tuning_target_value[tuning_index] = ImClamp(t, 0.0f, 1.0f) * 100.0f;
                                    }
                                    const float value_step = io.DeltaTime * 18.0f > 1.0f ? 1.0f : io.DeltaTime * 18.0f;
                                    weapon_tuning_value[tuning_index] += (weapon_tuning_target_value[tuning_index] - weapon_tuning_value[tuning_index]) * value_step;
                                    if (weapon_tuning_value[tuning_index] - weapon_tuning_target_value[tuning_index] < 0.01f && weapon_tuning_target_value[tuning_index] - weapon_tuning_value[tuning_index] < 0.01f)
                                        weapon_tuning_value[tuning_index] = weapon_tuning_target_value[tuning_index];
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    char value_text[32];
                                    ImFormatString(value_text, IM_ARRAYSIZE(value_text), "%.2f%%", weapon_tuning_value[tuning_index]);
                                    const ImVec2 value_size = title_font->CalcTextSizeA(12.0f, 1000.0f, 0.0f, value_text);
                                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 22.0f), IM_COL32(214, 214, 220, popup_alpha), weapon_tuning_labels[tuning_index]);
                                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_max.x - 16.0f - value_size.x, popup_min.y + 22.0f), IM_COL32(88, 88, 94, popup_alpha), value_text);
                                    popup_draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, popup_alpha), 2.5f);
                                    const float value_t = ImClamp(weapon_tuning_value[tuning_index] / 100.0f, 0.0f, 1.0f);
                                    const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
                                    if (fill_x > track_min.x + 0.5f)
                                        popup_draw_list->AddRectFilled(track_min, ImVec2(fill_x, track_max.y), IM_COL32(148, 140, 246, popup_alpha), 2.5f);
                                    popup_draw_list->AddCircleFilled(ImVec2(fill_x, track_min.y + 2.5f), slider_hovered || weapon_tuning_dragging[tuning_index] ? 7.5f : 7.0f, IM_COL32(44, 44, 44, popup_alpha), 24);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const bool popup_hovered = ImGui::IsMouseHoveringRect(popup_min, popup_max, false);
                                    if (weapon_tuning_popup_open[tuning_index] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                        weapon_tuning_popup_open[tuning_index] = false;
                                }
                            }
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        ImGui::GetBackgroundDrawList()->AddText(title_font, 13.0f, ImVec2(weapon_left_min.x + 13.0f, 334.0f), IM_COL32(205, 205, 210, static_cast<int>(255.0f * content_alpha)), "Attachments List");
                        const char* attachment_dropdown_items[] = {
                            "None",
                            "Default Clip",
                            "Extended Clip",
                            "Tracer Rounds",
                            "Incendiary Rounds",
                            "Hollow Point Rounds",
                            "Full Metal Jacket Rounds",
                            "Mounted Scope",
                            "Flashlight",
                            "Suppressor",
                            "Compensator"
                        };
                        const ImVec2 attachment_combo_min(weapon_left_min.x + 13.0f, 356.0f);
                        const ImVec2 attachment_combo_size(265.0f, 29.0f);
                        const bool attachment_combo_hovered =
                            io.MousePos.x >= attachment_combo_min.x && io.MousePos.x <= attachment_combo_min.x + attachment_combo_size.x &&
                            io.MousePos.y >= attachment_combo_min.y && io.MousePos.y <= attachment_combo_min.y + attachment_combo_size.y;
                        sigma::combobox(
                            attachment_dropdown_items[weapon_attachment_selected_index],
                            attachment_combo_min,
                            attachment_combo_size,
                            content_alpha,
                            false);
                        if (content_interactive && !weapon_tuning_popup_active && attachment_combo_hovered && io.MouseClicked[0])
                        {
                            weapon_attachment_dropdown_open = !weapon_attachment_dropdown_open;
                            weapon_spawner_category_dropdown_open = false;
                            weapon_spawner_weapon_dropdown_open = false;
                        }
                        const ImVec2 attachment_dropdown_min(attachment_combo_min.x, attachment_combo_min.y + attachment_combo_size.y + 7.0f);
                        const ImVec2 attachment_dropdown_size(265.0f, 138.0f);
                        const ImVec2 attachment_dropdown_max(attachment_dropdown_min.x + attachment_dropdown_size.x, attachment_dropdown_min.y + attachment_dropdown_size.y);
                        const bool attachment_dropdown_hovered =
                            io.MousePos.x >= attachment_dropdown_min.x && io.MousePos.x <= attachment_dropdown_max.x &&
                            io.MousePos.y >= attachment_dropdown_min.y && io.MousePos.y <= attachment_dropdown_max.y;
                        if (content_interactive && !weapon_tuning_popup_active && weapon_attachment_dropdown_open && io.MouseClicked[0] && !attachment_dropdown_hovered && !attachment_combo_hovered)
                            weapon_attachment_dropdown_open = false;
                        if (content_interactive && !weapon_tuning_popup_active && attachment_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool attachment_dropdown_active = weapon_attachment_dropdown_open || weapon_attachment_dropdown_fade > 0.005f;
                        const bool ammo_controls_interactive = content_interactive && !attachment_dropdown_active && !weapon_tuning_popup_active;
                        sigma::slider("Ammo Value", weapon_left_min, 405.0f, 432.0f, weapon_ammo_value, weapon_ammo_target_value, weapon_ammo_dragging, 250.0f, "%.0f", content_alpha, ammo_controls_interactive);
                        sigma::button("Add Ammo", ImVec2(weapon_left_min.x + 13.0f, 457.0f), ImVec2(265.0f, 33.0f), weapon_button_hover_fade[0], 12.0f, content_alpha, ammo_controls_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float attachment_dropdown_step = io.DeltaTime * 3.8f;
                        weapon_attachment_dropdown_fade += weapon_attachment_dropdown_open ? attachment_dropdown_step : -attachment_dropdown_step;
                        if (weapon_attachment_dropdown_fade < 0.0f)
                            weapon_attachment_dropdown_fade = 0.0f;
                        if (weapon_attachment_dropdown_fade > 1.0f)
                            weapon_attachment_dropdown_fade = 1.0f;
                        const float attachment_dropdown_eased = smooth_fade(weapon_attachment_dropdown_fade);
                        if (attachment_dropdown_eased > 0.005f)
                        {
                            const float dropdown_slide_offset = (1.0f - attachment_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(attachment_dropdown_min.x, attachment_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(attachment_dropdown_max.x, attachment_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = attachment_dropdown_size.y * attachment_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * attachment_dropdown_eased);
                            const float attachment_item_height = 28.0f;
                            const float attachment_content_height = 14.0f + IM_ARRAYSIZE(attachment_dropdown_items) * attachment_item_height;
                            const float attachment_max_scroll = attachment_content_height > attachment_dropdown_size.y ? attachment_content_height - attachment_dropdown_size.y : 0.0f;
                            if (content_interactive && !weapon_tuning_popup_active && weapon_attachment_dropdown_open && attachment_dropdown_hovered && io.MouseWheel != 0.0f)
                                weapon_attachment_dropdown_target_scroll -= io.MouseWheel * 35.0f;
                            if (weapon_attachment_dropdown_target_scroll < 0.0f)
                                weapon_attachment_dropdown_target_scroll = 0.0f;
                            if (weapon_attachment_dropdown_target_scroll > attachment_max_scroll)
                                weapon_attachment_dropdown_target_scroll = attachment_max_scroll;
                            weapon_attachment_dropdown_scroll += (weapon_attachment_dropdown_target_scroll - weapon_attachment_dropdown_scroll) * attachment_dropdown_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            ImDrawList* weapon_left_draw_list = ImGui::GetBackgroundDrawList();
                            weapon_left_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            weapon_left_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            weapon_left_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            weapon_left_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            for (int attachment_index = 0; attachment_index < IM_ARRAYSIZE(attachment_dropdown_items); ++attachment_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + attachment_index * attachment_item_height - weapon_attachment_dropdown_scroll;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + attachment_item_height);
                                const bool item_visible = item_max.y >= dropdown_draw_min.y && item_min.y <= visible_dropdown_max.y;
                                const bool item_hovered =
                                    item_visible &&
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && !weapon_tuning_popup_active && weapon_attachment_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    weapon_attachment_selected_index = attachment_index;
                                    weapon_attachment_dropdown_open = false;
                                }
                                if (content_interactive && !weapon_tuning_popup_active && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (!item_visible)
                                    continue;
                                const bool item_selected = attachment_index == weapon_attachment_selected_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                weapon_attachment_text_fade[attachment_index] += (item_target - weapon_attachment_text_fade[attachment_index]) * attachment_dropdown_step;
                                const float item_fade = smooth_fade(weapon_attachment_text_fade[attachment_index]);
                                if (item_selected || item_hovered)
                                    weapon_left_draw_list->AddRectFilled(item_min, item_max, item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha), 2.0f);
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    weapon_left_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 7.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                weapon_left_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 7.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    attachment_dropdown_items[attachment_index]);
                            }
                            weapon_left_draw_list->PopClipRect();
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (macro_subtab)
                    {
                        const ImVec2 macro_left_min = fallback_left_child_min;
                        sigma::child_header(macro_left_min, fallback_left_child_size, u8"\uf3dc", "Activation & Visual", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const char* macro_option_labels[] = {
                            "Enable",
                            "Show Strafe Circle",
                            "Show Only While Active"
                        };
                        const bool macro_enable_popup_active = macro_enable_popup_open || macro_enable_popup_fade > 0.005f;
                        for (int option_index = 0; option_index < IM_ARRAYSIZE(macro_option_labels); ++option_index)
                        {
                            const float row_y = macro_left_min.y + 57.0f + option_index * 29.0f;
                            char option_id[48];
                            ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##macro_activation_%d", option_index);
                            const bool enable_row = option_index == 0;
                            const ImVec2 row_min(macro_left_min.x + 16.0f, row_y - 9.0f);
                            const ImVec2 row_size(267.0f, 29.0f);
                            const ImVec2 label_pos(macro_left_min.x + 13.0f, row_y);
                            const ImVec2 toggle_min(macro_left_min.x + 249.0f, row_y - 2.0f);
                            if (!enable_row)
                            {
                                sigma::checkbox(
                                    option_id,
                                    macro_option_labels[option_index],
                                    row_min,
                                    row_size,
                                    label_pos,
                                    toggle_min,
                                    macro_activation_enabled[option_index],
                                    macro_activation_toggle_fade[option_index],
                                    macro_activation_hover_fade[option_index],
                                    13.0f,
                                    false,
                                    content_alpha,
                                    content_interactive && !macro_enable_popup_active);
                            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            if (enable_row)
                            {
                                const ImVec2 label_hit_min(macro_left_min.x + 16.0f, row_y - 9.0f);
                                const ImVec2 label_hit_size(204.0f, 29.0f);
                                const ImVec2 toggle_hit_min(macro_left_min.x + 244.0f, row_y - 8.0f);
                                const ImVec2 toggle_hit_size(34.0f, 27.0f);
                                const ImVec2 label_hit_max(label_hit_min.x + label_hit_size.x, label_hit_min.y + label_hit_size.y);
                                const ImVec2 toggle_hit_max(toggle_hit_min.x + toggle_hit_size.x, toggle_hit_min.y + toggle_hit_size.y);
                                bool enable_row_hovered = false;
                                if (content_interactive && !macro_enable_popup_active)
                                {
                                    enable_row_hovered =
                                        mouse_in_rect(io.MousePos, label_hit_min, label_hit_max) ||
                                        mouse_in_rect(io.MousePos, toggle_hit_min, toggle_hit_max);
                                    if (enable_row_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                        macro_activation_enabled[0] = !macro_activation_enabled[0];
                                    if (enable_row_hovered)
                                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                }
                                macro_activation_hover_fade[option_index] += ((enable_row_hovered ? 1.0f : 0.0f) - macro_activation_hover_fade[option_index]) * (io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const ImVec2 gear_pos(macro_left_min.x + 224.0f, row_y - 3.0f);
                                const ImVec2 gear_center(gear_pos.x + 6.5f, gear_pos.y + 7.0f);
                                const ImVec2 gear_hit_size(24.0f, 24.0f);
                                const ImVec2 gear_hit_min(gear_center.x - gear_hit_size.x * 0.5f, gear_center.y - gear_hit_size.y * 0.5f);
                                const ImVec2 gear_hit_max(gear_hit_min.x + gear_hit_size.x, gear_hit_min.y + gear_hit_size.y);
                                bool gear_hovered = false;
                                bool gear_clicked = false;
                                if (content_interactive)
                                {
                                    gear_hovered =
                                        io.MousePos.x >= gear_hit_min.x && io.MousePos.x <= gear_hit_max.x &&
                                        io.MousePos.y >= gear_hit_min.y && io.MousePos.y <= gear_hit_max.y;
                                    gear_clicked = gear_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                    if (gear_hovered)
                                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                }
                                else
                                {
                                    macro_enable_popup_open = false;
                                    macro_enable_keybind_waiting = false;
                                }
                                if (gear_clicked)
                                {
                                    macro_enable_popup_open = !macro_enable_popup_open;
                                    macro_tuning_mode_dropdown_open = false;
                                    macro_preset_dropdown_open = false;
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                sigma::checkbox(
                                    option_id,
                                    macro_option_labels[option_index],
                                    row_min,
                                    row_size,
                                    label_pos,
                                    toggle_min,
                                    macro_activation_enabled[option_index],
                                    macro_activation_toggle_fade[option_index],
                                    macro_activation_hover_fade[option_index],
                                    13.0f,
                                    false,
                                    content_alpha,
                                    false,
                                    true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float gear_step = io.DeltaTime * 14.0f > 1.0f ? 1.0f : io.DeltaTime * 14.0f;
                                macro_enable_gear_hover_fade += (((gear_hovered || macro_enable_popup_open) ? 1.0f : 0.0f) - macro_enable_gear_hover_fade) * gear_step;
                                const float gear_hover = smooth_fade(macro_enable_gear_hover_fade);
                                const int gear_r = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_g = static_cast<int>(118.0f + (255.0f - 118.0f) * gear_hover);
                                const int gear_b = static_cast<int>(124.0f + (255.0f - 124.0f) * gear_hover);
                                const int gear_alpha = static_cast<int>((150.0f + 55.0f * gear_hover) * content_alpha);
                                ImGui::GetBackgroundDrawList()->AddText(icon_font, 13.5f, gear_pos, IM_COL32(gear_r, gear_g, gear_b, gear_alpha), u8"\uf3dc");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const float popup_step = io.DeltaTime * 13.0f > 1.0f ? 1.0f : io.DeltaTime * 13.0f;
                                macro_enable_popup_fade += ((macro_enable_popup_open ? 1.0f : 0.0f) - macro_enable_popup_fade) * popup_step;
                                if (macro_enable_popup_fade < 0.01f)
                                    macro_enable_popup_fade = 0.0f;
                                if (macro_enable_popup_fade > 0.0f)
                                {
                                    const float popup_eased = smooth_fade(macro_enable_popup_fade);
                                    const float popup_alpha_f = content_alpha * popup_eased;
                                    const int popup_alpha = static_cast<int>(255.0f * popup_alpha_f);
                                    const float popup_slide_y = (1.0f - popup_eased) * -8.0f;
                                    const ImVec2 popup_size(210.0f, 94.0f);
                                    const ImVec2 popup_min(gear_center.x - 8.0f, gear_center.y + 28.0f + popup_slide_y);
                                    const ImVec2 popup_max(popup_min.x + popup_size.x, popup_min.y + popup_size.y);
                                    ImDrawList* popup_draw_list = ImGui::GetForegroundDrawList();
                                    popup_draw_list->AddRectFilled(
                                        ImVec2(popup_min.x + 3.0f, popup_min.y + 5.0f),
                                        ImVec2(popup_max.x + 4.0f, popup_max.y + 6.0f),
                                        IM_COL32(0, 0, 0, static_cast<int>(80.0f * popup_alpha_f)),
                                        3.0f);
                                    popup_draw_list->AddRectFilled(popup_min, popup_max, IM_COL32(14, 14, 14, popup_alpha), 3.0f);
                                    popup_draw_list->AddRect(popup_min, popup_max, IM_COL32(28, 28, 30, static_cast<int>(235.0f * popup_alpha_f)), 3.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const auto macro_keybind_name = [](ImGuiKey key) -> const char*
                                    {
                                        switch (key)
                                        {
                                        case ImGuiKey_None: return "NONE";
                                        case ImGuiKey_MouseLeft: return "M1";
                                        case ImGuiKey_MouseRight: return "M2";
                                        case ImGuiKey_MouseMiddle: return "M3";
                                        case ImGuiKey_MouseX1: return "M4";
                                        case ImGuiKey_MouseX2: return "M5";
                                        default: break;
                                        }
                                        const char* name = ImGui::GetKeyName(key);
                                        return name && name[0] ? name : "NONE";
                                    };
                                    const auto capture_macro_keybind = [&]()
                                    {
                                        if (!macro_enable_keybind_waiting || ImGui::GetFrameCount() <= macro_enable_keybind_wait_frame)
                                            return;
                                        bool captured = false;
                                        ImGuiKey captured_key = ImGuiKey_None;
                                        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_None;
                                        }
                                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_MouseLeft;
                                        }
                                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_MouseRight;
                                        }
                                        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_MouseMiddle;
                                        }
                                        else if (ImGui::IsMouseClicked(3))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_MouseX1;
                                        }
                                        else if (ImGui::IsMouseClicked(4))
                                        {
                                            captured = true;
                                            captured_key = ImGuiKey_MouseX2;
                                        }
                                        else
                                        {
                                            for (int key_index = ImGuiKey_NamedKey_BEGIN; key_index < ImGuiKey_NamedKey_END; ++key_index)
                                            {
                                                const ImGuiKey key = static_cast<ImGuiKey>(key_index);
                                                if (key == ImGuiKey_MouseLeft || key == ImGuiKey_MouseRight || key == ImGuiKey_MouseMiddle || key == ImGuiKey_MouseX1 || key == ImGuiKey_MouseX2 || key == ImGuiKey_MouseWheelX || key == ImGuiKey_MouseWheelY)
                                                    continue;
                                                if (ImGui::IsKeyPressed(key, false))
                                                {
                                                    captured = true;
                                                    captured_key = key;
                                                    break;
                                                }
                                            }
                                        }
                                        if (captured)
                                        {
                                            macro_enable_keybind = captured_key;
                                            macro_enable_keybind_waiting = false;
                                        }
                                    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    popup_draw_list->AddText(title_font, 13.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + 24.0f), IM_COL32(214, 214, 220, popup_alpha), "Keybind");
                                    const char* key_value = macro_enable_keybind_waiting ? "..." : macro_keybind_name(macro_enable_keybind);
                                    const char* key_icon = u8"\uf450";
                                    const float key_icon_size = 12.0f;
                                    const float key_text_size = 11.0f;
                                    const ImVec2 key_icon_measure = icon_font->CalcTextSizeA(key_icon_size, 1000.0f, 0.0f, key_icon);
                                    const ImVec2 key_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, key_value);
                                    const float key_gap = 7.0f;
                                    const float key_content_width = key_icon_measure.x + key_gap + key_text_measure.x;
                                    const float key_button_width = key_content_width + 18.0f > 74.0f ? key_content_width + 18.0f : 74.0f;
                                    const ImVec2 key_button_size(key_button_width, 32.0f);
                                    const ImVec2 key_button_min(popup_max.x - key_button_size.x - 16.0f, popup_min.y + 13.0f);
                                    const ImVec2 key_button_max(key_button_min.x + key_button_size.x, key_button_min.y + key_button_size.y);
                                    bool key_hovered = false;
                                    bool key_clicked = false;
                                    if (content_interactive)
                                    {
                                        key_hovered = mouse_in_rect(io.MousePos, key_button_min, key_button_max);
                                        key_clicked = key_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                                        if (key_hovered)
                                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                    }
                                    if (key_clicked && !macro_enable_keybind_waiting)
                                    {
                                        macro_enable_keybind_waiting = true;
                                        macro_enable_keybind_wait_frame = ImGui::GetFrameCount();
                                    }
                                    capture_macro_keybind();
                                    key_value = macro_enable_keybind_waiting ? "..." : macro_keybind_name(macro_enable_keybind);
                                    const ImVec2 draw_key_text_measure = medium_font->CalcTextSizeA(key_text_size, 1000.0f, 0.0f, key_value);
                                    const float draw_key_content_width = key_icon_measure.x + key_gap + draw_key_text_measure.x;
                                    macro_enable_key_hover_fade += (((key_hovered || macro_enable_keybind_waiting) ? 1.0f : 0.0f) - macro_enable_key_hover_fade) * popup_step;
                                    const float key_hover = smooth_fade(macro_enable_key_hover_fade);
                                    const int key_fill = static_cast<int>(24.0f + 8.0f * key_hover);
                                    const int key_stroke = static_cast<int>(31.0f + 18.0f * key_hover);
                                    popup_draw_list->AddRectFilled(key_button_min, key_button_max, IM_COL32(key_fill, key_fill, key_fill, popup_alpha), 2.0f);
                                    popup_draw_list->AddRect(key_button_min, key_button_max, IM_COL32(key_stroke, key_stroke, key_stroke, popup_alpha), 2.0f);
                                    const float key_start_x = key_button_min.x + (key_button_size.x - draw_key_content_width) * 0.5f;
                                    const float key_center_y = key_button_min.y + key_button_size.y * 0.5f;
                                    popup_draw_list->AddText(icon_font, key_icon_size, ImVec2(key_start_x, key_center_y - key_icon_measure.y * 0.5f), IM_COL32(142, 134, 246, popup_alpha), key_icon);
                                    popup_draw_list->AddText(medium_font, key_text_size, ImVec2(key_start_x + key_icon_measure.x + key_gap, key_center_y - draw_key_text_measure.y * 0.5f), IM_COL32(205, 205, 210, popup_alpha), key_value);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const float require_y = 66.0f;
                                    const ImVec2 require_hit_min(popup_min.x + 16.0f, popup_min.y + require_y - 8.0f);
                                    const ImVec2 require_hit_size(popup_size.x - 32.0f, 28.0f);
                                    const ImVec2 require_hit_max(require_hit_min.x + require_hit_size.x, require_hit_min.y + require_hit_size.y);
                                    bool require_hovered = false;
                                    if (content_interactive)
                                    {
                                        require_hovered = mouse_in_rect(io.MousePos, require_hit_min, require_hit_max);
                                        if (require_hovered)
                                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                                        if (require_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                                            macro_require_shift = !macro_require_shift;
                                    }
                                    macro_require_shift_toggle_fade += ((macro_require_shift ? 1.0f : 0.0f) - macro_require_shift_toggle_fade) * popup_step;
                                    const float require_enabled = smooth_fade(macro_require_shift_toggle_fade);
                                    const int require_text_value = require_hovered || macro_require_shift ? 150 : 88;
                                    popup_draw_list->AddText(title_font, 12.0f, ImVec2(popup_min.x + 16.0f, popup_min.y + require_y), IM_COL32(require_text_value, require_text_value, require_text_value + 6, popup_alpha), "Require Shift");
                                    const ImVec2 require_toggle_min(popup_max.x - 43.0f, popup_min.y + require_y - 2.0f);
                                    const ImVec2 require_toggle_size(25.0f, 15.0f);
                                    const int track_r = static_cast<int>(32.0f + (148.0f - 32.0f) * require_enabled);
                                    const int track_g = static_cast<int>(32.0f + (140.0f - 32.0f) * require_enabled);
                                    const int track_b = static_cast<int>(34.0f + (246.0f - 34.0f) * require_enabled);
                                    const int knob_value = static_cast<int>(86.0f + (10.0f - 86.0f) * require_enabled);
                                    const float knob_x = require_toggle_min.x + 7.5f + (require_toggle_size.x - 15.0f) * require_enabled;
                                    popup_draw_list->AddRectFilled(require_toggle_min, ImVec2(require_toggle_min.x + require_toggle_size.x, require_toggle_min.y + require_toggle_size.y), IM_COL32(track_r, track_g, track_b, popup_alpha), require_toggle_size.y * 0.5f);
                                    popup_draw_list->AddCircleFilled(ImVec2(knob_x, require_toggle_min.y + require_toggle_size.y * 0.5f), require_hovered ? 5.0f : 4.6f, IM_COL32(knob_value, knob_value, knob_value, popup_alpha), 24);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                    const bool popup_hovered = ImGui::IsMouseHoveringRect(popup_min, popup_max, false);
                                    if (macro_enable_popup_open && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !popup_hovered && !gear_hovered && !gear_clicked)
                                    {
                                        macro_enable_popup_open = false;
                                        macro_enable_keybind_waiting = false;
                                    }
                                }
                            }
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool macro_left_controls_interactive = content_interactive && !macro_enable_popup_active;
                        sigma::slider("Circle Radius", macro_left_min, macro_left_min.y + 151.0f, macro_left_min.y + 173.0f, macro_circle_radius_value, macro_circle_radius_target_value, macro_circle_radius_dragging, 120.0f, "%.0f", content_alpha, macro_left_controls_interactive);
                        sigma::slider("Circle Thickness", macro_left_min, macro_left_min.y + 202.0f, macro_left_min.y + 224.0f, macro_circle_thickness_value, macro_circle_thickness_target_value, macro_circle_thickness_dragging, 5.0f, "%.1f", content_alpha, macro_left_controls_interactive);
                        sigma::slider("Circle Segments", macro_left_min, macro_left_min.y + 253.0f, macro_left_min.y + 275.0f, macro_circle_segments_value, macro_circle_segments_target_value, macro_circle_segments_dragging, 128.0f, "%.0f", content_alpha, macro_left_controls_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        ImDrawList* macro_draw_list = ImGui::GetBackgroundDrawList();
                        const int macro_alpha = static_cast<int>(255.0f * content_alpha);
                        macro_draw_list->AddText(title_font, 13.0f, ImVec2(macro_left_min.x + 13.0f, macro_left_min.y + 304.0f), IM_COL32(205, 205, 210, macro_alpha), "Circle Color");
                        macro_draw_list->AddCircleFilled(ImVec2(macro_left_min.x + 272.0f, macro_left_min.y + 310.0f), 8.0f, IM_COL32(106, 18, 14, macro_alpha), 24);
                        macro_draw_list->AddCircle(ImVec2(macro_left_min.x + 272.0f, macro_left_min.y + 310.0f), 8.0f, IM_COL32(210, 38, 28, macro_alpha), 24, 1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 macro_tuning_min(macro_left_min.x, macro_left_min.y + fallback_left_child_size.y + 13.0f);
                        const ImVec2 macro_tuning_size(293.0f, 257.0f);
                        sigma::child(macro_tuning_min, macro_tuning_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
                        sigma::child_header(macro_tuning_min, macro_tuning_size, u8"\uf0f0", "Macro Tuning", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        macro_draw_list->AddText(
                            title_font,
                            14.0f,
                            ImVec2(macro_tuning_min.x + 13.0f, macro_tuning_min.y + 53.0f),
                            IM_COL32(235, 235, 235, macro_alpha),
                            "Mode");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const char* macro_tuning_mode_items[] = {
                            "WASD",
                            "WSDA",
                            "WSAD",
                            "DWAS (Best)",
                            "Recorded (4-step)"
                        };
                        const ImVec2 macro_mode_combo_min(macro_tuning_min.x + 13.0f, macro_tuning_min.y + 75.0f);
                        const ImVec2 macro_mode_combo_size(265.0f, 29.0f);
                        const ImVec2 macro_mode_combo_max(
                            macro_mode_combo_min.x + macro_mode_combo_size.x,
                            macro_mode_combo_min.y + macro_mode_combo_size.y);
                        const bool macro_mode_combo_hovered =
                            io.MousePos.x >= macro_mode_combo_min.x && io.MousePos.x <= macro_mode_combo_max.x &&
                            io.MousePos.y >= macro_mode_combo_min.y && io.MousePos.y <= macro_mode_combo_max.y;
                        sigma::combobox(
                            macro_tuning_mode_items[macro_tuning_mode_selected_index],
                            macro_mode_combo_min,
                            macro_mode_combo_size,
                            content_alpha,
                            false);
                        if (content_interactive && macro_mode_combo_hovered && io.MouseClicked[0])
                        {
                            macro_tuning_mode_dropdown_open = !macro_tuning_mode_dropdown_open;
                            macro_preset_dropdown_open = false;
                            teleport_location_dropdown_open = false;
                            weapon_attachment_dropdown_open = false;
                            weapon_spawner_category_dropdown_open = false;
                            weapon_spawner_weapon_dropdown_open = false;
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 macro_mode_dropdown_min(macro_mode_combo_min.x, macro_mode_combo_max.y + 7.0f);
                        const ImVec2 macro_mode_dropdown_size(265.0f, 150.0f);
                        const ImVec2 macro_mode_dropdown_max(
                            macro_mode_dropdown_min.x + macro_mode_dropdown_size.x,
                            macro_mode_dropdown_min.y + macro_mode_dropdown_size.y);
                        const bool macro_mode_dropdown_hovered =
                            io.MousePos.x >= macro_mode_dropdown_min.x && io.MousePos.x <= macro_mode_dropdown_max.x &&
                            io.MousePos.y >= macro_mode_dropdown_min.y && io.MousePos.y <= macro_mode_dropdown_max.y;
                        if (content_interactive && macro_tuning_mode_dropdown_open && io.MouseClicked[0] && !macro_mode_dropdown_hovered && !macro_mode_combo_hovered)
                            macro_tuning_mode_dropdown_open = false;
                        if (content_interactive && macro_mode_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool macro_mode_dropdown_active = macro_tuning_mode_dropdown_open || macro_tuning_mode_dropdown_fade > 0.005f;
                        const bool macro_tuning_controls_interactive = content_interactive && !macro_mode_dropdown_active;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        sigma::slider("Strafe Interval (ms)", macro_tuning_min, macro_tuning_min.y + 117.0f, macro_tuning_min.y + 139.0f, macro_strafe_interval_value, macro_strafe_interval_target_value, macro_strafe_interval_dragging, 200.0f, "%.0f ms", content_alpha, macro_tuning_controls_interactive);
                        sigma::slider("Release Gap (ms)", macro_tuning_min, macro_tuning_min.y + 161.0f, macro_tuning_min.y + 183.0f, macro_release_gap_value, macro_release_gap_target_value, macro_release_gap_dragging, 200.0f, "%.0f ms", content_alpha, macro_tuning_controls_interactive);
                        sigma::slider("Humanize Jitter (ms)", macro_tuning_min, macro_tuning_min.y + 205.0f, macro_tuning_min.y + 227.0f, macro_humanize_jitter_value, macro_humanize_jitter_target_value, macro_humanize_jitter_dragging, 200.0f, "%.0f ms", content_alpha, macro_tuning_controls_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float macro_mode_dropdown_step = io.DeltaTime * 3.8f;
                        macro_tuning_mode_dropdown_fade += macro_tuning_mode_dropdown_open ? macro_mode_dropdown_step : -macro_mode_dropdown_step;
                        if (macro_tuning_mode_dropdown_fade < 0.0f)
                            macro_tuning_mode_dropdown_fade = 0.0f;
                        if (macro_tuning_mode_dropdown_fade > 1.0f)
                            macro_tuning_mode_dropdown_fade = 1.0f;
                        const float macro_mode_dropdown_eased = smooth_fade(macro_tuning_mode_dropdown_fade);
                        if (macro_mode_dropdown_eased > 0.005f)
                        {
                            const float dropdown_slide_offset = (1.0f - macro_mode_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(macro_mode_dropdown_min.x, macro_mode_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(macro_mode_dropdown_max.x, macro_mode_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = macro_mode_dropdown_size.y * macro_mode_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * macro_mode_dropdown_eased);
                            macro_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            macro_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            macro_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            macro_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            const float mode_item_height = 28.0f;
                            for (int mode_index = 0; mode_index < IM_ARRAYSIZE(macro_tuning_mode_items); ++mode_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + mode_index * mode_item_height;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + mode_item_height);
                                const bool item_hovered =
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && macro_tuning_mode_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    macro_tuning_mode_selected_index = mode_index;
                                    macro_tuning_mode_dropdown_open = false;
                                }
                                if (content_interactive && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const bool item_selected = mode_index == macro_tuning_mode_selected_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                macro_tuning_mode_text_fade[mode_index] += (item_target - macro_tuning_mode_text_fade[mode_index]) * macro_mode_dropdown_step;
                                const float item_fade = smooth_fade(macro_tuning_mode_text_fade[mode_index]);
                                if (item_selected || item_hovered)
                                {
                                    macro_draw_list->AddRectFilled(
                                        item_min,
                                        item_max,
                                        item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                                        2.0f);
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    macro_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 7.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                macro_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 7.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    macro_tuning_mode_items[mode_index]);
                            }
                            macro_draw_list->PopClipRect();
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (displayed_top_tab == 0 && render_sub_tab == 2)
                    {
                        sigma::child_header(ImVec2(81.0f, 70.0f), ImVec2(293.0f, 314.0f), u8"\uf3dc", "Teleports", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 location_label_pos(94.0f, 126.0f);
                        const float location_label_size = 14.0f;
                        ImGui::GetBackgroundDrawList()->AddText(
                            title_font,
                            location_label_size,
                            location_label_pos,
                            IM_COL32(235, 235, 235, static_cast<int>(255.0f * content_alpha)),
                            "Location List");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 combo_min(94.0f, 148.0f);
                        const ImVec2 combo_size(265.0f, 29.0f);
                        const ImVec2 combo_max(combo_min.x + combo_size.x, combo_min.y + combo_size.y);
                        const char* teleport_location_items[] = {
                            "Waypoint",
                            "Square",
                            "Pier",
                            "Paleto Bay",
                            "Central Bank",
                            "Casino",
                            "Los Santos Airport"
                        };
                        const bool location_combo_hovered =
                            io.MousePos.x >= combo_min.x && io.MousePos.x <= combo_max.x &&
                            io.MousePos.y >= combo_min.y && io.MousePos.y <= combo_max.y;
                        sigma::combobox(
                            teleport_location_items[teleport_location_selected_index],
                            combo_min,
                            combo_size,
                            content_alpha,
                            false);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 location_dropdown_min(combo_min.x, combo_max.y + 7.0f);
                        const ImVec2 location_dropdown_size(265.0f, 190.0f);
                        const ImVec2 location_dropdown_max(
                            location_dropdown_min.x + location_dropdown_size.x,
                            location_dropdown_min.y + location_dropdown_size.y);
                        const bool location_dropdown_hovered =
                            io.MousePos.x >= location_dropdown_min.x && io.MousePos.x <= location_dropdown_max.x &&
                            io.MousePos.y >= location_dropdown_min.y && io.MousePos.y <= location_dropdown_max.y;
                        if (content_interactive && location_combo_hovered && io.MouseClicked[0])
                        {
                            teleport_location_dropdown_open = !teleport_location_dropdown_open;
                            weapon_attachment_dropdown_open = false;
                            weapon_spawner_category_dropdown_open = false;
                            weapon_spawner_weapon_dropdown_open = false;
                        }
                        if (content_interactive && teleport_location_dropdown_open && io.MouseClicked[0] && !location_dropdown_hovered && !location_combo_hovered)
                        {
                            teleport_location_dropdown_open = false;
                        }
                        if (content_interactive && location_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool teleport_location_dropdown_active =
                            teleport_location_dropdown_open || teleport_location_dropdown_fade > 0.005f;
                        const bool teleport_buttons_interactive = content_interactive && !teleport_location_dropdown_active;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 teleport_button_size(265.0f, 33.0f);
                        const float teleport_button_gap = 14.0f;
                        char teleport_selected_location_button_text[64];
                        sprintf_s(
                            teleport_selected_location_button_text,
                            IM_ARRAYSIZE(teleport_selected_location_button_text),
                            "Teleport to %s",
                            teleport_location_items[teleport_location_selected_index]);
                        const char* teleport_button_texts[] = {
                            teleport_selected_location_button_text,
                            "Teleport To Coords",
                            "Teleport to Nearby Player",
                            "Teleport to Nearby Vehicle"
                        };
                        for (int button_index = 0; button_index < IM_ARRAYSIZE(teleport_button_texts); ++button_index)
                        {
                            const ImVec2 teleport_button_min(
                                combo_min.x,
                                combo_max.y + teleport_button_gap + button_index * (teleport_button_size.y + teleport_button_gap));
                            sigma::button(teleport_button_texts[button_index], teleport_button_min, teleport_button_size, teleport_button_hover_fade[button_index], 12.0f, content_alpha, teleport_buttons_interactive);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float location_dropdown_step = io.DeltaTime * 3.8f;
                        teleport_location_dropdown_fade += teleport_location_dropdown_open ? location_dropdown_step : -location_dropdown_step;
                        if (teleport_location_dropdown_fade < 0.0f)
                            teleport_location_dropdown_fade = 0.0f;
                        if (teleport_location_dropdown_fade > 1.0f)
                            teleport_location_dropdown_fade = 1.0f;
                        const float location_dropdown_eased = smooth_fade(teleport_location_dropdown_fade);
                        if (location_dropdown_eased > 0.005f)
                        {
                            ImDrawList* teleport_draw_list = ImGui::GetBackgroundDrawList();
                            const float dropdown_slide_offset = (1.0f - location_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(location_dropdown_min.x, location_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(location_dropdown_max.x, location_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = location_dropdown_size.y * location_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * location_dropdown_eased);
                            teleport_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            teleport_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            teleport_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            teleport_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            const float location_item_height = 25.0f;
                            for (int location_index = 0; location_index < IM_ARRAYSIZE(teleport_location_items); ++location_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + location_index * location_item_height;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + location_item_height);
                                const bool item_hovered =
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && teleport_location_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    teleport_location_selected_index = location_index;
                                    teleport_location_dropdown_open = false;
                                }
                                if (content_interactive && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const bool item_selected = location_index == teleport_location_selected_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                teleport_location_text_fade[location_index] += (item_target - teleport_location_text_fade[location_index]) * location_dropdown_step;
                                const float item_fade = smooth_fade(teleport_location_text_fade[location_index]);
                                if (item_selected || item_hovered)
                                {
                                    teleport_draw_list->AddRectFilled(
                                        item_min,
                                        item_max,
                                        item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                                        2.0f);
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    teleport_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 6.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                teleport_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 6.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    teleport_location_items[location_index]);
                            }
                            teleport_draw_list->PopClipRect();
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    sigma::child(fallback_right_child_min, fallback_right_child_size, 3.0f, IM_COL32(14, 14, 14, 255), IM_COL32(22, 22, 22, 255), IM_COL32(5, 5, 5, 255), content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (weapon_subtab)
                    {
                        const ImVec2 weapon_right_min(392.0f, 70.0f);
                        sigma::child_header(weapon_right_min, fallback_right_child_size, u8"\uf677", "Weapon Spawner", content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        sigma::search_box_input(
                            "##weapon_search_input",
                            ImVec2(weapon_right_min.x + 13.0f, 126.0f),
                            ImVec2(265.0f, 33.0f),
                            weapon_search,
                            IM_ARRAYSIZE(weapon_search),
                            "Search weapon...",
                            u8"\uf5ae",
                            content_alpha,
                            content_interactive,
                            24,
                            24,
                            24);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        ImDrawList* weapon_draw_list = ImGui::GetBackgroundDrawList();
                        const int text_alpha = static_cast<int>(255.0f * content_alpha);
                        weapon_draw_list->AddText(title_font, 13.0f, ImVec2(weapon_right_min.x + 13.0f, 175.0f), IM_COL32(205, 205, 210, text_alpha), "Category");
                        const char* category_dropdown_items[] = {
                            "Shotguns",
                            "Assault Rifles",
                            "Machine Guns",
                            "Sniper Rifles",
                            "Heavy Weapons",
                            "Throwables",
                            "Misc"
                        };
                        const ImVec2 category_dropdown_combo_min(weapon_right_min.x + 13.0f, 195.0f);
                        const ImVec2 category_dropdown_combo_size(265.0f, 29.0f);
                        const bool category_combo_hovered =
                            io.MousePos.x >= category_dropdown_combo_min.x && io.MousePos.x <= category_dropdown_combo_min.x + category_dropdown_combo_size.x &&
                            io.MousePos.y >= category_dropdown_combo_min.y && io.MousePos.y <= category_dropdown_combo_min.y + category_dropdown_combo_size.y;
                        sigma::combobox(
                            category_dropdown_items[weapon_spawner_selected_category_index],
                            category_dropdown_combo_min,
                            category_dropdown_combo_size,
                            content_alpha,
                            false);
                        if (content_interactive && category_combo_hovered && io.MouseClicked[0])
                        {
                            weapon_spawner_category_dropdown_open = !weapon_spawner_category_dropdown_open;
                            weapon_spawner_weapon_dropdown_open = false;
                            weapon_attachment_dropdown_open = false;
                        }
                        const ImVec2 category_dropdown_min(category_dropdown_combo_min.x, category_dropdown_combo_min.y + category_dropdown_combo_size.y + 7.0f);
                        const ImVec2 category_dropdown_size(265.0f, 190.0f);
                        const ImVec2 category_dropdown_max(category_dropdown_min.x + category_dropdown_size.x, category_dropdown_min.y + category_dropdown_size.y);
                        const bool category_dropdown_hovered =
                            io.MousePos.x >= category_dropdown_min.x && io.MousePos.x <= category_dropdown_max.x &&
                            io.MousePos.y >= category_dropdown_min.y && io.MousePos.y <= category_dropdown_max.y;
                        const bool category_dropdown_blocks_click =
                            (weapon_spawner_category_dropdown_open || weapon_spawner_category_dropdown_fade > 0.005f) &&
                            category_dropdown_hovered;
                        if (content_interactive && weapon_spawner_category_dropdown_open && io.MouseClicked[0] && !category_dropdown_hovered && !category_combo_hovered)
                        {
                            weapon_spawner_category_dropdown_open = false;
                        }
                        if (content_interactive && category_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        weapon_draw_list->AddText(title_font, 13.0f, ImVec2(weapon_right_min.x + 13.0f, 237.0f), IM_COL32(205, 205, 210, text_alpha), "Weapon List");
                        const char* weapon_dropdown_items[] = {
                            "Sticky Bomb",
                            "Proximity Mine",
                            "Snowball",
                            "Pipe Bomb",
                            "Ball",
                            "Smoke Grenade",
                            "Flare"
                        };
                        const ImVec2 weapon_dropdown_combo_min(weapon_right_min.x + 13.0f, 257.0f);
                        const ImVec2 weapon_dropdown_combo_size(265.0f, 29.0f);
                        const bool weapon_combo_hovered =
                            io.MousePos.x >= weapon_dropdown_combo_min.x && io.MousePos.x <= weapon_dropdown_combo_min.x + weapon_dropdown_combo_size.x &&
                            io.MousePos.y >= weapon_dropdown_combo_min.y && io.MousePos.y <= weapon_dropdown_combo_min.y + weapon_dropdown_combo_size.y;
                        sigma::combobox(
                            weapon_dropdown_items[weapon_spawner_selected_weapon_index],
                            weapon_dropdown_combo_min,
                            weapon_dropdown_combo_size,
                            content_alpha,
                            false);
                        if (content_interactive && !category_dropdown_blocks_click && weapon_combo_hovered && io.MouseClicked[0])
                        {
                            weapon_spawner_weapon_dropdown_open = !weapon_spawner_weapon_dropdown_open;
                            weapon_spawner_category_dropdown_open = false;
                            weapon_attachment_dropdown_open = false;
                        }
                        const ImVec2 weapon_dropdown_min(weapon_dropdown_combo_min.x, weapon_dropdown_combo_min.y + weapon_dropdown_combo_size.y + 7.0f);
                        const ImVec2 weapon_dropdown_size(265.0f, 190.0f);
                        const ImVec2 weapon_dropdown_max(weapon_dropdown_min.x + weapon_dropdown_size.x, weapon_dropdown_min.y + weapon_dropdown_size.y);
                        const bool weapon_dropdown_hovered =
                            io.MousePos.x >= weapon_dropdown_min.x && io.MousePos.x <= weapon_dropdown_max.x &&
                            io.MousePos.y >= weapon_dropdown_min.y && io.MousePos.y <= weapon_dropdown_max.y;
                        if (content_interactive && weapon_spawner_weapon_dropdown_open && io.MouseClicked[0] && !weapon_dropdown_hovered && !weapon_combo_hovered)
                        {
                            weapon_spawner_weapon_dropdown_open = false;
                        }
                        if (content_interactive && !category_dropdown_blocks_click && weapon_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool weapon_spawner_dropdown_active =
                            weapon_spawner_weapon_dropdown_open || weapon_spawner_weapon_dropdown_fade > 0.005f ||
                            weapon_spawner_category_dropdown_open || weapon_spawner_category_dropdown_fade > 0.005f;
                        const bool weapon_spawner_controls_interactive = content_interactive && !weapon_spawner_dropdown_active;
                        {
                            const char* weapon_spawner_labels[] = { "Weapon Bypass", "Force in-Hand" };
                            for (int option_index = 0; option_index < IM_ARRAYSIZE(weapon_spawner_labels); ++option_index)
                            {
                                const float row_y = 300.0f + option_index * 29.0f;
                                char option_id[48];
                                ImFormatString(option_id, IM_ARRAYSIZE(option_id), "##weapon_spawner_%d", option_index);
                                sigma::checkbox(
                                    option_id,
                                    weapon_spawner_labels[option_index],
                                    ImVec2(weapon_right_min.x + 16.0f, row_y - 9.0f),
                                    ImVec2(267.0f, 29.0f),
                                    ImVec2(weapon_right_min.x + 13.0f, row_y),
                                    ImVec2(weapon_right_min.x + 249.0f, row_y - 2.0f),
                                    weapon_spawner_enabled[option_index],
                                    weapon_spawner_toggle_fade[option_index],
                                    weapon_spawner_hover_fade[option_index],
                                    13.0f,
                                    false,
                                    content_alpha,
                                    weapon_spawner_controls_interactive);
                            }
                            sigma::slider("Ammo", weapon_right_min, 357.0f, 384.0f, weapon_give_ammo_value, weapon_give_ammo_target_value, weapon_give_ammo_dragging, 250.0f, "%.0f", content_alpha, weapon_spawner_controls_interactive);
                            sigma::button("Give Weapon", ImVec2(weapon_right_min.x + 13.0f, 409.0f), ImVec2(265.0f, 33.0f), weapon_button_hover_fade[1], 12.0f, content_alpha, weapon_spawner_controls_interactive);
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float weapon_dropdown_step = io.DeltaTime * 3.8f;
                        weapon_spawner_weapon_dropdown_fade += weapon_spawner_weapon_dropdown_open ? weapon_dropdown_step : -weapon_dropdown_step;
                        if (weapon_spawner_weapon_dropdown_fade < 0.0f)
                            weapon_spawner_weapon_dropdown_fade = 0.0f;
                        if (weapon_spawner_weapon_dropdown_fade > 1.0f)
                            weapon_spawner_weapon_dropdown_fade = 1.0f;
                        const float weapon_dropdown_eased = smooth_fade(weapon_spawner_weapon_dropdown_fade);
                        if (weapon_dropdown_eased > 0.005f)
                        {
                            const float dropdown_slide_offset = (1.0f - weapon_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(weapon_dropdown_min.x, weapon_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(weapon_dropdown_max.x, weapon_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = weapon_dropdown_size.y * weapon_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * weapon_dropdown_eased);
                            weapon_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            weapon_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            weapon_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            weapon_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            const float weapon_item_height = 25.0f;
                            for (int weapon_index = 0; weapon_index < IM_ARRAYSIZE(weapon_dropdown_items); ++weapon_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + weapon_index * weapon_item_height;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + weapon_item_height);
                                const bool item_hovered =
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && weapon_spawner_weapon_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    weapon_spawner_selected_weapon_index = weapon_index;
                                    weapon_spawner_weapon_dropdown_open = false;
                                }
                                if (content_interactive && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const bool item_selected = weapon_index == weapon_spawner_selected_weapon_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                weapon_spawner_weapon_text_fade[weapon_index] += (item_target - weapon_spawner_weapon_text_fade[weapon_index]) * weapon_dropdown_step;
                                const float item_fade = smooth_fade(weapon_spawner_weapon_text_fade[weapon_index]);
                                if (item_selected || item_hovered)
                                {
                                    weapon_draw_list->AddRectFilled(
                                        item_min,
                                        item_max,
                                        item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                                        2.0f);
                                }
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    weapon_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 6.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                weapon_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 6.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    weapon_dropdown_items[weapon_index]);
                            }
                            weapon_draw_list->PopClipRect();
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float category_dropdown_step = io.DeltaTime * 3.8f;
                        weapon_spawner_category_dropdown_fade += weapon_spawner_category_dropdown_open ? category_dropdown_step : -category_dropdown_step;
                        if (weapon_spawner_category_dropdown_fade < 0.0f)
                            weapon_spawner_category_dropdown_fade = 0.0f;
                        if (weapon_spawner_category_dropdown_fade > 1.0f)
                            weapon_spawner_category_dropdown_fade = 1.0f;
                        const float category_dropdown_eased = smooth_fade(weapon_spawner_category_dropdown_fade);
                        if (category_dropdown_eased > 0.005f)
                        {
                            const float dropdown_slide_offset = (1.0f - category_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(category_dropdown_min.x, category_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(category_dropdown_max.x, category_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = category_dropdown_size.y * category_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * category_dropdown_eased);
                            weapon_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            weapon_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            weapon_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            weapon_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            const float category_item_height = 25.0f;
                            for (int category_index = 0; category_index < IM_ARRAYSIZE(category_dropdown_items); ++category_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + category_index * category_item_height;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + category_item_height);
                                const bool item_hovered =
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && weapon_spawner_category_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    weapon_spawner_selected_category_index = category_index;
                                    weapon_spawner_category_dropdown_open = false;
                                }
                                if (content_interactive && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const bool item_selected = category_index == weapon_spawner_selected_category_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                weapon_spawner_category_text_fade[category_index] += (item_target - weapon_spawner_category_text_fade[category_index]) * category_dropdown_step;
                                const float item_fade = smooth_fade(weapon_spawner_category_text_fade[category_index]);
                                if (item_selected || item_hovered)
                                {
                                    weapon_draw_list->AddRectFilled(
                                        item_min,
                                        item_max,
                                        item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                                        2.0f);
                                }
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    weapon_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 6.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                weapon_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 6.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    category_dropdown_items[category_index]);
                            }
                            weapon_draw_list->PopClipRect();
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (macro_subtab)
                    {
                        const ImVec2 macro_right_min = fallback_right_child_min;
                        sigma::child_header(macro_right_min, fallback_right_child_size, u8"\uf4ca", "Presets & Recording", content_alpha);
                        sigma::button(
                            "Record Macro (4 keys)",
                            ImVec2(macro_right_min.x + 13.0f, macro_right_min.y + 57.0f),
                            ImVec2(265.0f, 33.0f),
                            macro_button_hover_fade[0],
                            12.0f,
                            content_alpha,
                            content_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        ImGui::GetBackgroundDrawList()->AddText(
                            title_font,
                            14.0f,
                            ImVec2(macro_right_min.x + 13.0f, macro_right_min.y + 100.0f),
                            IM_COL32(235, 235, 235, static_cast<int>(255.0f * content_alpha)),
                            "Quick Preset");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const char* macro_preset_items[] = {
                            "W-A-S-D",
                            "D-A-A-W",
                            "A-D-A-D",
                            "D-A-D-A",
                            "W-D-S-A",
                            "D-W-A-S",
                            "W-A-W-D",
                            "S-D-S-A",
                            "W-D-W-A",
                            "A-S-D-W"
                        };
                        const ImVec2 macro_preset_combo_min(macro_right_min.x + 13.0f, macro_right_min.y + 122.0f);
                        const ImVec2 macro_preset_combo_size(265.0f, 29.0f);
                        const ImVec2 macro_preset_combo_max(
                            macro_preset_combo_min.x + macro_preset_combo_size.x,
                            macro_preset_combo_min.y + macro_preset_combo_size.y);
                        const bool macro_preset_combo_hovered =
                            io.MousePos.x >= macro_preset_combo_min.x && io.MousePos.x <= macro_preset_combo_max.x &&
                            io.MousePos.y >= macro_preset_combo_min.y && io.MousePos.y <= macro_preset_combo_max.y;
                        sigma::combobox(
                            macro_preset_items[macro_preset_selected_index],
                            macro_preset_combo_min,
                            macro_preset_combo_size,
                            content_alpha,
                            false);
                        if (content_interactive && macro_preset_combo_hovered && io.MouseClicked[0])
                        {
                            macro_preset_dropdown_open = !macro_preset_dropdown_open;
                            macro_tuning_mode_dropdown_open = false;
                            teleport_location_dropdown_open = false;
                            weapon_attachment_dropdown_open = false;
                            weapon_spawner_category_dropdown_open = false;
                            weapon_spawner_weapon_dropdown_open = false;
                        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 macro_preset_dropdown_min(macro_preset_combo_min.x, macro_preset_combo_max.y + 7.0f);
                        const ImVec2 macro_preset_dropdown_size(265.0f, 190.0f);
                        const ImVec2 macro_preset_dropdown_max(
                            macro_preset_dropdown_min.x + macro_preset_dropdown_size.x,
                            macro_preset_dropdown_min.y + macro_preset_dropdown_size.y);
                        const bool macro_preset_dropdown_hovered =
                            io.MousePos.x >= macro_preset_dropdown_min.x && io.MousePos.x <= macro_preset_dropdown_max.x &&
                            io.MousePos.y >= macro_preset_dropdown_min.y && io.MousePos.y <= macro_preset_dropdown_max.y;
                        if (content_interactive && macro_preset_dropdown_open && io.MouseClicked[0] && !macro_preset_dropdown_hovered && !macro_preset_combo_hovered)
                            macro_preset_dropdown_open = false;
                        if (content_interactive && macro_preset_combo_hovered)
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const bool macro_preset_dropdown_active = macro_preset_dropdown_open || macro_preset_dropdown_fade > 0.005f;
                        const bool macro_preset_buttons_interactive = content_interactive && !macro_preset_dropdown_active;
                        sigma::button(
                            "Apply Preset",
                            ImVec2(macro_right_min.x + 13.0f, macro_right_min.y + 165.0f),
                            ImVec2(265.0f, 33.0f),
                            macro_button_hover_fade[1],
                            12.0f,
                            content_alpha,
                            macro_preset_buttons_interactive);
                        sigma::button(
                            "Save As Named Preset",
                            ImVec2(macro_right_min.x + 13.0f, macro_right_min.y + 206.0f),
                            ImVec2(265.0f, 33.0f),
                            macro_button_hover_fade[2],
                            12.0f,
                            content_alpha,
                            macro_preset_buttons_interactive);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const float macro_preset_dropdown_step = io.DeltaTime * 3.8f;
                        macro_preset_dropdown_fade += macro_preset_dropdown_open ? macro_preset_dropdown_step : -macro_preset_dropdown_step;
                        if (macro_preset_dropdown_fade < 0.0f)
                            macro_preset_dropdown_fade = 0.0f;
                        if (macro_preset_dropdown_fade > 1.0f)
                            macro_preset_dropdown_fade = 1.0f;
                        const float macro_preset_dropdown_eased = smooth_fade(macro_preset_dropdown_fade);
                        if (macro_preset_dropdown_eased > 0.005f)
                        {
                            ImDrawList* macro_right_draw_list = ImGui::GetBackgroundDrawList();
                            const float dropdown_slide_offset = (1.0f - macro_preset_dropdown_eased) * -14.0f;
                            const ImVec2 dropdown_draw_min(macro_preset_dropdown_min.x, macro_preset_dropdown_min.y + dropdown_slide_offset);
                            const ImVec2 dropdown_draw_max(macro_preset_dropdown_max.x, macro_preset_dropdown_max.y + dropdown_slide_offset);
                            const float visible_height = macro_preset_dropdown_size.y * macro_preset_dropdown_eased;
                            const ImVec2 visible_dropdown_max(dropdown_draw_max.x, dropdown_draw_min.y + visible_height);
                            const int dropdown_alpha = static_cast<int>(255.0f * content_alpha * macro_preset_dropdown_eased);
                            const float preset_item_height = 28.0f;
                            const float preset_content_height = 14.0f + IM_ARRAYSIZE(macro_preset_items) * preset_item_height;
                            const float preset_max_scroll = preset_content_height > macro_preset_dropdown_size.y ? preset_content_height - macro_preset_dropdown_size.y : 0.0f;
                            if (content_interactive && macro_preset_dropdown_open && macro_preset_dropdown_hovered && io.MouseWheel != 0.0f)
                                macro_preset_dropdown_target_scroll -= io.MouseWheel * 35.0f;
                            if (macro_preset_dropdown_target_scroll < 0.0f)
                                macro_preset_dropdown_target_scroll = 0.0f;
                            if (macro_preset_dropdown_target_scroll > preset_max_scroll)
                                macro_preset_dropdown_target_scroll = preset_max_scroll;
                            macro_preset_dropdown_scroll += (macro_preset_dropdown_target_scroll - macro_preset_dropdown_scroll) * macro_preset_dropdown_step;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                            macro_right_draw_list->AddRectFilled(dropdown_draw_min, visible_dropdown_max, IM_COL32(24, 24, 24, dropdown_alpha), 3.0f);
                            macro_right_draw_list->AddRect(dropdown_draw_min, visible_dropdown_max, IM_COL32(26, 26, 26, dropdown_alpha), 3.0f, 0, 1.0f);
                            macro_right_draw_list->AddRect(
                                ImVec2(dropdown_draw_min.x - 1.0f, dropdown_draw_min.y - 1.0f),
                                ImVec2(visible_dropdown_max.x + 1.0f, visible_dropdown_max.y + 1.0f),
                                IM_COL32(15, 15, 15, dropdown_alpha),
                                4.0f,
                                0,
                                1.0f);
                            macro_right_draw_list->PushClipRect(dropdown_draw_min, visible_dropdown_max, true);
                            for (int preset_index = 0; preset_index < IM_ARRAYSIZE(macro_preset_items); ++preset_index)
                            {
                                const float item_y = dropdown_draw_min.y + 7.0f + preset_index * preset_item_height - macro_preset_dropdown_scroll;
                                const ImVec2 item_min(dropdown_draw_min.x + 7.0f, item_y);
                                const ImVec2 item_max(dropdown_draw_max.x - 7.0f, item_y + preset_item_height);
                                const bool item_visible = item_max.y >= dropdown_draw_min.y && item_min.y <= visible_dropdown_max.y;
                                const bool item_hovered =
                                    item_visible &&
                                    io.MousePos.x >= item_min.x && io.MousePos.x <= item_max.x &&
                                    io.MousePos.y >= item_min.y && io.MousePos.y <= item_max.y &&
                                    item_max.y <= visible_dropdown_max.y;
                                if (content_interactive && macro_preset_dropdown_open && item_hovered && io.MouseClicked[0])
                                {
                                    macro_preset_selected_index = preset_index;
                                    macro_preset_dropdown_open = false;
                                }
                                if (content_interactive && item_hovered)
                                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                if (!item_visible)
                                    continue;
                                const bool item_selected = preset_index == macro_preset_selected_index;
                                const float item_target = item_selected || item_hovered ? 1.0f : 0.0f;
                                macro_preset_text_fade[preset_index] += (item_target - macro_preset_text_fade[preset_index]) * macro_preset_dropdown_step;
                                const float item_fade = smooth_fade(macro_preset_text_fade[preset_index]);
                                if (item_selected || item_hovered)
                                {
                                    macro_right_draw_list->AddRectFilled(
                                        item_min,
                                        item_max,
                                        item_selected ? IM_COL32(31, 31, 31, dropdown_alpha) : IM_COL32(28, 28, 28, dropdown_alpha),
                                        2.0f);
                                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                                const int item_text_value = static_cast<int>(74.0f + (235.0f - 74.0f) * item_fade);
                                float item_text_x = item_min.x + 11.0f;
                                if (item_selected)
                                {
                                    macro_right_draw_list->AddText(
                                        icon_font,
                                        12.0f,
                                        ImVec2(item_min.x + 11.0f, item_min.y + 7.0f),
                                        IM_COL32(142, 134, 246, dropdown_alpha),
                                        u8"\uf313");
                                    item_text_x += 22.0f;
                                }
                                macro_right_draw_list->AddText(
                                    medium_font,
                                    13.0f,
                                    ImVec2(item_text_x, item_min.y + 7.0f),
                                    IM_COL32(item_text_value, item_text_value, item_text_value + 4 > 255 ? 255 : item_text_value + 4, dropdown_alpha),
                                    macro_preset_items[preset_index]);
                            }
                            macro_right_draw_list->PopClipRect();
                        }
                    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (macro_subtab)
                        fallback_draw_list->PopClipRect();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                    if (displayed_top_tab == 0 && render_sub_tab == 2)
                    {
                        sigma::child_header(ImVec2(392.0f, 70.0f), ImVec2(293.0f, 183.0f), u8"\uf0f0", "Location Manager", content_alpha);
                        sigma::checkbox(
                            "##saved_location_toggle_input",
                            "Show Saved Location",
                            ImVec2(408.0f, 118.0f),
                            ImVec2(267.0f, 29.0f),
                            ImVec2(405.0f, 127.0f),
                            ImVec2(641.0f, 125.0f),
                            saved_location_enabled,
                            saved_location_toggle_fade,
                            saved_location_toggle_hover_fade,
                            13.0f,
                            false,
                            content_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                        const ImVec2 location_button_size(265.0f, 33.0f);
                        const float location_button_gap = 14.0f;
                        const char* location_button_texts[] = {
                            "Save Current Position",
                            "Delete Selected Location"
                        };
                        for (int button_index = 0; button_index < IM_ARRAYSIZE(location_button_texts); ++button_index)
                        {
                            const ImVec2 location_button_min(
                                405.0f,
                                155.0f + button_index * (location_button_size.y + location_button_gap));
                            sigma::button(location_button_texts[button_index], location_button_min, location_button_size, location_manager_button_hover_fade[button_index], 12.0f, content_alpha, content_interactive);
                        }
                    }
                }
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (create_overlay_click_lock && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            create_overlay_click_lock = false;
        if (quick_presets_overlay_click_lock && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            quick_presets_overlay_click_lock = false;
        if (object_add_overlay_click_lock && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            object_add_overlay_click_lock = false;
        g_ModalOverlayHitTest =
            create_overlay_open || create_overlay_fade > 0.02f ||
            quick_presets_overlay_open || quick_presets_overlay_fade > 0.02f ||
            object_add_overlay_open || object_add_overlay_fade > 0.02f;
        float overlay_blur_fade = create_overlay_fade > quick_presets_overlay_fade ? create_overlay_fade : quick_presets_overlay_fade;
        if (object_add_overlay_fade > overlay_blur_fade)
            overlay_blur_fade = object_add_overlay_fade;
        const float overlay_blur_alpha = smooth_fade(overlay_blur_fade);
        const float modal_menu_center_x = static_cast<float>(kMenuRegionWidth) * 0.5f;
        const ImVec2 modal_overlay_min(4.0f, 4.0f);
        const ImVec2 modal_overlay_max(static_cast<float>(kMenuRegionWidth) - 8.0f, static_cast<float>(kMenuHeight) - 8.0f);
        const ImVec2 modal_close_center(static_cast<float>(kMenuRegionWidth) - 29.0f, 29.0f);
        if (overlay_blur_alpha > 0.005f)
        {
            ImDrawList* backdrop_draw_list = ImGui::GetForegroundDrawList();
            if (g_BlurCaptureRequested)
            {
                backdrop_draw_list->AddCallback(GaussianBlurDrawCallback, nullptr);
                backdrop_draw_list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
            }
            if (g_BlurCacheValid && g_BlurTextureView)
            {
                backdrop_draw_list->AddImage(
                    (ImTextureID)g_BlurTextureView,
                    modal_overlay_min,
                    modal_overlay_max,
                    ImVec2(0.0f, 0.0f),
                    ImVec2(1.0f, 1.0f),
                    IM_COL32(255, 255, 255, static_cast<int>(255.0f * overlay_blur_alpha)));
            }
        }
        else if (!create_overlay_open && !quick_presets_overlay_open && !object_add_overlay_open)
        {
            g_BlurCacheValid = false;
            g_BlurCaptureRequested = false;
        }
        sigma::modal_overlay(
            "##create_overlay",
            create_overlay_open,
            create_overlay_fade,
            modal_overlay_min,
            modal_overlay_max,
            false);
        sigma::modal_overlay(
            "##quick_presets_overlay",
            quick_presets_overlay_open,
            quick_presets_overlay_fade,
            modal_overlay_min,
            modal_overlay_max,
            false);
        sigma::modal_overlay(
            "##object_add_overlay",
            object_add_overlay_open,
            object_add_overlay_fade,
            modal_overlay_min,
            modal_overlay_max,
            false);
        ImGui::GetForegroundDrawList()->PushClipRect(modal_overlay_min, modal_overlay_max, true);
        ImGui::GetBackgroundDrawList()->PushClipRect(modal_overlay_min, modal_overlay_max, true);
        const float create_modal_alpha = smooth_fade(create_overlay_fade);
        const float create_content_alpha = create_modal_alpha;
        if (create_overlay_fade > 0.005f)
        {
            if (sigma::modal_close_button(
                "##create_modal_close",
                modal_close_center,
                create_content_alpha,
                create_close_hover_fade))
            {
                create_overlay_open = false;
            }
            const ImVec2 confirm_button_size(248.0f, 38.0f);
            const float group_height = 214.0f;
            const float modal_slide_distance = 390.0f;
            const float group_top_target = (static_cast<float>(kMenuHeight) - group_height) * 0.5f;
            const float group_top = group_top_target + modal_slide_distance * (1.0f - create_modal_alpha);
            const float confirm_y = group_top + 176.0f;
            const float confirm_x = modal_menu_center_x - confirm_button_size.x * 0.5f;
            sigma::modal_title_centered(
                ImVec2(modal_menu_center_x, confirm_y - 154.0f),
                u8"\uf317",
                "Create new config",
                create_content_alpha,
                20.0f,
                18.0f);
            sigma::modal_input(
                "##create_config_name_input",
                ImVec2(confirm_x, confirm_y - 128.0f),
                ImVec2(248.0f, 45.0f),
                u8"\uf4ca",
                "Enter the name...",
                create_config_input,
                IM_ARRAYSIZE(create_config_input),
                create_content_alpha);
            const ImVec2 modal_checkbox_size(248.0f, 24.0f);
            sigma::checkbox(
                "##create_private_config",
                "Private Config",
                ImVec2(confirm_x, confirm_y - 72.0f),
                modal_checkbox_size,
                ImVec2(confirm_x, confirm_y - 67.0f),
                ImVec2(confirm_x + 218.0f, confirm_y - 68.0f),
                create_private_config_enabled,
                create_private_config_fade,
                create_private_config_hover_fade,
                13.0f,
                true,
                create_content_alpha);
            sigma::checkbox(
                "##create_hide_username",
                "Hide Username",
                ImVec2(confirm_x, confirm_y - 38.0f),
                modal_checkbox_size,
                ImVec2(confirm_x, confirm_y - 33.0f),
                ImVec2(confirm_x + 218.0f, confirm_y - 34.0f),
                create_hide_username_enabled,
                create_hide_username_fade,
                create_hide_username_hover_fade,
                13.0f,
                true,
                create_content_alpha);
            if (sigma::modal_button(
                "Confirm",
                u8"\uf313",
                ImVec2(confirm_x, confirm_y),
                confirm_button_size,
                create_content_alpha,
                create_confirm_hover_fade))
            {
                if (config_count < IM_ARRAYSIZE(configs))
                {
                    ConfigEntry& config = configs[config_count];
                    const char* source_name = create_config_input[0] != '\0' ? create_config_input : "Default";
                    sprintf_s(config.name, IM_ARRAYSIZE(config.name), "%.18s", source_name);
                    format_config_time(config.created_at, IM_ARRAYSIZE(config.created_at));
                    config.editing = false;
                    config.deleting = false;
                    config.fade = 1.0f;
                    config.underline_fade = 0.0f;
                    config.button_hover_fade = 0.0f;
                    config.action_hover_fade[0] = 0.0f;
                    config.action_hover_fade[1] = 0.0f;
                    ++config_count;
                    create_config_input[0] = '\0';
                    create_overlay_open = false;
                }
            }
        }
        const float object_add_modal_alpha = smooth_fade(object_add_overlay_fade);
        const float object_add_content_alpha = object_add_modal_alpha;
        if (object_add_overlay_fade > 0.005f)
        {
            if (sigma::modal_close_button(
                "##object_add_modal_close",
                modal_close_center,
                object_add_content_alpha,
                object_add_close_hover_fade))
            {
                object_add_overlay_open = false;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 object_add_button_size(248.0f, 38.0f);
            const float object_add_group_height = 190.0f;
            const float object_add_slide_distance = 390.0f;
            const float object_add_group_top_target = (static_cast<float>(kMenuHeight) - object_add_group_height) * 0.5f;
            const float object_add_group_top = object_add_group_top_target + object_add_slide_distance * (1.0f - object_add_modal_alpha);
            const float object_add_confirm_y = object_add_group_top + 152.0f;
            const float object_add_confirm_x = modal_menu_center_x - object_add_button_size.x * 0.5f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            sigma::modal_title_centered(
                ImVec2(modal_menu_center_x, object_add_confirm_y - 144.0f),
                u8"\uf317",
                "Add Object",
                object_add_content_alpha,
                20.0f,
                18.0f);
            sigma::modal_input(
                "##object_add_hash_input",
                ImVec2(object_add_confirm_x, object_add_confirm_y - 114.0f),
                ImVec2(248.0f, 45.0f),
                u8"\uf4ca",
                "Hash",
                object_add_hash_input,
                IM_ARRAYSIZE(object_add_hash_input),
                object_add_content_alpha);
            sigma::modal_input(
                "##object_add_name_input",
                ImVec2(object_add_confirm_x, object_add_confirm_y - 58.0f),
                ImVec2(248.0f, 45.0f),
                u8"\uf4ca",
                "Name",
                object_add_name_input,
                IM_ARRAYSIZE(object_add_name_input),
                object_add_content_alpha);
            if (sigma::modal_button(
                "Add",
                u8"\uf313",
                ImVec2(object_add_confirm_x, object_add_confirm_y),
                object_add_button_size,
                object_add_content_alpha,
                object_add_confirm_hover_fade))
            {
                if (object_entry_count < IM_ARRAYSIZE(object_entries) &&
                    (object_add_hash_input[0] != '\0' || object_add_name_input[0] != '\0'))
                {
                    ObjectEntry& object_entry = object_entries[object_entry_count];
                    const char* source_hash = object_add_hash_input[0] != '\0' ? object_add_hash_input : object_add_name_input;
                    const char* source_name = object_add_name_input[0] != '\0' ? object_add_name_input : source_hash;
                    sprintf_s(object_entry.hash, IM_ARRAYSIZE(object_entry.hash), "%.31s", source_hash);
                    sprintf_s(object_entry.name, IM_ARRAYSIZE(object_entry.name), "%.31s", source_name);
                    object_entry.delete_hover_fade = 0.0f;
                    ++object_entry_count;
                    object_add_hash_input[0] = '\0';
                    object_add_name_input[0] = '\0';
                    object_add_overlay_open = false;
                }
            }
        }
        const float quick_presets_modal_alpha = smooth_fade(quick_presets_overlay_fade);
        const float quick_presets_content_alpha = quick_presets_modal_alpha;
        if (quick_presets_overlay_fade > 0.005f)
        {
            if (sigma::modal_close_button(
                "##quick_presets_modal_close",
                modal_close_center,
                quick_presets_content_alpha,
                quick_presets_close_hover_fade))
            {
                quick_presets_overlay_open = false;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const ImVec2 preset_button_size(205.0f, 38.0f);
            const float preset_button_gap = 10.0f;
            const float preset_grid_width = preset_button_size.x * 2.0f + preset_button_gap;
            const float preset_grid_height = preset_button_size.y * 4.0f + preset_button_gap * 3.0f;
            const float preset_group_height = preset_grid_height + 58.0f;
            const float preset_slide_distance = 420.0f;
            const float preset_group_top_target = (static_cast<float>(kMenuHeight) - preset_group_height) * 0.5f;
            const float preset_group_top = preset_group_top_target + preset_slide_distance * (1.0f - quick_presets_modal_alpha);
            const float preset_grid_x = modal_menu_center_x - preset_grid_width * 0.5f;
            const float preset_grid_y = preset_group_top + 58.0f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            sigma::modal_title_centered(
                ImVec2(modal_menu_center_x, preset_group_top + 22.0f),
                u8"\uf3dc",
                "Quick presets",
                quick_presets_content_alpha,
                20.0f,
                18.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const char* preset_labels[] = {
                "Default Mode (0.13)",
                "Default V2 Mode (0.16)",
                "Legit Mode (0.18)",
                "Legit V2 Mode (0.20)",
                "SemiRage Mode (0.23)",
                "SemiRage V2 Mode (0.35)",
                "Rage Mode (0.50)",
                "Rage V2 Mode (0.80)"
            };
            const float preset_values[] = {
                0.13f,
                0.16f,
                0.18f,
                0.20f,
                0.23f,
                0.35f,
                0.50f,
                0.80f
            };
            for (int preset_index = 0; preset_index < IM_ARRAYSIZE(preset_labels); ++preset_index)
            {
                const int column = preset_index % 2;
                const int row = preset_index / 2;
                const ImVec2 preset_min(
                    preset_grid_x + column * (preset_button_size.x + preset_button_gap),
                    preset_grid_y + row * (preset_button_size.y + preset_button_gap));
                if (sigma::modal_button(
                    preset_labels[preset_index],
                    u8"\uf313",
                    preset_min,
                    preset_button_size,
                    quick_presets_content_alpha,
                    quick_presets_button_hover_fade[preset_index],
                    11.0f,
                    13.0f))
                {
                    hitbox_height_target_value = preset_values[preset_index];
                    hitbox_width_target_value = preset_values[preset_index];
                    quick_presets_overlay_open = false;
                }
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->PopClipRect();
        ImGui::GetForegroundDrawList()->PopClipRect();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool over_sidebar_tab = false;
        for (int icon_index = 0; icon_index < IM_ARRAYSIZE(sidebar_icons); ++icon_index)
        {
            const ImVec2 tab_hit_min(12.0f, sidebar_icon_start_y + sidebar_icon_step_y * icon_index - 15.0f);
            const ImVec2 tab_hit_max(tab_hit_min.x + 48.0f, tab_hit_min.y + 30.0f);
            if (mouse_in_rect(local_mouse_pos, tab_hit_min, tab_hit_max))
            {
                over_sidebar_tab = true;
                break;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool over_subtab = false;
        if (subtab_counts[displayed_top_tab] > 0)
        {
            float drag_subtab_x = subtab_start_x;
            for (int subtab_index = 0; subtab_index < subtab_counts[displayed_top_tab]; ++subtab_index)
            {
                const float drag_subtab_width = sigma::subtab_width(
                    subtab_icons[displayed_top_tab][subtab_index],
                    subtab_labels[displayed_top_tab][subtab_index]);
                const ImVec2 subtab_hit_min(drag_subtab_x - 4.0f, 16.0f);
                const ImVec2 subtab_hit_max(subtab_hit_min.x + drag_subtab_width, subtab_hit_min.y + 30.0f);
                if (mouse_in_rect(local_mouse_pos, subtab_hit_min, subtab_hit_max))
                {
                    over_subtab = true;
                    break;
                }
                drag_subtab_x += drag_subtab_width + 8.0f;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 top_title_hit_min(top_tab_icon_x - 8.0f, 16.0f);
        const ImVec2 top_title_hit_max(top_tab_second_bar_x + 7.0f, 46.0f);
        const bool over_top_title = mouse_in_rect(local_mouse_pos, top_title_hit_min, top_title_hit_max);
        const bool over_bottom_icon = mouse_in_rect(
            local_mouse_pos,
            bottom_icon_hit_min,
            ImVec2(bottom_icon_hit_min.x + bottom_icon_hit_size.x, bottom_icon_hit_min.y + bottom_icon_hit_size.y));
        const bool over_profile = mouse_in_rect(local_mouse_pos, profile_min, profile_max);
        const bool over_menu = mouse_in_rect(
            local_mouse_pos,
            ImVec2(4.0f, 4.0f),
            ImVec2(static_cast<float>(kMenuRegionWidth) - 4.0f, static_cast<float>(kMenuHeight) - 4.0f));
        const bool over_content_panel = mouse_in_rect(
            local_mouse_pos,
            ImVec2(66.0f, 56.0f),
            ImVec2(694.0f, 550.0f));
        const bool over_interactive_control =
            ImGui::GetMouseCursor() != ImGuiMouseCursor_Arrow ||
            ImGui::IsAnyItemHovered() ||
            ImGui::IsAnyItemActive();
        const bool drag_background =
            over_menu &&
            !modal_active &&
            !over_content_panel &&
            !over_interactive_control &&
            !over_sidebar_tab &&
            !over_subtab &&
            !over_top_title &&
            !over_bottom_icon &&
            !over_profile;
        if (!g_MenuDragging && drag_background && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            g_MenuDragging = true;
            g_MenuDragDelta = local_mouse_pos;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        io.MousePos = raw_mouse_pos;
        for (int vertex_index = background_vertex_start; vertex_index < background_draw_list->VtxBuffer.Size; ++vertex_index)
        {
            background_draw_list->VtxBuffer[vertex_index].pos.x += g_MenuPos.x;
            background_draw_list->VtxBuffer[vertex_index].pos.y += g_MenuPos.y;
        }
        for (int command_index = background_command_start; command_index < background_draw_list->CmdBuffer.Size; ++command_index)
        {
            ImDrawCmd& command = background_draw_list->CmdBuffer[command_index];
            command.ClipRect.x += g_MenuPos.x;
            command.ClipRect.y += g_MenuPos.y;
            command.ClipRect.z += g_MenuPos.x;
            command.ClipRect.w += g_MenuPos.y;
        }
        for (int vertex_index = foreground_vertex_start; vertex_index < foreground_draw_list->VtxBuffer.Size; ++vertex_index)
        {
            foreground_draw_list->VtxBuffer[vertex_index].pos.x += g_MenuPos.x;
            foreground_draw_list->VtxBuffer[vertex_index].pos.y += g_MenuPos.y;
        }
        for (int command_index = foreground_command_start; command_index < foreground_draw_list->CmdBuffer.Size; ++command_index)
        {
            ImDrawCmd& command = foreground_draw_list->CmdBuffer[command_index];
            command.ClipRect.x += g_MenuPos.x;
            command.ClipRect.y += g_MenuPos.y;
            command.ClipRect.z += g_MenuPos.x;
            command.ClipRect.w += g_MenuPos.y;
        }
        if (imgui_context)
        {
            for (int window_index = 0; window_index < imgui_context->Windows.Size; ++window_index)
            {
                ImGuiWindow* window = imgui_context->Windows[window_index];
                if (!window || !window->DrawList)
                    continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                int vertex_start = window->DrawList->VtxBuffer.Size;
                int command_start = window->DrawList->CmdBuffer.Size;
                for (const WindowDrawListStart& start : window_draw_list_starts)
                {
                    if (start.drawList == window->DrawList)
                    {
                        vertex_start = start.vertexStart;
                        command_start = start.commandStart;
                        break;
                    }
                }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                for (int vertex_index = vertex_start; vertex_index < window->DrawList->VtxBuffer.Size; ++vertex_index)
                {
                    window->DrawList->VtxBuffer[vertex_index].pos.x += g_MenuPos.x;
                    window->DrawList->VtxBuffer[vertex_index].pos.y += g_MenuPos.y;
                }
                for (int command_index = command_start; command_index < window->DrawList->CmdBuffer.Size; ++command_index)
                {
                    ImDrawCmd& command = window->DrawList->CmdBuffer[command_index];
                    command.ClipRect.x += g_MenuPos.x;
                    command.ClipRect.y += g_MenuPos.y;
                    command.ClipRect.z += g_MenuPos.x;
                    command.ClipRect.w += g_MenuPos.y;
                }
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::Render();
        constexpr float clear_color[4] = { 140.0f / 255.0f, 184.0f / 255.0f, 1.0f, 1.0f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        HRESULT hr = g_pSwapChain->Present(settings_general_enabled[2] ? 1 : 0, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    CleanupLogoTexture();
    CleanupProfileTexture();
    CleanupBackgroundTexture();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    if (com_initialized)
        CoUninitialize();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    return 0;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
bool CreateTextureFromPngMemory(const unsigned char* data, unsigned int data_size, ID3D11ShaderResourceView** out_srv, UINT* out_width, UINT* out_height)
{
    *out_srv = nullptr;
    if (out_width)
        *out_width = 0;
    if (out_height)
        *out_height = 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    IWICImagingFactory* factory = nullptr;
    IWICStream* stream = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (SUCCEEDED(hr))
        hr = factory->CreateStream(&stream);
    if (SUCCEEDED(hr))
        hr = stream->InitializeFromMemory(const_cast<BYTE*>(data), data_size);
    if (SUCCEEDED(hr))
        hr = factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, &decoder);
    if (SUCCEEDED(hr))
        hr = decoder->GetFrame(0, &frame);
    if (SUCCEEDED(hr))
        hr = factory->CreateFormatConverter(&converter);
    if (SUCCEEDED(hr))
        hr = converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    UINT width = 0;
    UINT height = 0;
    if (SUCCEEDED(hr))
        hr = converter->GetSize(&width, &height);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    std::vector<unsigned char> pixels(width * height * 4);
    if (SUCCEEDED(hr))
        hr = converter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ID3D11Texture2D* texture = nullptr;
    if (SUCCEEDED(hr))
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        D3D11_SUBRESOURCE_DATA sub_resource = {};
        sub_resource.pSysMem = pixels.data();
        sub_resource.SysMemPitch = width * 4;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        hr = g_pd3dDevice->CreateTexture2D(&desc, &sub_resource, &texture);
    }
    if (SUCCEEDED(hr))
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MipLevels = 1;
        hr = g_pd3dDevice->CreateShaderResourceView(texture, &srv_desc, out_srv);
    }
    if (SUCCEEDED(hr))
    {
        if (out_width)
            *out_width = width;
        if (out_height)
            *out_height = height;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (texture) texture->Release();
    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    if (stream) stream->Release();
    if (factory) factory->Release();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    return SUCCEEDED(hr);
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupLogoTexture()
{
    if (g_LogoTexture) { g_LogoTexture->Release(); g_LogoTexture = nullptr; }
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupProfileTexture()
{
    if (g_ProfileTexture) { g_ProfileTexture->Release(); g_ProfileTexture = nullptr; }
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupBackgroundTexture()
{
    if (g_BackgroundTexture) { g_BackgroundTexture->Release(); g_BackgroundTexture = nullptr; }
    g_BackgroundTextureWidth = 0;
    g_BackgroundTextureHeight = 0;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupBlurResources()
{
    if (g_BlurReadbackTexture) { g_BlurReadbackTexture->Release(); g_BlurReadbackTexture = nullptr; }
    if (g_BlurTextureView) { g_BlurTextureView->Release(); g_BlurTextureView = nullptr; }
    if (g_BlurUploadTexture) { g_BlurUploadTexture->Release(); g_BlurUploadTexture = nullptr; }
    g_BlurTextureWidth = 0;
    g_BlurTextureHeight = 0;
    g_BlurTextureFormat = DXGI_FORMAT_UNKNOWN;
    g_BlurCacheValid = false;
    g_BlurCaptureRequested = false;
    g_BlurSourcePixels.clear();
    g_BlurTempPixels.clear();
    g_BlurOutputPixels.clear();
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static bool EnsureBlurResources(const D3D11_TEXTURE2D_DESC& source_desc)
{
    if (!g_pd3dDevice || source_desc.SampleDesc.Count != 1)
        return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (g_BlurReadbackTexture &&
        g_BlurUploadTexture &&
        g_BlurTextureWidth == source_desc.Width &&
        g_BlurTextureHeight == source_desc.Height &&
        g_BlurTextureFormat == source_desc.Format)
    {
        return true;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    CleanupBlurResources();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_TEXTURE2D_DESC readback_desc = source_desc;
    readback_desc.BindFlags = 0;
    readback_desc.MiscFlags = 0;
    readback_desc.Usage = D3D11_USAGE_STAGING;
    readback_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_TEXTURE2D_DESC upload_desc = source_desc;
    upload_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    upload_desc.MiscFlags = 0;
    upload_desc.Usage = D3D11_USAGE_DEFAULT;
    upload_desc.CPUAccessFlags = 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    if (FAILED(g_pd3dDevice->CreateTexture2D(&readback_desc, nullptr, &g_BlurReadbackTexture)))
    {
        CleanupBlurResources();
        return false;
    }
    if (FAILED(g_pd3dDevice->CreateTexture2D(&upload_desc, nullptr, &g_BlurUploadTexture)))
    {
        CleanupBlurResources();
        return false;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    srv_desc.Format = source_desc.Format;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MipLevels = 1;
    if (FAILED(g_pd3dDevice->CreateShaderResourceView(g_BlurUploadTexture, &srv_desc, &g_BlurTextureView)))
    {
        CleanupBlurResources();
        return false;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const size_t pixel_count = static_cast<size_t>(source_desc.Width) * static_cast<size_t>(source_desc.Height) * 4u;
    g_BlurSourcePixels.resize(pixel_count);
    g_BlurTempPixels.resize(pixel_count);
    g_BlurOutputPixels.resize(pixel_count);
    g_BlurTextureWidth = source_desc.Width;
    g_BlurTextureHeight = source_desc.Height;
    g_BlurTextureFormat = source_desc.Format;
    return true;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
static int ClampPixelCoord(int value, int max_value)
{
    if (value < 0)
        return 0;
    if (value > max_value)
        return max_value;
    return value;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CaptureGaussianBlurBackdrop()
{
    if (!g_pSwapChain || !g_pd3dDeviceContext || !g_BlurCaptureRequested)
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ID3D11Texture2D* back_buffer = nullptr;
    if (FAILED(g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))) || !back_buffer)
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_TEXTURE2D_DESC desc = {};
    back_buffer->GetDesc(&desc);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const int source_left = ClampPixelCoord(static_cast<int>(std::round((g_MenuPos.x + 4.0f) * g_UiWindowScale)), static_cast<int>(desc.Width) - 1);
    const int source_top = ClampPixelCoord(static_cast<int>(std::round((g_MenuPos.y + 4.0f) * g_UiWindowScale)), static_cast<int>(desc.Height) - 1);
    const int source_right = ClampPixelCoord(static_cast<int>(std::round((g_MenuPos.x + static_cast<float>(kMenuRegionWidth) - 8.0f) * g_UiWindowScale)), static_cast<int>(desc.Width));
    const int source_bottom = ClampPixelCoord(static_cast<int>(std::round((g_MenuPos.y + static_cast<float>(kMenuHeight) - 8.0f) * g_UiWindowScale)), static_cast<int>(desc.Height));
    if (source_right <= source_left || source_bottom <= source_top)
    {
        back_buffer->Release();
        return;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_TEXTURE2D_DESC blur_desc = desc;
    blur_desc.Width = static_cast<UINT>(source_right - source_left);
    blur_desc.Height = static_cast<UINT>(source_bottom - source_top);
    if (desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM || !EnsureBlurResources(blur_desc))
    {
        back_buffer->Release();
        return;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    ID3D11RenderTargetView* old_render_target = nullptr;
    ID3D11DepthStencilView* old_depth_stencil = nullptr;
    g_pd3dDeviceContext->OMGetRenderTargets(1, &old_render_target, &old_depth_stencil);
    g_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    const auto restore_render_target = [&]()
    {
        g_pd3dDeviceContext->OMSetRenderTargets(1, &old_render_target, old_depth_stencil);
        if (old_render_target) { old_render_target->Release(); old_render_target = nullptr; }
        if (old_depth_stencil) { old_depth_stencil->Release(); old_depth_stencil = nullptr; }
    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_BOX source_box = {};
    source_box.left = static_cast<UINT>(source_left);
    source_box.top = static_cast<UINT>(source_top);
    source_box.front = 0;
    source_box.right = static_cast<UINT>(source_right);
    source_box.bottom = static_cast<UINT>(source_bottom);
    source_box.back = 1;
    g_pd3dDeviceContext->CopySubresourceRegion(g_BlurReadbackTexture, 0, 0, 0, 0, back_buffer, 0, &source_box);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    if (FAILED(g_pd3dDeviceContext->Map(g_BlurReadbackTexture, 0, D3D11_MAP_READ, 0, &mapped)))
    {
        restore_render_target();
        back_buffer->Release();
        return;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const UINT row_bytes = blur_desc.Width * 4u;
    const unsigned char* source = static_cast<const unsigned char*>(mapped.pData);
    for (UINT y = 0; y < blur_desc.Height; ++y)
    {
        const unsigned char* source_row = source + static_cast<size_t>(mapped.RowPitch) * y;
        unsigned char* target_row = g_BlurSourcePixels.data() + static_cast<size_t>(row_bytes) * y;
        for (UINT x = 0; x < row_bytes; ++x)
            target_row[x] = source_row[x];
    }
    g_pd3dDeviceContext->Unmap(g_BlurReadbackTexture, 0);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    constexpr int radius = 7;
    constexpr float weights[radius + 1] = {
        0.132981f, 0.125794f, 0.106483f, 0.080657f,
        0.054670f, 0.033159f, 0.017997f, 0.008741f
    };
    const float weight_sum = weights[0] + 2.0f * (weights[1] + weights[2] + weights[3] + weights[4] + weights[5] + weights[6] + weights[7]);
    const int width = static_cast<int>(blur_desc.Width);
    const int height = static_cast<int>(blur_desc.Height);
    const int max_x = width - 1;
    const int max_y = height - 1;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float rgba[4] = {};
            for (int offset = -radius; offset <= radius; ++offset)
            {
                const int source_x = ClampPixelCoord(x + offset, max_x);
                const float weight = weights[offset < 0 ? -offset : offset] / weight_sum;
                const size_t source_index = (static_cast<size_t>(y) * width + source_x) * 4u;
                rgba[0] += g_BlurSourcePixels[source_index + 0] * weight;
                rgba[1] += g_BlurSourcePixels[source_index + 1] * weight;
                rgba[2] += g_BlurSourcePixels[source_index + 2] * weight;
                rgba[3] += g_BlurSourcePixels[source_index + 3] * weight;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const size_t target_index = (static_cast<size_t>(y) * width + x) * 4u;
            g_BlurTempPixels[target_index + 0] = static_cast<unsigned char>(rgba[0] + 0.5f);
            g_BlurTempPixels[target_index + 1] = static_cast<unsigned char>(rgba[1] + 0.5f);
            g_BlurTempPixels[target_index + 2] = static_cast<unsigned char>(rgba[2] + 0.5f);
            g_BlurTempPixels[target_index + 3] = static_cast<unsigned char>(rgba[3] + 0.5f);
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float rgba[4] = {};
            for (int offset = -radius; offset <= radius; ++offset)
            {
                const int source_y = ClampPixelCoord(y + offset, max_y);
                const float weight = weights[offset < 0 ? -offset : offset] / weight_sum;
                const size_t source_index = (static_cast<size_t>(source_y) * width + x) * 4u;
                rgba[0] += g_BlurTempPixels[source_index + 0] * weight;
                rgba[1] += g_BlurTempPixels[source_index + 1] * weight;
                rgba[2] += g_BlurTempPixels[source_index + 2] * weight;
                rgba[3] += g_BlurTempPixels[source_index + 3] * weight;
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const size_t target_index = (static_cast<size_t>(y) * width + x) * 4u;
            for (int channel = 0; channel < 3; ++channel)
            {
                const float blurred = rgba[channel];
                g_BlurOutputPixels[target_index + channel] = static_cast<unsigned char>(blurred + 0.5f);
            }
            g_BlurOutputPixels[target_index + 3] = g_BlurSourcePixels[target_index + 3];
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    g_pd3dDeviceContext->UpdateSubresource(g_BlurUploadTexture, 0, nullptr, g_BlurOutputPixels.data(), row_bytes, 0);
    restore_render_target();
    back_buffer->Release();
    g_BlurCacheValid = true;
    g_BlurCaptureRequested = false;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void GaussianBlurDrawCallback(const ImDrawList*, const ImDrawCmd*)
{
    CaptureGaussianBlurBackdrop();
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
bool CreateDeviceD3D(HWND hWnd)
{
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) 
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    CreateRenderTarget();
    return true;
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void CleanupRenderTarget()
{
    CleanupBlurResources();
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
void ApplyDarkTitleBar(HWND hWnd)
{
    if (!hWnd)
        return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    constexpr DWORD kDwmwaUseImmersiveDarkModeBefore20h1 = 19;
    constexpr DWORD kDwmwaUseImmersiveDarkMode = 20;
    constexpr DWORD kDwmwaBorderColor = 34;
    constexpr DWORD kDwmwaCaptionColor = 35;
    constexpr DWORD kDwmwaTextColor = 36;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    BOOL dark_mode = TRUE;
    if (FAILED(DwmSetWindowAttribute(hWnd, kDwmwaUseImmersiveDarkMode, &dark_mode, sizeof(dark_mode))))
        DwmSetWindowAttribute(hWnd, kDwmwaUseImmersiveDarkModeBefore20h1, &dark_mode, sizeof(dark_mode));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    const COLORREF caption_color = RGB(0, 0, 0);
    const COLORREF border_color = RGB(18, 18, 18);
    const COLORREF text_color = RGB(255, 255, 255);
    DwmSetWindowAttribute(hWnd, kDwmwaCaptionColor, &caption_color, sizeof(caption_color));
    DwmSetWindowAttribute(hWnd, kDwmwaBorderColor, &border_color, sizeof(border_color));
    DwmSetWindowAttribute(hWnd, kDwmwaTextColor, &text_color, sizeof(text_color));
}
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); 
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) 
            return 0;
        break;
    case WM_THEMECHANGED:
    case WM_DWMCOMPOSITIONCHANGED:
        ApplyDarkTitleBar(hWnd);
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
