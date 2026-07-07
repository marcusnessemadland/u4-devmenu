#include "minhook/minhook.h"

#include <windows.h>
#include <fstream>
#include <cstdio>

typedef __int64 (__fastcall *t_debugDrawPass)(__int64 _ctx, __int64 _a2, double _xmm2, double _xmm3);
typedef void    (__fastcall *t_menuInputHandler)(__int64 _menuObj);

static const uintptr_t kOffsetDebugDrawPass   = 0x157C690;
static const uintptr_t kOffsetMenuInputHandler = 0x1558420;
static const uintptr_t kOffsetMenuObjPtr       = 0x3E362B0;

static const char *kLogFileName = "u4_debugmenu_log.txt";

static uintptr_t          g_base;
static t_debugDrawPass    g_origDebugDrawPass;
static t_menuInputHandler g_menuInputHandler;

static void log(const char *_msg)
{
    std::ofstream file(kLogFileName, std::ios::app);
    file << _msg << "\n";
}

static void logAndPopup(const char *_msg)
{
    log(_msg);
    MessageBoxA(nullptr, _msg, "Debug Menu Mod - Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
}

static __int64 __fastcall hkDebugDrawPass(__int64 _ctx, __int64 _a2, double _xmm2, double _xmm3)
{
    __int64 menuObj = *(__int64 *)(g_base + kOffsetMenuObjPtr);

    static bool s_logged = false;
    if (!s_logged)
    {
        char buf[128];
        sprintf_s(buf, "hkDebugDrawPass fired, menuObj = 0x%llX", (unsigned long long)menuObj);
        log(buf);
        s_logged = true;
    }

    if (menuObj)
    {
        g_menuInputHandler(menuObj);
    }

    return g_origDebugDrawPass(_ctx, _a2, _xmm2, _xmm3);
}

static bool installHooks()
{
    g_menuInputHandler = (t_menuInputHandler)(g_base + kOffsetMenuInputHandler);

    if (MH_Initialize() != MH_OK)
    {
        logAndPopup("MH_Initialize FAILED");
        return false;
    }
    log("MH_Initialize OK");

    MH_STATUS hookStatus = MH_CreateHook(
        (void *)(g_base + kOffsetDebugDrawPass),
        &hkDebugDrawPass,
        reinterpret_cast<void **>(&g_origDebugDrawPass));

    if (hookStatus != MH_OK)
    {
        char msg[128];
        sprintf_s(msg, "MH_CreateHook FAILED, status=%d", hookStatus);
        logAndPopup(msg);
        return false;
    }
    log("MH_CreateHook OK");

    MH_STATUS enableStatus = MH_EnableHook(MH_ALL_HOOKS);
    if (enableStatus != MH_OK)
    {
        char msg[128];
        sprintf_s(msg, "MH_EnableHook FAILED, status=%d", enableStatus);
        logAndPopup(msg);
        return false;
    }
    log("MH_EnableHook OK");

    return true;
}

static DWORD WINAPI mainThread(LPVOID _param)
{
    g_base = (uintptr_t)GetModuleHandle(nullptr);

    char msg[128];
    sprintf_s(msg, "Base address: 0x%llX", (unsigned long long)g_base);
    log(msg);

    installHooks();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE _hModule, DWORD _reason, LPVOID _reserved)
{
    if (_reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(_hModule);
        CreateThread(nullptr, 0, mainThread, nullptr, 0, nullptr);
    }

    return TRUE;
}