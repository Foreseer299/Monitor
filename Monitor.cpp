#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <commdlg.h>
#include <d2d1.h>
#include <dwrite.h>
#include <shellapi.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif

template <typename FnPtr>
inline FnPtr LoadProc(HMODULE hMod, const char* procName) {
    return reinterpret_cast<FnPtr>(reinterpret_cast<void*>(GetProcAddress(hMod, procName)));
}

// -------------------------------------------------------------
// 常量与菜单 ID
// -------------------------------------------------------------
#define WM_TRAYICON (WM_USER + 101)
#define ID_TIMER_REFRESH 1001
#define ID_TIMER_TRIM    1002

// 刷新频率
#define IDM_REFRESH_250   2001
#define IDM_REFRESH_500   2002
#define IDM_REFRESH_1000  2003
#define IDM_REFRESH_2000  2004
#define IDM_REFRESH_3000  2005

// 显示项目
#define IDM_SHOW_CPU       2101
#define IDM_SHOW_GPU       2102
#define IDM_SHOW_GPU_TEMP  2103
#define IDM_SHOW_GPU_VRAM  2104
#define IDM_SHOW_GPU_POWER 2105
#define IDM_SHOW_RAM       2106
#define IDM_SHOW_RAM_GB    2107
#define IDM_SHOW_NET       2108
#define IDM_SHOW_CLOCK     2109
#define IDM_SHOW_TEMP_UNIT 2110

// 字体大小
#define IDM_FONT_10       2201
#define IDM_FONT_11       2202
#define IDM_FONT_12       2203
#define IDM_FONT_13       2204
#define IDM_FONT_14       2205
#define IDM_FONT_15       2206
#define IDM_FONT_18       2207
#define IDM_FONT_22       2208

// 字体粗细
#define IDM_WEIGHT_LIGHT    2221
#define IDM_WEIGHT_NORMAL   2222
#define IDM_WEIGHT_SEMIBOLD 2223
#define IDM_WEIGHT_BOLD     2224

// 字体风格 (10 款精选 + 系统字体选择器)
#define IDM_FAMILY_BASE   2230
#define IDM_FAMILY_COUNT  10
#define IDM_FAMILY_PICK   2249

// 标签文字风格
#define IDM_LABEL_STANDARD  2251
#define IDM_LABEL_COMPACT   2252
#define IDM_LABEL_CHINESE   2253
#define IDM_LABEL_NOCOLON   2254
#define IDM_LABEL_VALUEONLY 2255
#define IDM_LABEL_BRACKETS  2256

// 配色与美化
#define IDM_COLOR_WHITE     2301
#define IDM_COLOR_BLACK     2302
#define IDM_COLOR_SILVER    2303
#define IDM_COLOR_CYAN      2304
#define IDM_COLOR_GREEN     2305
#define IDM_COLOR_AMBER     2306
#define IDM_COLOR_PINK      2307
#define IDM_COLOR_VIOLET    2308
#define IDM_COLOR_CUSTOM    2309
#define IDM_COLOR_PICK      2310
#define IDM_COLOR_PERMETRIC 2311
#define IDM_COLOR_DYNLOAD   2312

// 背景底板
#define IDM_BG_NONE       2321
#define IDM_BG_DARK_PILL  2322
#define IDM_BG_LIGHT_PILL 2323
#define IDM_BG_DARK_BAR   2324
#define IDM_BG_NEON_PILL  2325

// 阴影与描边
#define IDM_SHADOW_SUBTLE  2331
#define IDM_SHADOW_STRONG  2332
#define IDM_SHADOW_OUTLINE 2333
#define IDM_SHADOW_NONE    2334

// 不透明度
#define IDM_OPACITY_100   2341
#define IDM_OPACITY_85    2342
#define IDM_OPACITY_70    2343
#define IDM_OPACITY_55    2344
#define IDM_OPACITY_40    2345

// 布局、间距与定位
#define IDM_LAYOUT_HORIZ     2401
#define IDM_LAYOUT_TWOROW    2402
#define IDM_LAYOUT_VERT      2403
#define IDM_SPACING_COMPACT  2411
#define IDM_SPACING_STANDARD 2412
#define IDM_SPACING_WIDE     2413
#define IDM_DOCK_TL          2421
#define IDM_DOCK_TC          2422
#define IDM_DOCK_TR          2423
#define IDM_DOCK_BL          2424
#define IDM_DOCK_BC          2425
#define IDM_DOCK_BR          2426
#define IDM_TOGGLE_DRAG      2430

// 系统行为
#define IDM_HIDE_FULLSCREEN 2501
#define IDM_AUTORUN         2502
#define IDM_LANG_ZH         2503
#define IDM_LANG_EN         2504
#define IDM_TOGGLE_SHOW     2505
#define IDM_RESET_DEFAULT   2506
#define IDM_EXIT            2599

// -------------------------------------------------------------
// 枚举与配置结构
// -------------------------------------------------------------
enum LabelStyle {
    LABEL_STANDARD = 0, // CPU: 12%
    LABEL_COMPACT  = 1, // C:12%
    LABEL_CHINESE  = 2, // 处理器 12%
    LABEL_NOCOLON  = 3, // CPU 12%
    LABEL_VALUE    = 4, // 12%
    LABEL_BRACKETS = 5  // [CPU 12%]
};

enum ColorTheme {
    THEME_WHITE  = 0,
    THEME_BLACK  = 1,
    THEME_SILVER = 2,
    THEME_CYAN   = 3,
    THEME_GREEN  = 4,
    THEME_AMBER  = 5,
    THEME_PINK   = 6,
    THEME_VIOLET = 7,
    THEME_CUSTOM = 8
};

enum BgStyle {
    BG_NONE       = 0,
    BG_DARK_PILL  = 1,
    BG_LIGHT_PILL = 2,
    BG_DARK_BAR   = 3,
    BG_NEON_PILL  = 4
};

enum ShadowStyle {
    SHADOW_SUBTLE  = 0,
    SHADOW_STRONG  = 1,
    SHADOW_OUTLINE = 2,
    SHADOW_NONE    = 3
};

enum LayoutMode {
    LAYOUT_HORIZONTAL = 0,
    LAYOUT_TWO_ROWS   = 1,
    LAYOUT_VERTICAL   = 2
};

enum SpacingMode {
    SPACING_COMPACT  = 0,
    SPACING_STANDARD = 1,
    SPACING_WIDE     = 2
};

enum DockPreset {
    DOCK_TOP_LEFT      = 0,
    DOCK_TOP_CENTER    = 1,
    DOCK_TOP_RIGHT     = 2,
    DOCK_BOTTOM_LEFT   = 3,
    DOCK_BOTTOM_CENTER = 4,
    DOCK_BOTTOM_RIGHT  = 5,
    DOCK_CUSTOM        = 6
};

enum MetricType {
    METRIC_CPU = 0,
    METRIC_GPU_USAGE,
    METRIC_GPU_TEMP,
    METRIC_GPU_VRAM,
    METRIC_GPU_POWER,
    METRIC_RAM,
    METRIC_NET,
    METRIC_CLOCK
};

struct AppConfig {
    int refreshInterval;
    float fontSize;
    int fontWeight;
    std::wstring fontFamily;
    int labelStyle;

    bool showCpu;
    bool showGpu;
    bool showGpuTemp;
    bool showGpuVram;
    bool showGpuPower;
    bool showRam;
    bool showRamGB;
    bool showNet;
    bool showClock;
    bool showTempUnit;

    int colorTheme;
    COLORREF customColor;
    bool perMetricColor;
    bool dynamicLoadColor;
    int bgStyle;
    int shadowStyle;
    int opacityPct;

    int layoutMode;
    int spacingMode;
    int dockPreset;
    int customPosX;
    int customPosY;

    bool hideOnFullscreen;
    bool langEnglish;
};

static const wchar_t* kFontPresets[IDM_FAMILY_COUNT] = {
    L"Segoe UI Variable Text",
    L"Segoe UI",
    L"Bahnschrift",
    L"Constantia",
    L"Cascadia Mono",
    L"Cascadia Code",
    L"Consolas",
    L"Microsoft YaHei UI",
    L"Calibri",
    L"Arial"
};

static const wchar_t* kRegConfigPath = L"Software\\Monitor";

// -------------------------------------------------------------
// 全局状态
// -------------------------------------------------------------
HWND g_hWnd = NULL;
NOTIFYICONDATAW g_nid{};
HINSTANCE g_hInstance = NULL;

ID2D1Factory* g_pD2DFactory = NULL;
IDWriteFactory* g_pDWriteFactory = NULL;
IDWriteTextFormat* g_pTextFormat = NULL;
ID2D1DCRenderTarget* g_pDCRenderTarget = NULL;
ID2D1SolidColorBrush* g_pBrush = NULL;

AppConfig g_cfg;
bool g_isShown = true;
bool g_hiddenByFullscreen = false;
bool g_dragUnlocked = false;
bool g_isDragging = false;
bool g_dialogOpen = false;
POINT g_dragStartCursor = { 0, 0 };
POINT g_dragStartWnd = { 0, 0 };

float g_dpiScale = 1.0f;
int g_winPhysW = 260;
int g_winPhysH = 24;
int g_posX = 8;
int g_posY = 6;

// 性能优化状态：脏检测 / 离屏缓存 / 字宽缓存
UINT64 g_configVersion = 1;
UINT64 g_fontVersion = 1;
UINT64 g_lastRenderedConfigVer = 0;
int g_lastRenderedPhysW = 0;
int g_lastRenderedPhysH = 0;
int g_lastRenderedPosX = 0;
int g_lastRenderedPosY = 0;
std::wstring g_lastFrameKey;

HDC g_hMemDC = NULL;
HBITMAP g_hBitmap = NULL;
HBITMAP g_hOldBitmap = NULL;
void* g_pBits = NULL;
int g_dibW = 0;
int g_dibH = 0;

inline void MarkVisualDirty() {
    ++g_configVersion;
}

inline void TrimWorkingSet() {
    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1);
}

// 前向声明
void RenderOverlay();
void UpdateTextFormat();

// -------------------------------------------------------------
// 语言辅助
// -------------------------------------------------------------
inline const wchar_t* T(const wchar_t* zh, const wchar_t* en) {
    return g_cfg.langEnglish ? en : zh;
}

void UpdateTrayTooltip() {
    if (!g_hWnd) return;
    wcscpy_s(g_nid.szTip, T(L"Monitor 硬件监控 (右键打开设置)", L"Monitor (Right-click for settings)"));
    Shell_NotifyIconW(NIM_MODIFY, &g_nid);
}

// -------------------------------------------------------------
// 默认配置与注册表持久化
// -------------------------------------------------------------
void InitDefaultConfig(AppConfig& cfg) {
    cfg.refreshInterval = 1000;
    cfg.fontSize = 13.0f;
    cfg.fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    cfg.fontFamily = L"Segoe UI Variable Text";
    cfg.labelStyle = LABEL_STANDARD;

    cfg.showCpu = true;
    cfg.showGpu = true;
    cfg.showGpuTemp = true;
    cfg.showGpuVram = false;
    cfg.showGpuPower = false;
    cfg.showRam = true;
    cfg.showRamGB = false;
    cfg.showNet = false;
    cfg.showClock = false;
    cfg.showTempUnit = false;

    cfg.colorTheme = THEME_WHITE;
    cfg.customColor = RGB(255, 255, 255);
    cfg.perMetricColor = false;
    cfg.dynamicLoadColor = false;
    cfg.bgStyle = BG_NONE;
    cfg.shadowStyle = SHADOW_SUBTLE;
    cfg.opacityPct = 100;

    cfg.layoutMode = LAYOUT_HORIZONTAL;
    cfg.spacingMode = SPACING_STANDARD;
    cfg.dockPreset = DOCK_TOP_LEFT;
    cfg.customPosX = 8;
    cfg.customPosY = 6;

    cfg.hideOnFullscreen = false;
    cfg.langEnglish = false;
}

static bool ReadRegDword(HKEY hKey, const wchar_t* name, DWORD& val) {
    DWORD type = 0, sz = sizeof(DWORD);
    if (RegQueryValueExW(hKey, name, NULL, &type, (LPBYTE)&val, &sz) == ERROR_SUCCESS && type == REG_DWORD) {
        return true;
    }
    return false;
}

static void WriteRegDword(HKEY hKey, const wchar_t* name, DWORD val) {
    RegSetValueExW(hKey, name, 0, REG_DWORD, (const BYTE*)&val, sizeof(DWORD));
}

void LoadConfig() {
    InitDefaultConfig(g_cfg);
    HKEY hKey = NULL;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, kRegConfigPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return;
    }

    DWORD v = 0;
    if (ReadRegDword(hKey, L"RefreshInterval", v) && (v == 250 || v == 500 || v == 1000 || v == 2000 || v == 3000)) {
        g_cfg.refreshInterval = (int)v;
    }
    if (ReadRegDword(hKey, L"FontSizeX10", v) && v >= 80 && v <= 360) {
        g_cfg.fontSize = v / 10.0f;
    }
    if (ReadRegDword(hKey, L"FontWeight", v) && (v == 300 || v == 400 || v == 600 || v == 700)) {
        g_cfg.fontWeight = (int)v;
    }

    wchar_t fontBuf[LF_FACESIZE + 1] = { 0 };
    DWORD fSz = sizeof(fontBuf);
    DWORD fType = 0;
    if (RegQueryValueExW(hKey, L"FontFamily", NULL, &fType, (LPBYTE)fontBuf, &fSz) == ERROR_SUCCESS &&
        fType == REG_SZ && wcslen(fontBuf) > 0) {
        g_cfg.fontFamily = fontBuf;
    }

    if (ReadRegDword(hKey, L"LabelStyle", v) && v <= LABEL_BRACKETS) g_cfg.labelStyle = (int)v;
    if (ReadRegDword(hKey, L"ShowCpu", v)) g_cfg.showCpu = (v != 0);
    if (ReadRegDword(hKey, L"ShowGpu", v)) g_cfg.showGpu = (v != 0);
    if (ReadRegDword(hKey, L"ShowGpuTemp", v)) g_cfg.showGpuTemp = (v != 0);
    if (ReadRegDword(hKey, L"ShowGpuVram", v)) g_cfg.showGpuVram = (v != 0);
    if (ReadRegDword(hKey, L"ShowGpuPower", v)) g_cfg.showGpuPower = (v != 0);
    if (ReadRegDword(hKey, L"ShowRam", v)) g_cfg.showRam = (v != 0);
    if (ReadRegDword(hKey, L"ShowRamGB", v)) g_cfg.showRamGB = (v != 0);
    if (ReadRegDword(hKey, L"ShowNet", v)) g_cfg.showNet = (v != 0);
    if (ReadRegDword(hKey, L"ShowClock", v)) g_cfg.showClock = (v != 0);
    if (ReadRegDword(hKey, L"ShowTempUnit", v)) g_cfg.showTempUnit = (v != 0);

    if (ReadRegDword(hKey, L"ColorTheme", v) && v <= THEME_CUSTOM) g_cfg.colorTheme = (int)v;
    if (ReadRegDword(hKey, L"CustomColor", v)) g_cfg.customColor = (COLORREF)v;
    if (ReadRegDword(hKey, L"PerMetricColor", v)) g_cfg.perMetricColor = (v != 0);
    if (ReadRegDword(hKey, L"DynamicLoadColor", v)) g_cfg.dynamicLoadColor = (v != 0);
    if (ReadRegDword(hKey, L"BgStyle", v) && v <= BG_NEON_PILL) g_cfg.bgStyle = (int)v;
    if (ReadRegDword(hKey, L"ShadowStyle", v) && v <= SHADOW_NONE) g_cfg.shadowStyle = (int)v;
    if (ReadRegDword(hKey, L"OpacityPct", v) && v >= 40 && v <= 100) g_cfg.opacityPct = (int)v;

    if (ReadRegDword(hKey, L"LayoutMode", v) && v <= LAYOUT_VERTICAL) g_cfg.layoutMode = (int)v;
    if (ReadRegDword(hKey, L"SpacingMode", v) && v <= SPACING_WIDE) g_cfg.spacingMode = (int)v;
    if (ReadRegDword(hKey, L"DockPreset", v) && v <= DOCK_CUSTOM) g_cfg.dockPreset = (int)v;
    if (ReadRegDword(hKey, L"CustomPosX", v)) g_cfg.customPosX = (int)(INT32)v;
    if (ReadRegDword(hKey, L"CustomPosY", v)) g_cfg.customPosY = (int)(INT32)v;

    if (ReadRegDword(hKey, L"HideOnFullscreen", v)) g_cfg.hideOnFullscreen = (v != 0);
    if (ReadRegDword(hKey, L"LangEnglish", v)) g_cfg.langEnglish = (v != 0);

    RegCloseKey(hKey);
}

void SaveConfig() {
    HKEY hKey = NULL;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, kRegConfigPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        return;
    }

    WriteRegDword(hKey, L"RefreshInterval", (DWORD)g_cfg.refreshInterval);
    WriteRegDword(hKey, L"FontSizeX10", (DWORD)roundf(g_cfg.fontSize * 10.0f));
    WriteRegDword(hKey, L"FontWeight", (DWORD)g_cfg.fontWeight);
    RegSetValueExW(hKey, L"FontFamily", 0, REG_SZ,
        (const BYTE*)g_cfg.fontFamily.c_str(),
        (DWORD)((g_cfg.fontFamily.size() + 1) * sizeof(wchar_t)));

    WriteRegDword(hKey, L"LabelStyle", (DWORD)g_cfg.labelStyle);
    WriteRegDword(hKey, L"ShowCpu", g_cfg.showCpu ? 1 : 0);
    WriteRegDword(hKey, L"ShowGpu", g_cfg.showGpu ? 1 : 0);
    WriteRegDword(hKey, L"ShowGpuTemp", g_cfg.showGpuTemp ? 1 : 0);
    WriteRegDword(hKey, L"ShowGpuVram", g_cfg.showGpuVram ? 1 : 0);
    WriteRegDword(hKey, L"ShowGpuPower", g_cfg.showGpuPower ? 1 : 0);
    WriteRegDword(hKey, L"ShowRam", g_cfg.showRam ? 1 : 0);
    WriteRegDword(hKey, L"ShowRamGB", g_cfg.showRamGB ? 1 : 0);
    WriteRegDword(hKey, L"ShowNet", g_cfg.showNet ? 1 : 0);
    WriteRegDword(hKey, L"ShowClock", g_cfg.showClock ? 1 : 0);
    WriteRegDword(hKey, L"ShowTempUnit", g_cfg.showTempUnit ? 1 : 0);

    WriteRegDword(hKey, L"ColorTheme", (DWORD)g_cfg.colorTheme);
    WriteRegDword(hKey, L"CustomColor", (DWORD)g_cfg.customColor);
    WriteRegDword(hKey, L"PerMetricColor", g_cfg.perMetricColor ? 1 : 0);
    WriteRegDword(hKey, L"DynamicLoadColor", g_cfg.dynamicLoadColor ? 1 : 0);
    WriteRegDword(hKey, L"BgStyle", (DWORD)g_cfg.bgStyle);
    WriteRegDword(hKey, L"ShadowStyle", (DWORD)g_cfg.shadowStyle);
    WriteRegDword(hKey, L"OpacityPct", (DWORD)g_cfg.opacityPct);

    WriteRegDword(hKey, L"LayoutMode", (DWORD)g_cfg.layoutMode);
    WriteRegDword(hKey, L"SpacingMode", (DWORD)g_cfg.spacingMode);
    WriteRegDword(hKey, L"DockPreset", (DWORD)g_cfg.dockPreset);
    WriteRegDword(hKey, L"CustomPosX", (DWORD)(INT32)g_cfg.customPosX);
    WriteRegDword(hKey, L"CustomPosY", (DWORD)(INT32)g_cfg.customPosY);

    WriteRegDword(hKey, L"HideOnFullscreen", g_cfg.hideOnFullscreen ? 1 : 0);
    WriteRegDword(hKey, L"LangEnglish", g_cfg.langEnglish ? 1 : 0);

    RegCloseKey(hKey);
}

// -------------------------------------------------------------
// DPI 辅助
// -------------------------------------------------------------
void EnablePerMonitorDpi() {
    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (!hUser32) return;
    typedef BOOL(WINAPI* SetProcessDpiAwarenessContext_t)(HANDLE);
    auto fn = LoadProc<SetProcessDpiAwarenessContext_t>(hUser32, "SetProcessDpiAwarenessContext");
    if (fn) {
        fn((HANDLE)-4); // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    } else {
        typedef BOOL(WINAPI* SetProcessDPIAware_t)();
        auto fnOld = LoadProc<SetProcessDPIAware_t>(hUser32, "SetProcessDPIAware");
        if (fnOld) fnOld();
    }
}

float QueryDpiScale(HWND hWnd) {
    UINT dpi = 96;
    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (hUser32) {
        if (hWnd) {
            typedef UINT(WINAPI* GetDpiForWindow_t)(HWND);
            auto fn = LoadProc<GetDpiForWindow_t>(hUser32, "GetDpiForWindow");
            if (fn) dpi = fn(hWnd);
        } else {
            typedef UINT(WINAPI* GetDpiForSystem_t)();
            auto fn = LoadProc<GetDpiForSystem_t>(hUser32, "GetDpiForSystem");
            if (fn) dpi = fn();
        }
    }
    if (dpi == 0) dpi = 96;
    return dpi / 96.0f;
}

// -------------------------------------------------------------
// CPU 使用率采集 (GetSystemTimes - 零开销)
// -------------------------------------------------------------
ULARGE_INTEGER g_prevIdleTime{};
ULARGE_INTEGER g_prevKernelTime{};
ULARGE_INTEGER g_prevUserTime{};
bool g_cpuInitialized = false;

int GetCpuUsage() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) return 0;

    ULARGE_INTEGER curIdle, curKernel, curUser;
    curIdle.LowPart = idleTime.dwLowDateTime; curIdle.HighPart = idleTime.dwHighDateTime;
    curKernel.LowPart = kernelTime.dwLowDateTime; curKernel.HighPart = kernelTime.dwHighDateTime;
    curUser.LowPart = userTime.dwLowDateTime; curUser.HighPart = userTime.dwHighDateTime;

    if (!g_cpuInitialized) {
        g_prevIdleTime = curIdle;
        g_prevKernelTime = curKernel;
        g_prevUserTime = curUser;
        g_cpuInitialized = true;
        return 0;
    }

    ULONGLONG idleDiff = curIdle.QuadPart - g_prevIdleTime.QuadPart;
    ULONGLONG kernelDiff = curKernel.QuadPart - g_prevKernelTime.QuadPart;
    ULONGLONG userDiff = curUser.QuadPart - g_prevUserTime.QuadPart;

    g_prevIdleTime = curIdle;
    g_prevKernelTime = curKernel;
    g_prevUserTime = curUser;

    ULONGLONG totalDiff = kernelDiff + userDiff;
    if (totalDiff == 0) return 0;

    ULONGLONG usage = (totalDiff > idleDiff) ? ((totalDiff - idleDiff) * 100 / totalDiff) : 0;
    if (usage > 100) usage = 100;
    return (int)usage;
}

// -------------------------------------------------------------
// 内存 (RAM) 使用率与容量采集
// -------------------------------------------------------------
void GetRamStats(int& outPct, double& outUsedGB, double& outTotalGB) {
    outPct = 0;
    outUsedGB = 0.0;
    outTotalGB = 0.0;
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&mem)) {
        outPct = (int)mem.dwMemoryLoad;
        const double kGB = 1024.0 * 1024.0 * 1024.0;
        outTotalGB = (double)mem.ullTotalPhys / kGB;
        outUsedGB = (double)(mem.ullTotalPhys - mem.ullAvailPhys) / kGB;
    }
}

// -------------------------------------------------------------
// GPU 采集 (NVIDIA NVML 原生轻量接口)
// -------------------------------------------------------------
typedef void* nvmlDevice_t;
typedef struct {
    unsigned int gpu;
    unsigned int memory;
} nvmlUtilization_t;

typedef struct {
    unsigned long long total;
    unsigned long long free;
    unsigned long long used;
} nvmlMemory_t;

typedef int (*nvmlInit_t)();
typedef int (*nvmlShutdown_t)();
typedef int (*nvmlDeviceGetCount_t)(unsigned int* deviceCount);
typedef int (*nvmlDeviceGetHandleByIndex_t)(unsigned int index, nvmlDevice_t* device);
typedef int (*nvmlDeviceGetUtilizationRates_t)(nvmlDevice_t device, nvmlUtilization_t* utilization);
typedef int (*nvmlDeviceGetTemperature_t)(nvmlDevice_t device, int sensorType, unsigned int* temp);
typedef int (*nvmlDeviceGetMemoryInfo_t)(nvmlDevice_t device, nvmlMemory_t* memory);
typedef int (*nvmlDeviceGetPowerUsage_t)(nvmlDevice_t device, unsigned int* power);

HMODULE g_hNvml = NULL;
nvmlDevice_t g_nvmlDevice = NULL;
nvmlDeviceGetUtilizationRates_t g_nvmlGetUtil = NULL;
nvmlDeviceGetTemperature_t g_nvmlGetTemp = NULL;
nvmlDeviceGetMemoryInfo_t g_nvmlGetMem = NULL;
nvmlDeviceGetPowerUsage_t g_nvmlGetPower = NULL;
nvmlShutdown_t g_nvmlShutdown = NULL;
bool g_nvmlReady = false;
bool g_nvmlTried = false;

void InitNvml() {
    if (g_nvmlTried) return;
    g_nvmlTried = true;

    g_hNvml = LoadLibraryW(L"nvml.dll");
    if (!g_hNvml) {
        g_hNvml = LoadLibraryW(L"C:\\Windows\\System32\\nvml.dll");
    }
    if (!g_hNvml) return;

    nvmlInit_t nvmlInit_fn = LoadProc<nvmlInit_t>(g_hNvml, "nvmlInit_v2");
    if (!nvmlInit_fn) nvmlInit_fn = LoadProc<nvmlInit_t>(g_hNvml, "nvmlInit");

    nvmlDeviceGetCount_t getCount_fn = LoadProc<nvmlDeviceGetCount_t>(g_hNvml, "nvmlDeviceGetCount_v2");
    if (!getCount_fn) getCount_fn = LoadProc<nvmlDeviceGetCount_t>(g_hNvml, "nvmlDeviceGetCount");

    nvmlDeviceGetHandleByIndex_t getHandle_fn = LoadProc<nvmlDeviceGetHandleByIndex_t>(g_hNvml, "nvmlDeviceGetHandleByIndex_v2");
    if (!getHandle_fn) getHandle_fn = LoadProc<nvmlDeviceGetHandleByIndex_t>(g_hNvml, "nvmlDeviceGetHandleByIndex");

    g_nvmlGetUtil = LoadProc<nvmlDeviceGetUtilizationRates_t>(g_hNvml, "nvmlDeviceGetUtilizationRates");
    g_nvmlGetTemp = LoadProc<nvmlDeviceGetTemperature_t>(g_hNvml, "nvmlDeviceGetTemperature");
    g_nvmlGetMem = LoadProc<nvmlDeviceGetMemoryInfo_t>(g_hNvml, "nvmlDeviceGetMemoryInfo");
    g_nvmlGetPower = LoadProc<nvmlDeviceGetPowerUsage_t>(g_hNvml, "nvmlDeviceGetPowerUsage");
    g_nvmlShutdown = LoadProc<nvmlShutdown_t>(g_hNvml, "nvmlShutdown");

    if (nvmlInit_fn && nvmlInit_fn() == 0 && getCount_fn && getHandle_fn) {
        unsigned int count = 0;
        if (getCount_fn(&count) == 0 && count > 0) {
            if (getHandle_fn(0, &g_nvmlDevice) == 0) {
                g_nvmlReady = true;
            }
        }
    }
}

void GetGpuStats(int& outUsage, int& outTemp, int& outVramPct, double& outVramUsedGB, double& outVramTotalGB, int& outPowerW) {
    outUsage = -1;
    outTemp = -1;
    outVramPct = -1;
    outVramUsedGB = 0.0;
    outVramTotalGB = 0.0;
    outPowerW = -1;

    if (!g_nvmlTried) {
        InitNvml();
    }

    if (g_nvmlReady && g_nvmlDevice) {
        if (g_nvmlGetUtil) {
            nvmlUtilization_t util{};
            if (g_nvmlGetUtil(g_nvmlDevice, &util) == 0) {
                outUsage = (int)util.gpu;
            }
        }
        if (g_nvmlGetTemp) {
            unsigned int temp = 0;
            if (g_nvmlGetTemp(g_nvmlDevice, 0, &temp) == 0) {
                outTemp = (int)temp;
            }
        }
        if (g_nvmlGetMem && g_cfg.showGpuVram) {
            nvmlMemory_t mem{};
            if (g_nvmlGetMem(g_nvmlDevice, &mem) == 0 && mem.total > 0) {
                const double kGB = 1024.0 * 1024.0 * 1024.0;
                outVramUsedGB = (double)mem.used / kGB;
                outVramTotalGB = (double)mem.total / kGB;
                outVramPct = (int)((mem.used * 100ULL) / mem.total);
            }
        }
        if (g_nvmlGetPower && g_cfg.showGpuPower) {
            unsigned int mw = 0;
            if (g_nvmlGetPower(g_nvmlDevice, &mw) == 0) {
                outPowerW = (int)((mw + 500) / 1000);
            }
        }
    }
}

// -------------------------------------------------------------
// 实时网速采集 (GetIfTable2)
// -------------------------------------------------------------
ULONGLONG g_prevNetRx = 0;
ULONGLONG g_prevNetTx = 0;
ULONGLONG g_prevNetTick = 0;
bool g_netInitialized = false;

void ResetNetBaseline() {
    g_netInitialized = false;
    g_prevNetRx = 0;
    g_prevNetTx = 0;
    g_prevNetTick = 0;
}

void GetNetSpeed(double& outDownBps, double& outUpBps) {
    outDownBps = 0.0;
    outUpBps = 0.0;

    MIB_IF_TABLE2* pTable = NULL;
    if (GetIfTable2(&pTable) != NO_ERROR || !pTable) return;

    ULONGLONG totalRx = 0;
    ULONGLONG totalTx = 0;
    for (ULONG i = 0; i < pTable->NumEntries; ++i) {
        const MIB_IF_ROW2& row = pTable->Table[i];
        if (row.OperStatus != IfOperStatusUp) continue;
        if (row.Type == IF_TYPE_SOFTWARE_LOOPBACK || row.Type == IF_TYPE_TUNNEL) continue;
        if (row.InterfaceAndOperStatusFlags.FilterInterface) continue;
        if (!row.InterfaceAndOperStatusFlags.HardwareInterface) continue;
        totalRx += row.InOctets;
        totalTx += row.OutOctets;
    }
    FreeMibTable(pTable);

    ULONGLONG now = GetTickCount64();
    if (!g_netInitialized) {
        g_prevNetRx = totalRx;
        g_prevNetTx = totalTx;
        g_prevNetTick = now;
        g_netInitialized = true;
        return;
    }

    ULONGLONG dtMs = now - g_prevNetTick;
    if (dtMs > 50) {
        if (totalRx >= g_prevNetRx) {
            outDownBps = (double)(totalRx - g_prevNetRx) * 1000.0 / (double)dtMs;
        }
        if (totalTx >= g_prevNetTx) {
            outUpBps = (double)(totalTx - g_prevNetTx) * 1000.0 / (double)dtMs;
        }
    }
    g_prevNetRx = totalRx;
    g_prevNetTx = totalTx;
    g_prevNetTick = now;
}

std::wstring FormatNetRate(double bps) {
    wchar_t buf[32];
    if (bps < 10240.0) {
        swprintf_s(buf, L"%.1fK", bps / 1024.0);
    } else if (bps < 1048576.0) {
        swprintf_s(buf, L"%.0fK", bps / 1024.0);
    } else if (bps < 104857600.0) {
        swprintf_s(buf, L"%.1fM", bps / 1048576.0);
    } else {
        swprintf_s(buf, L"%.0fM", bps / 1048576.0);
    }
    return buf;
}

// -------------------------------------------------------------
// 格式化与配色辅助
// -------------------------------------------------------------
void UpdateTextFormat() {
    if (g_pTextFormat) {
        g_pTextFormat->Release();
        g_pTextFormat = NULL;
    }
    if (g_pDWriteFactory) {
        g_pDWriteFactory->CreateTextFormat(
            g_cfg.fontFamily.c_str(),
            NULL,
            (DWRITE_FONT_WEIGHT)g_cfg.fontWeight,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            g_cfg.fontSize,
            L"en-us",
            &g_pTextFormat
        );
        if (g_pTextFormat) {
            g_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            g_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            g_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
    }
    ++g_fontVersion;
    MarkVisualDirty();
}

float MeasureTextWidth(const std::wstring& str) {
    if (!g_pDWriteFactory || !g_pTextFormat || str.empty()) return 0.0f;
    IDWriteTextLayout* pLayout = NULL;
    if (SUCCEEDED(g_pDWriteFactory->CreateTextLayout(
            str.c_str(), (UINT32)str.size(), g_pTextFormat, 2000.0f, 200.0f, &pLayout))) {
        DWRITE_TEXT_METRICS tm;
        float w = 0.0f;
        if (SUCCEEDED(pLayout->GetMetrics(&tm))) {
            w = ceilf(tm.widthIncludingTrailingWhitespace);
        }
        pLayout->Release();
        return w;
    }
    return (float)str.size() * g_cfg.fontSize * 0.6f;
}

float GetSlotWidth(MetricType type, const std::wstring& sampleText, const std::wstring& actualText) {
    static UINT64 s_cacheFontVer = 0;
    static std::wstring s_sampleStr[8];
    static float s_sampleW[8] = { 0 };

    if (s_cacheFontVer != g_fontVersion) {
        for (int i = 0; i < 8; ++i) {
            s_sampleStr[i].clear();
            s_sampleW[i] = 0.0f;
        }
        s_cacheFontVer = g_fontVersion;
    }

    int idx = (int)type;
    float sw = 0.0f;
    if (idx >= 0 && idx < 8) {
        if (s_sampleStr[idx] != sampleText) {
            s_sampleStr[idx] = sampleText;
            s_sampleW[idx] = MeasureTextWidth(sampleText);
        }
        sw = s_sampleW[idx];
    } else {
        sw = MeasureTextWidth(sampleText);
    }

    float aw = (actualText.size() > sampleText.size()) ? MeasureTextWidth(actualText) : 0.0f;
    return fmaxf(sw, aw) + 2.0f;
}

void ReleaseBackbuffer() {
    if (g_hMemDC) {
        if (g_hOldBitmap) {
            SelectObject(g_hMemDC, g_hOldBitmap);
            g_hOldBitmap = NULL;
        }
        DeleteDC(g_hMemDC);
        g_hMemDC = NULL;
    }
    if (g_hBitmap) {
        DeleteObject(g_hBitmap);
        g_hBitmap = NULL;
    }
    g_pBits = NULL;
    g_dibW = 0;
    g_dibH = 0;
}

bool EnsureBackbuffer(int physW, int physH) {
    if (g_hMemDC && g_hBitmap && g_pBits && g_dibW == physW && g_dibH == physH) {
        return true;
    }
    ReleaseBackbuffer();

    HDC hScreenDC = GetDC(NULL);
    g_hMemDC = CreateCompatibleDC(hScreenDC);
    ReleaseDC(NULL, hScreenDC);
    if (!g_hMemDC) return false;

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = physW;
    bmi.bmiHeader.biHeight = -physH;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    g_hBitmap = CreateDIBSection(g_hMemDC, &bmi, DIB_RGB_COLORS, &g_pBits, NULL, 0);
    if (!g_hBitmap || !g_pBits) {
        ReleaseBackbuffer();
        return false;
    }
    g_hOldBitmap = (HBITMAP)SelectObject(g_hMemDC, g_hBitmap);
    g_dibW = physW;
    g_dibH = physH;
    return true;
}

D2D1_COLOR_F ColorFromTheme() {
    switch (g_cfg.colorTheme) {
    case THEME_BLACK:  return D2D1::ColorF(0.08f, 0.08f, 0.09f, 1.0f);
    case THEME_SILVER: return D2D1::ColorF(0.85f, 0.87f, 0.91f, 1.0f);
    case THEME_CYAN:   return D2D1::ColorF(0.00f, 0.90f, 1.00f, 1.0f);
    case THEME_GREEN:  return D2D1::ColorF(0.29f, 0.87f, 0.50f, 1.0f);
    case THEME_AMBER:  return D2D1::ColorF(0.98f, 0.75f, 0.14f, 1.0f);
    case THEME_PINK:   return D2D1::ColorF(0.98f, 0.66f, 0.83f, 1.0f);
    case THEME_VIOLET: return D2D1::ColorF(0.77f, 0.71f, 0.99f, 1.0f);
    case THEME_CUSTOM:
        return D2D1::ColorF(
            GetRValue(g_cfg.customColor) / 255.0f,
            GetGValue(g_cfg.customColor) / 255.0f,
            GetBValue(g_cfg.customColor) / 255.0f,
            1.0f
        );
    case THEME_WHITE:
    default:
        return D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

D2D1_COLOR_F ColorForMetric(MetricType type, int severity) {
    if (g_cfg.dynamicLoadColor) {
        if (severity == 2) return D2D1::ColorF(0.97f, 0.44f, 0.44f, 1.0f); // 高负载警示红
        if (severity == 1) return D2D1::ColorF(0.98f, 0.75f, 0.14f, 1.0f); // 中高负载橙黄
    }
    if (g_cfg.perMetricColor) {
        switch (type) {
        case METRIC_CPU:       return D2D1::ColorF(0.38f, 0.82f, 1.00f, 1.0f); // 天空蓝
        case METRIC_GPU_USAGE: return D2D1::ColorF(0.30f, 0.90f, 0.58f, 1.0f); // 翡翠绿
        case METRIC_GPU_TEMP:  return D2D1::ColorF(1.00f, 0.72f, 0.30f, 1.0f); // 暖琥珀
        case METRIC_GPU_VRAM:  return D2D1::ColorF(0.56f, 0.93f, 0.86f, 1.0f); // 薄荷青
        case METRIC_GPU_POWER: return D2D1::ColorF(0.99f, 0.88f, 0.38f, 1.0f); // 柠檬黄
        case METRIC_RAM:       return D2D1::ColorF(0.78f, 0.68f, 1.00f, 1.0f); // 淡紫罗兰
        case METRIC_NET:       return D2D1::ColorF(0.99f, 0.68f, 0.84f, 1.0f); // 樱花粉
        case METRIC_CLOCK:     return D2D1::ColorF(0.93f, 0.95f, 0.98f, 1.0f); // 冷月白
        }
    }
    return ColorFromTheme();
}

struct MetricSlot {
    MetricType type;
    std::wstring text;
    std::wstring sampleText;
    int severity;
    float x, y, w, h;
};

static std::wstring FormatLabel(const wchar_t* stdName, const wchar_t* compactName, const wchar_t* zhName, const std::wstring& val) {
    switch (g_cfg.labelStyle) {
    case LABEL_COMPACT:  return std::wstring(compactName) + L":" + val;
    case LABEL_CHINESE:  return std::wstring(zhName) + L" " + val;
    case LABEL_NOCOLON:  return std::wstring(stdName) + L" " + val;
    case LABEL_VALUE:    return val;
    case LABEL_BRACKETS: return std::wstring(L"[") + stdName + L" " + val + L"]";
    case LABEL_STANDARD:
    default:             return std::wstring(stdName) + L": " + val;
    }
}

void BuildMetricSlots(std::vector<MetricSlot>& slots) {
    slots.clear();

    int cpu = g_cfg.showCpu ? GetCpuUsage() : 0;
    int ramPct = 0;
    double ramUsedGB = 0.0, ramTotalGB = 0.0;
    if (g_cfg.showRam || g_cfg.showRamGB) {
        GetRamStats(ramPct, ramUsedGB, ramTotalGB);
    }

    int gpuUsage = -1, gpuTemp = -1, gpuVramPct = -1, gpuPowerW = -1;
    double gpuVramUsedGB = 0.0, gpuVramTotalGB = 0.0;
    if (g_cfg.showGpu || g_cfg.showGpuTemp || g_cfg.showGpuVram || g_cfg.showGpuPower) {
        GetGpuStats(gpuUsage, gpuTemp, gpuVramPct, gpuVramUsedGB, gpuVramTotalGB, gpuPowerW);
    }

    auto severityForPct = [](int pct) -> int {
        if (pct >= 90) return 2;
        if (pct >= 70) return 1;
        return 0;
    };
    auto severityForTemp = [](int temp) -> int {
        if (temp >= 85) return 2;
        if (temp >= 75) return 1;
        return 0;
    };

    wchar_t buf[64];

    if (g_cfg.showCpu) {
        swprintf_s(buf, L"%d%%", cpu);
        slots.push_back({
            METRIC_CPU,
            FormatLabel(L"CPU", L"C", L"处理器", buf),
            FormatLabel(L"CPU", L"C", L"处理器", L"100%"),
            severityForPct(cpu),
            0, 0, 0, 0
        });
    }

    if (g_cfg.showGpu) {
        if (gpuUsage >= 0) swprintf_s(buf, L"%d%%", gpuUsage);
        else wcscpy_s(buf, L"--");
        slots.push_back({
            METRIC_GPU_USAGE,
            FormatLabel(L"GPU", L"G", L"显卡", buf),
            FormatLabel(L"GPU", L"G", L"显卡", L"100%"),
            (gpuUsage >= 0) ? severityForPct(gpuUsage) : 0,
            0, 0, 0, 0
        });
    }

    if (g_cfg.showGpuTemp) {
        const wchar_t* unit = g_cfg.showTempUnit ? L"\x00B0" L"C" : L"";
        if (gpuTemp >= 0) swprintf_s(buf, L"%d%s", gpuTemp, unit);
        else swprintf_s(buf, L"--%s", unit);

        std::wstring sample = g_cfg.showTempUnit ? L"99\x00B0" L"C" : L"99";
        // 当不显示 GPU 占用率而单独显示温度时加上简短前缀避免孤立数字
        std::wstring text = buf;
        if (!g_cfg.showGpu) {
            text = FormatLabel(L"TEMP", L"T", L"温度", buf);
            sample = FormatLabel(L"TEMP", L"T", L"温度", sample);
        }
        slots.push_back({
            METRIC_GPU_TEMP,
            text,
            sample,
            (gpuTemp >= 0) ? severityForTemp(gpuTemp) : 0,
            0, 0, 0, 0
        });
    }

    if (g_cfg.showGpuVram) {
        if (gpuVramPct >= 0) {
            swprintf_s(buf, L"%.1f/%.0fG", gpuVramUsedGB, gpuVramTotalGB);
        } else {
            wcscpy_s(buf, L"--");
        }
        slots.push_back({
            METRIC_GPU_VRAM,
            FormatLabel(L"VRAM", L"V", L"显存", buf),
            FormatLabel(L"VRAM", L"V", L"显存", L"99.9/99G"),
            (gpuVramPct >= 0) ? severityForPct(gpuVramPct) : 0,
            0, 0, 0, 0
        });
    }

    if (g_cfg.showGpuPower) {
        if (gpuPowerW >= 0) swprintf_s(buf, L"%dW", gpuPowerW);
        else wcscpy_s(buf, L"--W");
        slots.push_back({
            METRIC_GPU_POWER,
            FormatLabel(L"PWR", L"W", L"功耗", buf),
            FormatLabel(L"PWR", L"W", L"功耗", L"999W"),
            0,
            0, 0, 0, 0
        });
    }

    if (g_cfg.showRam || g_cfg.showRamGB) {
        std::wstring val, sampleVal;
        if (g_cfg.showRam && g_cfg.showRamGB) {
            swprintf_s(buf, L"%d%% (%.1f/%.0fG)", ramPct, ramUsedGB, ramTotalGB);
            val = buf;
            sampleVal = L"100% (99.9/999G)";
        } else if (g_cfg.showRamGB) {
            swprintf_s(buf, L"%.1f/%.0fG", ramUsedGB, ramTotalGB);
            val = buf;
            sampleVal = L"99.9/999G";
        } else {
            swprintf_s(buf, L"%d%%", ramPct);
            val = buf;
            sampleVal = L"100%";
        }
        slots.push_back({
            METRIC_RAM,
            FormatLabel(L"RAM", L"R", L"内存", val),
            FormatLabel(L"RAM", L"R", L"内存", sampleVal),
            severityForPct(ramPct),
            0, 0, 0, 0
        });
    }

    if (g_cfg.showNet) {
        double downBps = 0.0, upBps = 0.0;
        GetNetSpeed(downBps, upBps);
        std::wstring netStr = L"\x2193" + FormatNetRate(downBps) + L" \x2191" + FormatNetRate(upBps);
        std::wstring netSample = L"\x2193" L"99.9M \x2191" L"99.9M";
        if (g_cfg.labelStyle == LABEL_CHINESE) {
            netStr = L"\x4E0B" + FormatNetRate(downBps) + L" \x4E0A" + FormatNetRate(upBps);
            netSample = L"\x4E0B" L"99.9M \x4E0A" L"99.9M";
        }
        slots.push_back({ METRIC_NET, netStr, netSample, 0, 0, 0, 0, 0 });
    }

    if (g_cfg.showClock) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        swprintf_s(buf, L"%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
        slots.push_back({ METRIC_CLOCK, buf, L"88:88:88", 0, 0, 0, 0, 0 });
    }

    // 兜底：保证至少有一项
    if (slots.empty()) {
        g_cfg.showCpu = true;
        swprintf_s(buf, L"%d%%", GetCpuUsage());
        slots.push_back({
            METRIC_CPU,
            FormatLabel(L"CPU", L"C", L"处理器", buf),
            FormatLabel(L"CPU", L"C", L"处理器", L"100%"),
            0, 0, 0, 0, 0
        });
    }
}

// -------------------------------------------------------------
// 屏幕停靠位置计算
// -------------------------------------------------------------
void ComputeWindowPosition(int physW, int physH, int& outX, int& outY) {
    if (g_cfg.dockPreset == DOCK_CUSTOM) {
        int vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
        int vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        outX = g_cfg.customPosX;
        outY = g_cfg.customPosY;
        if (outX < vx - physW + 24) outX = vx;
        if (outY < vy - physH + 16) outY = vy;
        if (outX > vx + vw - 24) outX = vx + vw - physW;
        if (outY > vy + vh - 16) outY = vy + vh - physH;
        return;
    }

    HMONITOR hMon = MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfoW(hMon, &mi)) {
        outX = (int)roundf(8.0f * g_dpiScale);
        outY = (int)roundf(6.0f * g_dpiScale);
        return;
    }

    RECT wa = mi.rcWork;
    int marginX = (int)roundf(8.0f * g_dpiScale);
    int marginY = (int)roundf(6.0f * g_dpiScale);

    switch (g_cfg.dockPreset) {
    case DOCK_TOP_CENTER:
        outX = wa.left + ((wa.right - wa.left) - physW) / 2;
        outY = wa.top + marginY;
        break;
    case DOCK_TOP_RIGHT:
        outX = wa.right - physW - marginX;
        outY = wa.top + marginY;
        break;
    case DOCK_BOTTOM_LEFT:
        outX = wa.left + marginX;
        outY = wa.bottom - physH - marginY;
        break;
    case DOCK_BOTTOM_CENTER:
        outX = wa.left + ((wa.right - wa.left) - physW) / 2;
        outY = wa.bottom - physH - marginY;
        break;
    case DOCK_BOTTOM_RIGHT:
        outX = wa.right - physW - marginX;
        outY = wa.bottom - physH - marginY;
        break;
    case DOCK_TOP_LEFT:
    default:
        outX = wa.left + marginX;
        outY = wa.top + marginY;
        break;
    }
}

// -------------------------------------------------------------
// 全屏应用检测
// -------------------------------------------------------------
bool IsForegroundFullscreen() {
    HWND hFg = GetForegroundWindow();
    if (!hFg || hFg == g_hWnd || hFg == GetDesktopWindow() || hFg == GetShellWindow()) return false;
    if (!IsWindowVisible(hFg) || IsIconic(hFg)) return false;

    wchar_t cls[64] = { 0 };
    GetClassNameW(hFg, cls, 64);
    if (wcscmp(cls, L"Progman") == 0 || wcscmp(cls, L"WorkerW") == 0 || wcscmp(cls, L"Shell_TrayWnd") == 0) {
        return false;
    }

    HMONITOR hMonSelf = MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTOPRIMARY);
    HMONITOR hMonFg = MonitorFromWindow(hFg, MONITOR_DEFAULTTONEAREST);
    if (hMonFg != hMonSelf) return false;

    RECT rcFg{};
    if (!GetWindowRect(hFg, &rcFg)) return false;

    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfoW(hMonFg, &mi)) return false;

    return rcFg.left <= mi.rcMonitor.left &&
           rcFg.top <= mi.rcMonitor.top &&
           rcFg.right >= mi.rcMonitor.right &&
           rcFg.bottom >= mi.rcMonitor.bottom;
}

// -------------------------------------------------------------
// Direct2D + DirectWrite 分层极清渲染
// -------------------------------------------------------------
void RenderOverlay() {
    if (!g_hWnd || !g_pD2DFactory || !g_pDWriteFactory || !g_pDCRenderTarget || !g_pBrush) return;
    if (!g_isShown) return;

    if (g_cfg.hideOnFullscreen && !g_dragUnlocked && !g_dialogOpen) {
        bool fs = IsForegroundFullscreen();
        if (fs && !g_hiddenByFullscreen) {
            g_hiddenByFullscreen = true;
            ShowWindow(g_hWnd, SW_HIDE);
            return;
        } else if (!fs && g_hiddenByFullscreen) {
            g_hiddenByFullscreen = false;
            ShowWindow(g_hWnd, SW_SHOWNOACTIVATE);
            MarkVisualDirty();
        }
        if (g_hiddenByFullscreen) return;
    } else if (g_hiddenByFullscreen) {
        g_hiddenByFullscreen = false;
        ShowWindow(g_hWnd, SW_SHOWNOACTIVATE);
        MarkVisualDirty();
    }

    static std::vector<MetricSlot> slots;
    BuildMetricSlots(slots);

    // 间距参数 (DIP)
    float colGap = 6.0f;
    float tightGap = 2.0f;
    float rowGap = 2.0f;
    if (g_cfg.spacingMode == SPACING_COMPACT) {
        colGap = 4.0f;
        tightGap = 1.0f;
        rowGap = 1.0f;
    } else if (g_cfg.spacingMode == SPACING_WIDE) {
        colGap = 12.0f;
        tightGap = 4.0f;
        rowGap = 4.0f;
    }

    bool hasBg = (g_cfg.bgStyle != BG_NONE) || g_dragUnlocked;
    float padX = hasBg ? 11.0f : 2.0f;
    float padY = hasBg ? 4.0f : 0.0f;
    float rowH = hasBg ? ceilf(g_cfg.fontSize * 1.50f) : fmaxf(24.0f, ceilf(g_cfg.fontSize * 1.65f));

    // 为每个槽位计算缓存后的固定最大宽度 (零抖动 + 零重复 COM 测宽)
    static std::wstring frameKey;
    frameKey.clear();
    for (size_t i = 0; i < slots.size(); ++i) {
        slots[i].w = GetSlotWidth(slots[i].type, slots[i].sampleText, slots[i].text);
        slots[i].h = rowH;
        frameKey.append(slots[i].text);
        frameKey.push_back(L'\t');
        frameKey.push_back((wchar_t)(L'0' + slots[i].severity));
        frameKey.push_back(L'|');
    }

    float logicalW = 0.0f;
    float logicalH = 0.0f;
    bool multiRow = false;

    if (g_cfg.layoutMode == LAYOUT_VERTICAL) {
        multiRow = (slots.size() > 1);
        float maxW = 0.0f;
        float curY = padY;
        for (size_t i = 0; i < slots.size(); ++i) {
            slots[i].x = padX;
            slots[i].y = curY;
            if (slots[i].w > maxW) maxW = slots[i].w;
            curY += rowH + (i + 1 < slots.size() ? rowGap : 0.0f);
        }
        logicalW = padX * 2.0f + maxW;
        logicalH = curY + padY;
    } else if (g_cfg.layoutMode == LAYOUT_TWO_ROWS && slots.size() >= 2) {
        multiRow = true;
        size_t split = (slots.size() + 1) / 2;
        auto layoutRow = [&](size_t start, size_t end, float yPos) -> float {
            float curX = padX;
            for (size_t i = start; i < end; ++i) {
                if (i > start) {
                    bool isGpuTempPair = (slots[i].type == METRIC_GPU_TEMP && slots[i - 1].type == METRIC_GPU_USAGE);
                    curX += isGpuTempPair ? tightGap : colGap;
                }
                slots[i].x = curX;
                slots[i].y = yPos;
                curX += slots[i].w;
            }
            return curX + padX;
        };
        float w1 = layoutRow(0, split, padY);
        float w2 = layoutRow(split, slots.size(), padY + rowH + rowGap);
        logicalW = fmaxf(w1, w2);
        logicalH = padY * 2.0f + rowH * 2.0f + rowGap;
    } else {
        float curX = padX;
        for (size_t i = 0; i < slots.size(); ++i) {
            if (i > 0) {
                bool isGpuTempPair = (slots[i].type == METRIC_GPU_TEMP && slots[i - 1].type == METRIC_GPU_USAGE);
                curX += isGpuTempPair ? tightGap : colGap;
            }
            slots[i].x = curX;
            slots[i].y = padY;
            curX += slots[i].w;
        }
        logicalW = curX + padX;
        logicalH = padY * 2.0f + rowH;
    }

    int physW = (int)ceilf(logicalW * g_dpiScale);
    int physH = (int)ceilf(logicalH * g_dpiScale);
    if (physW < 16) physW = 16;
    if (physH < 16) physH = 16;

    g_winPhysW = physW;
    g_winPhysH = physH;

    if (!g_isDragging) {
        ComputeWindowPosition(physW, physH, g_posX, g_posY);
    }

    // 脏检测：数值/样式/位置均未变化时直接跳过整帧绘制与合成
    if (g_configVersion == g_lastRenderedConfigVer &&
        physW == g_lastRenderedPhysW &&
        physH == g_lastRenderedPhysH &&
        g_posX == g_lastRenderedPosX &&
        g_posY == g_lastRenderedPosY &&
        frameKey == g_lastFrameKey) {
        return;
    }

    if (!EnsureBackbuffer(physW, physH)) return;
    memset(g_pBits, 0, (size_t)physW * (size_t)physH * 4);

    RECT rc = { 0, 0, physW, physH };
    if (SUCCEEDED(g_pDCRenderTarget->BindDC(g_hMemDC, &rc))) {
            g_pDCRenderTarget->SetDpi(96.0f * g_dpiScale, 96.0f * g_dpiScale);
            g_pDCRenderTarget->BeginDraw();
            g_pDCRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
            g_pDCRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            // 1. 绘制可选背景底板 / 拖动提示高亮
            if (hasBg) {
                float r = (g_cfg.bgStyle == BG_DARK_BAR || multiRow) ? 7.0f : (logicalH * 0.5f);
                D2D1_ROUNDED_RECT rr = D2D1::RoundedRect(
                    D2D1::RectF(0.5f, 0.5f, logicalW - 0.5f, logicalH - 0.5f), r, r);

                if (g_dragUnlocked) {
                    g_pBrush->SetColor(D2D1::ColorF(0.10f, 0.36f, 0.65f, 0.45f));
                    g_pDCRenderTarget->FillRoundedRectangle(rr, g_pBrush);
                    g_pBrush->SetColor(D2D1::ColorF(0.40f, 0.78f, 1.00f, 0.90f));
                    g_pDCRenderTarget->DrawRoundedRectangle(rr, g_pBrush, 1.2f);
                } else if (g_cfg.bgStyle == BG_DARK_PILL) {
                    g_pBrush->SetColor(D2D1::ColorF(0.06f, 0.07f, 0.09f, 0.66f));
                    g_pDCRenderTarget->FillRoundedRectangle(rr, g_pBrush);
                    g_pBrush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.14f));
                    g_pDCRenderTarget->DrawRoundedRectangle(rr, g_pBrush, 1.0f);
                } else if (g_cfg.bgStyle == BG_LIGHT_PILL) {
                    g_pBrush->SetColor(D2D1::ColorF(0.96f, 0.97f, 0.99f, 0.72f));
                    g_pDCRenderTarget->FillRoundedRectangle(rr, g_pBrush);
                    g_pBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.12f));
                    g_pDCRenderTarget->DrawRoundedRectangle(rr, g_pBrush, 1.0f);
                } else if (g_cfg.bgStyle == BG_DARK_BAR) {
                    g_pBrush->SetColor(D2D1::ColorF(0.05f, 0.06f, 0.08f, 0.76f));
                    g_pDCRenderTarget->FillRoundedRectangle(rr, g_pBrush);
                } else if (g_cfg.bgStyle == BG_NEON_PILL) {
                    g_pBrush->SetColor(D2D1::ColorF(0.04f, 0.06f, 0.10f, 0.68f));
                    g_pDCRenderTarget->FillRoundedRectangle(rr, g_pBrush);
                    D2D1_COLOR_F accent = ColorFromTheme();
                    if (g_cfg.colorTheme == THEME_BLACK || g_cfg.colorTheme == THEME_WHITE) {
                        accent = D2D1::ColorF(0.22f, 0.78f, 1.00f, 0.85f);
                    } else {
                        accent.a = 0.85f;
                    }
                    g_pBrush->SetColor(accent);
                    g_pDCRenderTarget->DrawRoundedRectangle(rr, g_pBrush, 1.2f);
                }
            }

            // 2. 逐槽绘制阴影 + 主体文字
            for (size_t i = 0; i < slots.size(); ++i) {
                const MetricSlot& s = slots[i];
                UINT32 len = (UINT32)s.text.size();
                D2D1_COLOR_F mainCol = ColorForMetric(s.type, s.severity);

                float lum = 0.299f * mainCol.r + 0.587f * mainCol.g + 0.114f * mainCol.b;
                bool darkText = (lum < 0.35f);
                D2D1_COLOR_F shCol = darkText
                    ? D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.65f)
                    : D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.75f);

                if (g_cfg.shadowStyle == SHADOW_SUBTLE) {
                    g_pBrush->SetColor(shCol);
                    g_pDCRenderTarget->DrawText(
                        s.text.c_str(), len, g_pTextFormat,
                        D2D1::RectF(s.x + 1.0f, s.y + 1.0f, s.x + s.w + 1.0f, s.y + s.h + 1.0f),
                        g_pBrush);
                } else if (g_cfg.shadowStyle == SHADOW_STRONG) {
                    D2D1_COLOR_F shFar = shCol;
                    shFar.a = 0.42f;
                    g_pBrush->SetColor(shFar);
                    g_pDCRenderTarget->DrawText(
                        s.text.c_str(), len, g_pTextFormat,
                        D2D1::RectF(s.x + 2.0f, s.y + 2.0f, s.x + s.w + 2.0f, s.y + s.h + 2.0f),
                        g_pBrush);
                    D2D1_COLOR_F shNear = shCol;
                    shNear.a = 0.85f;
                    g_pBrush->SetColor(shNear);
                    g_pDCRenderTarget->DrawText(
                        s.text.c_str(), len, g_pTextFormat,
                        D2D1::RectF(s.x + 1.0f, s.y + 1.0f, s.x + s.w + 1.0f, s.y + s.h + 1.0f),
                        g_pBrush);
                } else if (g_cfg.shadowStyle == SHADOW_OUTLINE) {
                    D2D1_COLOR_F shOut = shCol;
                    shOut.a = 0.85f;
                    g_pBrush->SetColor(shOut);
                    const float dx[4] = { -1.0f, 1.0f, 0.0f, 0.0f };
                    const float dy[4] = { 0.0f, 0.0f, -1.0f, 1.0f };
                    for (int k = 0; k < 4; ++k) {
                        g_pDCRenderTarget->DrawText(
                            s.text.c_str(), len, g_pTextFormat,
                            D2D1::RectF(s.x + dx[k], s.y + dy[k], s.x + s.w + dx[k], s.y + s.h + dy[k]),
                            g_pBrush);
                    }
                }

                g_pBrush->SetColor(mainCol);
                g_pDCRenderTarget->DrawText(
                    s.text.c_str(), len, g_pTextFormat,
                    D2D1::RectF(s.x, s.y, s.x + s.w, s.y + s.h),
                    g_pBrush);
            }

            g_pDCRenderTarget->EndDraw();

            POINT ptSrc = { 0, 0 };
            SIZE sizeWnd = { physW, physH };
            POINT ptDst = { g_posX, g_posY };

            BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.SourceConstantAlpha = (BYTE)roundf(255.0f * (g_cfg.opacityPct / 100.0f));
            blend.AlphaFormat = AC_SRC_ALPHA;

            UpdateLayeredWindow(g_hWnd, NULL, &ptDst, &sizeWnd, g_hMemDC, &ptSrc, 0, &blend, ULW_ALPHA);

            g_lastRenderedConfigVer = g_configVersion;
            g_lastRenderedPhysW = physW;
            g_lastRenderedPhysH = physH;
            g_lastRenderedPosX = g_posX;
            g_lastRenderedPosY = g_posY;
            g_lastFrameKey = frameKey;
    }
}

// -------------------------------------------------------------
// 拖动锁定切换 & 系统字体/调色盘弹窗
// -------------------------------------------------------------
void SetDragUnlocked(bool unlock) {
    g_dragUnlocked = unlock;
    LONG_PTR ex = GetWindowLongPtrW(g_hWnd, GWL_EXSTYLE);
    if (unlock) {
        ex &= ~WS_EX_TRANSPARENT;
    } else {
        ex |= WS_EX_TRANSPARENT;
    }
    SetWindowLongPtrW(g_hWnd, GWL_EXSTYLE, ex);
    SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    MarkVisualDirty();
    RenderOverlay();
}

void PickSystemFont() {
    g_dialogOpen = true;
    HDC hDC = GetDC(NULL);
    int logY = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(NULL, hDC);

    LOGFONTW lf{};
    lf.lfHeight = -MulDiv((int)roundf(g_cfg.fontSize * 72.0f / 96.0f), logY, 72);
    lf.lfWeight = g_cfg.fontWeight;
    lf.lfCharSet = DEFAULT_CHARSET;
    wcsncpy_s(lf.lfFaceName, g_cfg.fontFamily.c_str(), LF_FACESIZE - 1);

    CHOOSEFONTW cf{};
    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = NULL;
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOSCRIPTSEL;

    SetForegroundWindow(g_hWnd);
    if (ChooseFontW(&cf)) {
        if (wcslen(lf.lfFaceName) > 0) {
            g_cfg.fontFamily = lf.lfFaceName;
        }
        if (cf.iPointSize > 0) {
            float dip = roundf((cf.iPointSize / 10.0f) * (96.0f / 72.0f) * 2.0f) * 0.5f;
            if (dip < 8.0f) dip = 8.0f;
            if (dip > 36.0f) dip = 36.0f;
            g_cfg.fontSize = dip;
        }
        if (lf.lfWeight >= 650) g_cfg.fontWeight = DWRITE_FONT_WEIGHT_BOLD;
        else if (lf.lfWeight >= 550) g_cfg.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
        else if (lf.lfWeight <= 350) g_cfg.fontWeight = DWRITE_FONT_WEIGHT_LIGHT;
        else g_cfg.fontWeight = DWRITE_FONT_WEIGHT_NORMAL;

        UpdateTextFormat();
        SaveConfig();
        RenderOverlay();
    }
    g_dialogOpen = false;
    TrimWorkingSet();
}

void PickCustomColor() {
    g_dialogOpen = true;
    static COLORREF customPalette[16] = {
        RGB(255,255,255), RGB(0,229,255), RGB(74,222,128), RGB(251,191,36),
        RGB(249,168,212), RGB(196,181,253), RGB(248,113,113), RGB(216,222,233),
        RGB(20,20,24), RGB(56,189,248), RGB(163,230,53), RGB(251,146,60),
        RGB(244,114,182), RGB(167,139,250), RGB(45,212,191), RGB(250,204,21)
    };

    CHOOSECOLORW cc{};
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = NULL;
    cc.rgbResult = g_cfg.customColor;
    cc.lpCustColors = customPalette;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    SetForegroundWindow(g_hWnd);
    if (ChooseColorW(&cc)) {
        g_cfg.customColor = cc.rgbResult;
        g_cfg.colorTheme = THEME_CUSTOM;
        MarkVisualDirty();
        SaveConfig();
        RenderOverlay();
    }
    g_dialogOpen = false;
    TrimWorkingSet();
}

// -------------------------------------------------------------
// 开机自启动注册表管理
// -------------------------------------------------------------
bool IsAutoRunEnabled() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        wchar_t path[MAX_PATH];
        DWORD size = sizeof(path);
        LSTATUS res = RegQueryValueExW(hKey, L"Monitor", NULL, NULL, (LPBYTE)path, &size);
        RegCloseKey(hKey);
        return (res == ERROR_SUCCESS);
    }
    return false;
}

void SetAutoRun(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            wchar_t exePath[MAX_PATH];
            GetModuleFileNameW(NULL, exePath, MAX_PATH);
            RegSetValueExW(hKey, L"Monitor", 0, REG_SZ, (const BYTE*)exePath, (DWORD)((wcslen(exePath) + 1) * sizeof(wchar_t)));
        } else {
            RegDeleteValueW(hKey, L"Monitor");
        }
        RegCloseKey(hKey);
    }
}

// -------------------------------------------------------------
// 分级托盘右键菜单 (中/英双语)
// -------------------------------------------------------------
void ShowTrayMenu(HWND hWnd) {
    HMENU hMenu = CreatePopupMenu();

    // 1. 显示项目
    HMENU hShowMenu = CreatePopupMenu();
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showCpu ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_CPU, T(L"CPU 占用率", L"CPU Usage"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showGpu ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_GPU, T(L"GPU 占用率", L"GPU Usage"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showGpuTemp ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_GPU_TEMP, T(L"GPU 温度", L"GPU Temperature"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showGpuVram ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_GPU_VRAM, T(L"GPU 显存 (VRAM)", L"GPU VRAM"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showGpuPower ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_GPU_POWER, T(L"GPU 功耗 (W)", L"GPU Power (W)"));
    AppendMenuW(hShowMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showRam ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_RAM, T(L"RAM 百分比", L"RAM %"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showRamGB ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_RAM_GB, T(L"RAM 已用/总量 (GB)", L"RAM Used/Total (GB)"));
    AppendMenuW(hShowMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showNet ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_NET, T(L"实时网速 (↓ / ↑)", L"Network Speed (↓ / ↑)"));
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showClock ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_CLOCK, T(L"当前时间 (HH:MM:SS)", L"Clock (HH:MM:SS)"));
    AppendMenuW(hShowMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hShowMenu, MF_STRING | (g_cfg.showTempUnit ? MF_CHECKED : MF_UNCHECKED), IDM_SHOW_TEMP_UNIT, T(L"温度显示 °C 单位", L"Show °C Unit"));
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hShowMenu, T(L"显示项目", L"Display Metrics"));

    // 2. 文字与字体
    HMENU hTextMenu = CreatePopupMenu();
    HMENU hFontSizeMenu = CreatePopupMenu();
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 10.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_10, T(L"超小 (10px)", L"Tiny (10px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 11.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_11, T(L"紧凑 (11px)", L"Compact (11px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 12.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_12, T(L"极小 (12px)", L"Small (12px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 13.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_13, T(L"原生时钟字号 (13px, 默认)", L"Taskbar Default (13px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 14.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_14, T(L"适中 (14px)", L"Medium (14px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 15.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_15, T(L"清晰 (15px)", L"Clear (15px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 18.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_18, T(L"大字 (18px)", L"Large (18px)"));
    AppendMenuW(hFontSizeMenu, MF_STRING | (fabsf(g_cfg.fontSize - 22.0f) < 0.05f ? MF_CHECKED : MF_UNCHECKED), IDM_FONT_22, T(L"超大 (22px)", L"Extra Large (22px)"));

    HMENU hWeightMenu = CreatePopupMenu();
    AppendMenuW(hWeightMenu, MF_STRING | (g_cfg.fontWeight == DWRITE_FONT_WEIGHT_LIGHT ? MF_CHECKED : MF_UNCHECKED), IDM_WEIGHT_LIGHT, T(L"细体 (Light)", L"Light"));
    AppendMenuW(hWeightMenu, MF_STRING | (g_cfg.fontWeight == DWRITE_FONT_WEIGHT_NORMAL ? MF_CHECKED : MF_UNCHECKED), IDM_WEIGHT_NORMAL, T(L"常规 (Normal, 默认)", L"Normal (Default)"));
    AppendMenuW(hWeightMenu, MF_STRING | (g_cfg.fontWeight == DWRITE_FONT_WEIGHT_SEMI_BOLD ? MF_CHECKED : MF_UNCHECKED), IDM_WEIGHT_SEMIBOLD, T(L"半粗 (SemiBold)", L"SemiBold"));
    AppendMenuW(hWeightMenu, MF_STRING | (g_cfg.fontWeight == DWRITE_FONT_WEIGHT_BOLD ? MF_CHECKED : MF_UNCHECKED), IDM_WEIGHT_BOLD, T(L"粗体 (Bold)", L"Bold"));

    HMENU hFamilyMenu = CreatePopupMenu();
    const wchar_t* familyLabelsZh[IDM_FAMILY_COUNT] = {
        L"Segoe UI Variable Text (Win11 默认)",
        L"Segoe UI (经典原生)",
        L"Bahnschrift (工业数字)",
        L"Constantia (优雅衬线)",
        L"Cascadia Mono (等宽极客, 数字不晃)",
        L"Cascadia Code (等宽编程)",
        L"Consolas (经典等宽)",
        L"Microsoft YaHei UI (微软雅黑 UI)",
        L"Calibri (圆润现代)",
        L"Arial (通用无衬线)"
    };
    const wchar_t* familyLabelsEn[IDM_FAMILY_COUNT] = {
        L"Segoe UI Variable Text (Win11 Default)",
        L"Segoe UI (Classic)",
        L"Bahnschrift (Industrial)",
        L"Constantia (Serif)",
        L"Cascadia Mono (Tabular Monospace)",
        L"Cascadia Code (Monospace)",
        L"Consolas (Classic Mono)",
        L"Microsoft YaHei UI",
        L"Calibri",
        L"Arial"
    };
    bool isPresetFamily = false;
    for (int i = 0; i < IDM_FAMILY_COUNT; ++i) {
        bool match = (g_cfg.fontFamily == kFontPresets[i]);
        if (match) isPresetFamily = true;
        AppendMenuW(hFamilyMenu, MF_STRING | (match ? MF_CHECKED : MF_UNCHECKED),
            IDM_FAMILY_BASE + i, T(familyLabelsZh[i], familyLabelsEn[i]));
    }
    AppendMenuW(hFamilyMenu, MF_SEPARATOR, 0, NULL);
    wchar_t pickFontText[128];
    if (!isPresetFamily) {
        swprintf_s(pickFontText, T(L"选择系统字体... (当前: %s)", L"Choose System Font... (%s)"), g_cfg.fontFamily.c_str());
    } else {
        wcscpy_s(pickFontText, T(L"选择系统字体...", L"Choose System Font..."));
    }
    AppendMenuW(hFamilyMenu, MF_STRING | (!isPresetFamily ? MF_CHECKED : MF_UNCHECKED), IDM_FAMILY_PICK, pickFontText);

    HMENU hLabelMenu = CreatePopupMenu();
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_STANDARD ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_STANDARD, T(L"标准英文 (CPU: 12%, 默认)", L"Standard (CPU: 12%, Default)"));
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_COMPACT ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_COMPACT, T(L"紧凑简写 (C:12% G:30%)", L"Compact (C:12% G:30%)"));
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_CHINESE ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_CHINESE, T(L"中文标签 (处理器 12%)", L"Chinese Labels (处理器 12%)"));
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_NOCOLON ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_NOCOLON, T(L"无冒号极简 (CPU 12%)", L"No Colon (CPU 12%)"));
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_VALUE ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_VALUEONLY, T(L"纯数值 (12%  30%)", L"Values Only (12%  30%)"));
    AppendMenuW(hLabelMenu, MF_STRING | (g_cfg.labelStyle == LABEL_BRACKETS ? MF_CHECKED : MF_UNCHECKED), IDM_LABEL_BRACKETS, T(L"方括号 ([CPU 12%])", L"Brackets ([CPU 12%])"));

    AppendMenuW(hTextMenu, MF_POPUP, (UINT_PTR)hFamilyMenu, T(L"字体风格", L"Font Family"));
    AppendMenuW(hTextMenu, MF_POPUP, (UINT_PTR)hFontSizeMenu, T(L"字体大小", L"Font Size"));
    AppendMenuW(hTextMenu, MF_POPUP, (UINT_PTR)hWeightMenu, T(L"字体粗细", L"Font Weight"));
    AppendMenuW(hTextMenu, MF_POPUP, (UINT_PTR)hLabelMenu, T(L"文字标签样式", L"Label Format"));
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hTextMenu, T(L"文字与字体", L"Text & Font"));

    // 3. 外观美化
    HMENU hStyleMenu = CreatePopupMenu();
    HMENU hColorMenu = CreatePopupMenu();
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_WHITE ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_WHITE, T(L"纯白 (默认)", L"Pure White (Default)"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_BLACK ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_BLACK, T(L"墨黑 (适合浅色桌面)", L"Ink Black (For Light Wallpaper)"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_SILVER ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_SILVER, T(L"冰霜银灰", L"Frost Silver"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_CYAN ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_CYAN, T(L"赛博青", L"Cyber Cyan"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_GREEN ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_GREEN, T(L"极客荧光绿", L"Matrix Green"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_AMBER ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_AMBER, T(L"暖琥珀金", L"Warm Amber"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_PINK ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_PINK, T(L"樱花粉", L"Sakura Pink"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.colorTheme == THEME_VIOLET ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_VIOLET, T(L"幻夜紫", L"Soft Violet"));
    AppendMenuW(hColorMenu, MF_SEPARATOR, 0, NULL);
    if (g_cfg.colorTheme == THEME_CUSTOM) {
        AppendMenuW(hColorMenu, MF_STRING | MF_CHECKED, IDM_COLOR_CUSTOM, T(L"当前自定义颜色", L"Current Custom Color"));
    }
    AppendMenuW(hColorMenu, MF_STRING, IDM_COLOR_PICK, T(L"调色盘自定义颜色...", L"Custom Color Picker..."));
    AppendMenuW(hColorMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.perMetricColor ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_PERMETRIC, T(L"分项多彩模式", L"Per-Metric Colors"));
    AppendMenuW(hColorMenu, MF_STRING | (g_cfg.dynamicLoadColor ? MF_CHECKED : MF_UNCHECKED), IDM_COLOR_DYNLOAD, T(L"高负载自动变色 (≥70%黄 / ≥90%红)", L"Dynamic Load Alert Color"));

    HMENU hBgMenu = CreatePopupMenu();
    AppendMenuW(hBgMenu, MF_STRING | (g_cfg.bgStyle == BG_NONE ? MF_CHECKED : MF_UNCHECKED), IDM_BG_NONE, T(L"完全透明无底板 (默认)", L"Transparent (Default)"));
    AppendMenuW(hBgMenu, MF_STRING | (g_cfg.bgStyle == BG_DARK_PILL ? MF_CHECKED : MF_UNCHECKED), IDM_BG_DARK_PILL, T(L"暗色磨砂胶囊", L"Dark Frosted Pill"));
    AppendMenuW(hBgMenu, MF_STRING | (g_cfg.bgStyle == BG_LIGHT_PILL ? MF_CHECKED : MF_UNCHECKED), IDM_BG_LIGHT_PILL, T(L"浅色玻璃胶囊", L"Light Glass Pill"));
    AppendMenuW(hBgMenu, MF_STRING | (g_cfg.bgStyle == BG_DARK_BAR ? MF_CHECKED : MF_UNCHECKED), IDM_BG_DARK_BAR, T(L"极简圆角暗底条", L"Minimal Dark Card"));
    AppendMenuW(hBgMenu, MF_STRING | (g_cfg.bgStyle == BG_NEON_PILL ? MF_CHECKED : MF_UNCHECKED), IDM_BG_NEON_PILL, T(L"细描边霓虹胶囊", L"Neon Border Pill"));

    HMENU hShadowMenu = CreatePopupMenu();
    AppendMenuW(hShadowMenu, MF_STRING | (g_cfg.shadowStyle == SHADOW_SUBTLE ? MF_CHECKED : MF_UNCHECKED), IDM_SHADOW_SUBTLE, T(L"1px 锐利微阴影 (默认)", L"1px Subtle Shadow (Default)"));
    AppendMenuW(hShadowMenu, MF_STRING | (g_cfg.shadowStyle == SHADOW_STRONG ? MF_CHECKED : MF_UNCHECKED), IDM_SHADOW_STRONG, T(L"双层柔和强阴影", L"Soft Strong Shadow"));
    AppendMenuW(hShadowMenu, MF_STRING | (g_cfg.shadowStyle == SHADOW_OUTLINE ? MF_CHECKED : MF_UNCHECKED), IDM_SHADOW_OUTLINE, T(L"四向细描边 (高亮壁纸清晰)", L"4-Way Outline"));
    AppendMenuW(hShadowMenu, MF_STRING | (g_cfg.shadowStyle == SHADOW_NONE ? MF_CHECKED : MF_UNCHECKED), IDM_SHADOW_NONE, T(L"无阴影", L"None"));

    HMENU hOpacityMenu = CreatePopupMenu();
    AppendMenuW(hOpacityMenu, MF_STRING | (g_cfg.opacityPct == 100 ? MF_CHECKED : MF_UNCHECKED), IDM_OPACITY_100, T(L"100% (完全不透明, 默认)", L"100% (Opaque, Default)"));
    AppendMenuW(hOpacityMenu, MF_STRING | (g_cfg.opacityPct == 85 ? MF_CHECKED : MF_UNCHECKED), IDM_OPACITY_85, L"85%");
    AppendMenuW(hOpacityMenu, MF_STRING | (g_cfg.opacityPct == 70 ? MF_CHECKED : MF_UNCHECKED), IDM_OPACITY_70, L"70%");
    AppendMenuW(hOpacityMenu, MF_STRING | (g_cfg.opacityPct == 55 ? MF_CHECKED : MF_UNCHECKED), IDM_OPACITY_55, L"55%");
    AppendMenuW(hOpacityMenu, MF_STRING | (g_cfg.opacityPct == 40 ? MF_CHECKED : MF_UNCHECKED), IDM_OPACITY_40, L"40%");

    AppendMenuW(hStyleMenu, MF_POPUP, (UINT_PTR)hColorMenu, T(L"文字颜色", L"Text Color"));
    AppendMenuW(hStyleMenu, MF_POPUP, (UINT_PTR)hBgMenu, T(L"背景底板", L"Background"));
    AppendMenuW(hStyleMenu, MF_POPUP, (UINT_PTR)hShadowMenu, T(L"阴影与描边", L"Shadow & Outline"));
    AppendMenuW(hStyleMenu, MF_POPUP, (UINT_PTR)hOpacityMenu, T(L"整体不透明度", L"Opacity"));
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hStyleMenu, T(L"外观美化", L"Appearance"));

    // 4. 布局与位置
    HMENU hLayoutSubMenu = CreatePopupMenu();
    HMENU hArrangeMenu = CreatePopupMenu();
    AppendMenuW(hArrangeMenu, MF_STRING | (g_cfg.layoutMode == LAYOUT_HORIZONTAL ? MF_CHECKED : MF_UNCHECKED), IDM_LAYOUT_HORIZ, T(L"单行横排 (默认)", L"Horizontal (Default)"));
    AppendMenuW(hArrangeMenu, MF_STRING | (g_cfg.layoutMode == LAYOUT_TWO_ROWS ? MF_CHECKED : MF_UNCHECKED), IDM_LAYOUT_TWOROW, T(L"双行紧凑", L"Two Rows"));
    AppendMenuW(hArrangeMenu, MF_STRING | (g_cfg.layoutMode == LAYOUT_VERTICAL ? MF_CHECKED : MF_UNCHECKED), IDM_LAYOUT_VERT, T(L"竖排列表", L"Vertical List"));

    HMENU hSpacingMenu = CreatePopupMenu();
    AppendMenuW(hSpacingMenu, MF_STRING | (g_cfg.spacingMode == SPACING_COMPACT ? MF_CHECKED : MF_UNCHECKED), IDM_SPACING_COMPACT, T(L"紧凑", L"Compact"));
    AppendMenuW(hSpacingMenu, MF_STRING | (g_cfg.spacingMode == SPACING_STANDARD ? MF_CHECKED : MF_UNCHECKED), IDM_SPACING_STANDARD, T(L"标准 (默认)", L"Standard (Default)"));
    AppendMenuW(hSpacingMenu, MF_STRING | (g_cfg.spacingMode == SPACING_WIDE ? MF_CHECKED : MF_UNCHECKED), IDM_SPACING_WIDE, T(L"宽松", L"Wide"));

    HMENU hDockMenu = CreatePopupMenu();
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_TOP_LEFT ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_TL, T(L"左上角 (默认)", L"Top Left (Default)"));
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_TOP_CENTER ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_TC, T(L"顶部居中", L"Top Center"));
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_TOP_RIGHT ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_TR, T(L"右上角", L"Top Right"));
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_BOTTOM_LEFT ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_BL, T(L"左下角", L"Bottom Left"));
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_BOTTOM_CENTER ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_BC, T(L"底部居中", L"Bottom Center"));
    AppendMenuW(hDockMenu, MF_STRING | (g_cfg.dockPreset == DOCK_BOTTOM_RIGHT ? MF_CHECKED : MF_UNCHECKED), IDM_DOCK_BR, T(L"右下角", L"Bottom Right"));

    AppendMenuW(hLayoutSubMenu, MF_POPUP, (UINT_PTR)hArrangeMenu, T(L"排列方式", L"Arrangement"));
    AppendMenuW(hLayoutSubMenu, MF_POPUP, (UINT_PTR)hSpacingMenu, T(L"列间距", L"Spacing"));
    AppendMenuW(hLayoutSubMenu, MF_POPUP, (UINT_PTR)hDockMenu, T(L"停靠角落", L"Dock Position"));
    AppendMenuW(hLayoutSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hLayoutSubMenu, MF_STRING | (g_dragUnlocked ? MF_CHECKED : MF_UNCHECKED), IDM_TOGGLE_DRAG,
        g_dragUnlocked ? T(L"锁定鼠标穿透 (完成拖动)", L"Lock Click-Through (Finish Drag)")
                       : T(L"解锁拖动位置", L"Unlock Drag to Move"));
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hLayoutSubMenu, T(L"布局与位置", L"Layout & Position"));

    // 5. 刷新频率
    HMENU hRefreshMenu = CreatePopupMenu();
    AppendMenuW(hRefreshMenu, MF_STRING | (g_cfg.refreshInterval == 250 ? MF_CHECKED : MF_UNCHECKED), IDM_REFRESH_250, T(L"0.25 秒 (极速)", L"0.25s (Ultra Fast)"));
    AppendMenuW(hRefreshMenu, MF_STRING | (g_cfg.refreshInterval == 500 ? MF_CHECKED : MF_UNCHECKED), IDM_REFRESH_500, T(L"0.5 秒", L"0.5s"));
    AppendMenuW(hRefreshMenu, MF_STRING | (g_cfg.refreshInterval == 1000 ? MF_CHECKED : MF_UNCHECKED), IDM_REFRESH_1000, T(L"1.0 秒 (推荐)", L"1.0s (Recommended)"));
    AppendMenuW(hRefreshMenu, MF_STRING | (g_cfg.refreshInterval == 2000 ? MF_CHECKED : MF_UNCHECKED), IDM_REFRESH_2000, T(L"2.0 秒 (低功耗)", L"2.0s (Low Power)"));
    AppendMenuW(hRefreshMenu, MF_STRING | (g_cfg.refreshInterval == 3000 ? MF_CHECKED : MF_UNCHECKED), IDM_REFRESH_3000, T(L"3.0 秒 (省电)", L"3.0s (Eco)"));
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hRefreshMenu, T(L"刷新频率", L"Refresh Rate"));

    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

    // 6. 行为与语言
    AppendMenuW(hMenu, MF_STRING | (g_cfg.hideOnFullscreen ? MF_CHECKED : MF_UNCHECKED), IDM_HIDE_FULLSCREEN, T(L"全屏应用自动隐藏", L"Auto-Hide in Fullscreen"));
    AppendMenuW(hMenu, MF_STRING | (IsAutoRunEnabled() ? MF_CHECKED : MF_UNCHECKED), IDM_AUTORUN, T(L"开机自启动", L"Run at Startup"));

    HMENU hLangMenu = CreatePopupMenu();
    AppendMenuW(hLangMenu, MF_STRING | (!g_cfg.langEnglish ? MF_CHECKED : MF_UNCHECKED), IDM_LANG_ZH, L"简体中文");
    AppendMenuW(hLangMenu, MF_STRING | (g_cfg.langEnglish ? MF_CHECKED : MF_UNCHECKED), IDM_LANG_EN, L"English");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hLangMenu, T(L"菜单语言 / Language", L"Language / 语言"));

    AppendMenuW(hMenu, MF_STRING, IDM_TOGGLE_SHOW, g_isShown ? T(L"隐藏监控", L"Hide Overlay") : T(L"显示监控", L"Show Overlay"));
    AppendMenuW(hMenu, MF_STRING, IDM_RESET_DEFAULT, T(L"恢复默认设置", L"Reset to Defaults"));
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_EXIT, T(L"退出程序", L"Exit"));

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_RIGHTALIGN, pt.x, pt.y, 0, hWnd, NULL);
    PostMessageW(hWnd, WM_NULL, 0, 0);

    DestroyMenu(hMenu);
    TrimWorkingSet();
}

// -------------------------------------------------------------
// 窗口消息过程
// -------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        SetTimer(hWnd, ID_TIMER_REFRESH, g_cfg.refreshInterval, NULL);
        SetTimer(hWnd, ID_TIMER_TRIM, 4000, NULL);
        break;

    case WM_TIMER:
        if (wParam == ID_TIMER_REFRESH) {
            RenderOverlay();
        } else if (wParam == ID_TIMER_TRIM) {
            KillTimer(hWnd, ID_TIMER_TRIM);
            TrimWorkingSet();
        }
        break;

    case WM_DPICHANGED: {
        UINT newDpi = HIWORD(wParam);
        g_dpiScale = (newDpi > 0 ? newDpi : 96) / 96.0f;
        MarkVisualDirty();
        RenderOverlay();
        break;
    }

    case WM_DISPLAYCHANGE:
        MarkVisualDirty();
        RenderOverlay();
        break;

    case WM_LBUTTONDOWN:
        if (g_dragUnlocked) {
            g_isDragging = true;
            SetCapture(hWnd);
            GetCursorPos(&g_dragStartCursor);
            RECT rc;
            GetWindowRect(hWnd, &rc);
            g_dragStartWnd.x = rc.left;
            g_dragStartWnd.y = rc.top;
        }
        break;

    case WM_MOUSEMOVE:
        if (g_isDragging && g_dragUnlocked) {
            POINT pt;
            GetCursorPos(&pt);
            g_posX = g_dragStartWnd.x + (pt.x - g_dragStartCursor.x);
            g_posY = g_dragStartWnd.y + (pt.y - g_dragStartCursor.y);
            SetWindowPos(hWnd, HWND_TOPMOST, g_posX, g_posY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
        }
        break;

    case WM_LBUTTONUP:
        if (g_isDragging) {
            g_isDragging = false;
            ReleaseCapture();
            g_cfg.dockPreset = DOCK_CUSTOM;
            g_cfg.customPosX = g_posX;
            g_cfg.customPosY = g_posY;
            g_lastRenderedPosX = g_posX;
            g_lastRenderedPosY = g_posY;
            SaveConfig();
        }
        break;

    case WM_RBUTTONUP:
        if (g_dragUnlocked) {
            ShowTrayMenu(hWnd);
        }
        break;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP) {
            ShowTrayMenu(hWnd);
        }
        break;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        bool needSave = false;
        bool needFormat = false;
        bool needRender = false;

        // 刷新频率
        if (id == IDM_REFRESH_250 || id == IDM_REFRESH_500 || id == IDM_REFRESH_1000 ||
            id == IDM_REFRESH_2000 || id == IDM_REFRESH_3000) {
            if (id == IDM_REFRESH_250) g_cfg.refreshInterval = 250;
            else if (id == IDM_REFRESH_500) g_cfg.refreshInterval = 500;
            else if (id == IDM_REFRESH_1000) g_cfg.refreshInterval = 1000;
            else if (id == IDM_REFRESH_2000) g_cfg.refreshInterval = 2000;
            else g_cfg.refreshInterval = 3000;
            SetTimer(hWnd, ID_TIMER_REFRESH, g_cfg.refreshInterval, NULL);
            needSave = true;
            needRender = true;
        }
        // 显示项目
        else if (id == IDM_SHOW_CPU)       { g_cfg.showCpu = !g_cfg.showCpu; needSave = needRender = true; }
        else if (id == IDM_SHOW_GPU)       { g_cfg.showGpu = !g_cfg.showGpu; needSave = needRender = true; }
        else if (id == IDM_SHOW_GPU_TEMP)  { g_cfg.showGpuTemp = !g_cfg.showGpuTemp; needSave = needRender = true; }
        else if (id == IDM_SHOW_GPU_VRAM)  { g_cfg.showGpuVram = !g_cfg.showGpuVram; needSave = needRender = true; }
        else if (id == IDM_SHOW_GPU_POWER) { g_cfg.showGpuPower = !g_cfg.showGpuPower; needSave = needRender = true; }
        else if (id == IDM_SHOW_RAM)       { g_cfg.showRam = !g_cfg.showRam; needSave = needRender = true; }
        else if (id == IDM_SHOW_RAM_GB)    { g_cfg.showRamGB = !g_cfg.showRamGB; needSave = needRender = true; }
        else if (id == IDM_SHOW_NET) {
            g_cfg.showNet = !g_cfg.showNet;
            if (g_cfg.showNet) ResetNetBaseline();
            needSave = needRender = true;
        }
        else if (id == IDM_SHOW_CLOCK)     { g_cfg.showClock = !g_cfg.showClock; needSave = needRender = true; }
        else if (id == IDM_SHOW_TEMP_UNIT) { g_cfg.showTempUnit = !g_cfg.showTempUnit; needSave = needRender = true; }
        // 字体大小
        else if (id >= IDM_FONT_10 && id <= IDM_FONT_22) {
            const float sizes[8] = { 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 18.0f, 22.0f };
            g_cfg.fontSize = sizes[id - IDM_FONT_10];
            needSave = needFormat = needRender = true;
        }
        // 字体粗细
        else if (id == IDM_WEIGHT_LIGHT)    { g_cfg.fontWeight = DWRITE_FONT_WEIGHT_LIGHT; needSave = needFormat = needRender = true; }
        else if (id == IDM_WEIGHT_NORMAL)   { g_cfg.fontWeight = DWRITE_FONT_WEIGHT_NORMAL; needSave = needFormat = needRender = true; }
        else if (id == IDM_WEIGHT_SEMIBOLD) { g_cfg.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD; needSave = needFormat = needRender = true; }
        else if (id == IDM_WEIGHT_BOLD)     { g_cfg.fontWeight = DWRITE_FONT_WEIGHT_BOLD; needSave = needFormat = needRender = true; }
        // 字体家族
        else if (id >= IDM_FAMILY_BASE && id < IDM_FAMILY_BASE + IDM_FAMILY_COUNT) {
            g_cfg.fontFamily = kFontPresets[id - IDM_FAMILY_BASE];
            needSave = needFormat = needRender = true;
        }
        else if (id == IDM_FAMILY_PICK) {
            PickSystemFont();
        }
        // 标签风格
        else if (id >= IDM_LABEL_STANDARD && id <= IDM_LABEL_BRACKETS) {
            g_cfg.labelStyle = id - IDM_LABEL_STANDARD;
            needSave = needRender = true;
        }
        // 配色
        else if (id >= IDM_COLOR_WHITE && id <= IDM_COLOR_CUSTOM) {
            g_cfg.colorTheme = id - IDM_COLOR_WHITE;
            needSave = needRender = true;
        }
        else if (id == IDM_COLOR_PICK) {
            PickCustomColor();
        }
        else if (id == IDM_COLOR_PERMETRIC) {
            g_cfg.perMetricColor = !g_cfg.perMetricColor;
            needSave = needRender = true;
        }
        else if (id == IDM_COLOR_DYNLOAD) {
            g_cfg.dynamicLoadColor = !g_cfg.dynamicLoadColor;
            needSave = needRender = true;
        }
        // 背景底板
        else if (id >= IDM_BG_NONE && id <= IDM_BG_NEON_PILL) {
            g_cfg.bgStyle = id - IDM_BG_NONE;
            needSave = needRender = true;
        }
        // 阴影
        else if (id >= IDM_SHADOW_SUBTLE && id <= IDM_SHADOW_NONE) {
            g_cfg.shadowStyle = id - IDM_SHADOW_SUBTLE;
            needSave = needRender = true;
        }
        // 不透明度
        else if (id >= IDM_OPACITY_100 && id <= IDM_OPACITY_40) {
            const int ops[5] = { 100, 85, 70, 55, 40 };
            g_cfg.opacityPct = ops[id - IDM_OPACITY_100];
            needSave = needRender = true;
        }
        // 布局与位置
        else if (id >= IDM_LAYOUT_HORIZ && id <= IDM_LAYOUT_VERT) {
            g_cfg.layoutMode = id - IDM_LAYOUT_HORIZ;
            needSave = needRender = true;
        }
        else if (id >= IDM_SPACING_COMPACT && id <= IDM_SPACING_WIDE) {
            g_cfg.spacingMode = id - IDM_SPACING_COMPACT;
            needSave = needRender = true;
        }
        else if (id >= IDM_DOCK_TL && id <= IDM_DOCK_BR) {
            g_cfg.dockPreset = id - IDM_DOCK_TL;
            needSave = needRender = true;
        }
        else if (id == IDM_TOGGLE_DRAG) {
            SetDragUnlocked(!g_dragUnlocked);
        }
        // 行为与系统
        else if (id == IDM_HIDE_FULLSCREEN) {
            g_cfg.hideOnFullscreen = !g_cfg.hideOnFullscreen;
            needSave = needRender = true;
        }
        else if (id == IDM_AUTORUN) {
            SetAutoRun(!IsAutoRunEnabled());
        }
        else if (id == IDM_LANG_ZH) {
            g_cfg.langEnglish = false;
            UpdateTrayTooltip();
            needSave = true;
        }
        else if (id == IDM_LANG_EN) {
            g_cfg.langEnglish = true;
            UpdateTrayTooltip();
            needSave = true;
        }
        else if (id == IDM_TOGGLE_SHOW) {
            g_isShown = !g_isShown;
            if (!g_isShown) {
                g_hiddenByFullscreen = false;
                ShowWindow(hWnd, SW_HIDE);
            } else {
                ShowWindow(hWnd, SW_SHOWNOACTIVATE);
                MarkVisualDirty();
                RenderOverlay();
            }
        }
        else if (id == IDM_RESET_DEFAULT) {
            InitDefaultConfig(g_cfg);
            if (g_dragUnlocked) SetDragUnlocked(false);
            ResetNetBaseline();
            SetTimer(hWnd, ID_TIMER_REFRESH, g_cfg.refreshInterval, NULL);
            UpdateTrayTooltip();
            needSave = needFormat = needRender = true;
        }
        else if (id == IDM_EXIT) {
            DestroyWindow(hWnd);
        }

        if (needFormat) UpdateTextFormat();
        if (needSave) SaveConfig();
        if (needRender) {
            MarkVisualDirty();
            RenderOverlay();
        }
        break;
    }

    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER_REFRESH);
        KillTimer(hWnd, ID_TIMER_TRIM);
        Shell_NotifyIconW(NIM_DELETE, &g_nid);
        ReleaseBackbuffer();
        if (g_nvmlShutdown) {
            g_nvmlShutdown();
        }
        if (g_hNvml) {
            FreeLibrary(g_hNvml);
            g_hNvml = NULL;
        }
        if (g_pBrush) g_pBrush->Release();
        if (g_pDCRenderTarget) g_pDCRenderTarget->Release();
        if (g_pTextFormat) g_pTextFormat->Release();
        if (g_pDWriteFactory) g_pDWriteFactory->Release();
        if (g_pD2DFactory) g_pD2DFactory->Release();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// -------------------------------------------------------------
// 入口函数
// -------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    g_hInstance = hInstance;

    EnablePerMonitorDpi();

    HANDLE hMutex = CreateMutexW(NULL, TRUE, L"Monitor_SingleInstance");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (hMutex) CloseHandle(hMutex);
        return 0;
    }

    LoadConfig();
    g_dpiScale = QueryDpiScale(NULL);

    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory))) {
        return 0;
    }
    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&g_pDWriteFactory))) {
        return 0;
    }

    UpdateTextFormat();

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        96.0f, 96.0f,
        D2D1_RENDER_TARGET_USAGE_NONE,
        D2D1_FEATURE_LEVEL_DEFAULT
    );

    if (FAILED(g_pD2DFactory->CreateDCRenderTarget(&props, &g_pDCRenderTarget))) {
        return 0;
    }

    g_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &g_pBrush);

    HICON hAppIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(1));
    if (!hAppIcon) {
        hAppIcon = LoadIconW(NULL, IDI_APPLICATION);
    }

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MonitorOverlayClass";
    wc.hIcon = hAppIcon;
    wc.hIconSm = hAppIcon;
    wc.hCursor = LoadCursor(NULL, IDC_SIZEALL);
    RegisterClassExW(&wc);

    g_hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        L"MonitorOverlayClass",
        L"Monitor",
        WS_POPUP,
        g_posX, g_posY, g_winPhysW, g_winPhysH,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hWnd) {
        return 0;
    }

    g_dpiScale = QueryDpiScale(g_hWnd);

    memset(&g_nid, 0, sizeof(g_nid));
    g_nid.cbSize = sizeof(NOTIFYICONDATAW);
    g_nid.hWnd = g_hWnd;
    g_nid.uID = 1;
    g_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    g_nid.uCallbackMessage = WM_TRAYICON;
    g_nid.hIcon = hAppIcon;
    wcscpy_s(g_nid.szTip, T(L"Monitor 硬件监控 (右键打开设置)", L"Monitor (Right-click for settings)"));
    Shell_NotifyIconW(NIM_ADD, &g_nid);

    ShowWindow(g_hWnd, SW_SHOWNOACTIVATE);
    RenderOverlay();

    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }
    return 0;
}
