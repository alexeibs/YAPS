#include "winkbhook.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_NAME L"YAPS-07c3a94e-700e-4cf4-adcd-8e7de7b8058e"

static HINSTANCE g_dllInstance;
static HANDLE g_clipboardPasteEvent;

static HWND g_messageReceiver;
static HHOOK g_hook;
static HANDLE g_listenerThread;
bool g_isHookActive;

WINKBHOOK_API BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID /*lpvReserved*/)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        g_dllInstance = hinstDLL;
        g_isHookActive = false;
        g_clipboardPasteEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_NAME);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        if (g_isHookActive)
            removeKeyBoardHook();
        CloseHandle(g_clipboardPasteEvent);
    }
    return TRUE;
}

LRESULT CALLBACK winKeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
    // 0x56 - virtual key code of 'V'
    // 30 bit of lParam should be 0, this means that the key was up before message sent
    // 31 bit of lParam should be 0, this means that the key is being pressed
    if (code == HC_ACTION && wParam == 0x56 && (lParam & 0xC0000000) == 0) {
        // check whether Control is pressed or not
        if (GetKeyState(VK_CONTROL) & 0x8000)
            SetEvent(g_clipboardPasteEvent);
    }
    return CallNextHookEx(0, code, wParam, lParam);
}

static DWORD WINAPI listenerThread(LPVOID)
{
    for(;;) {
        if (WaitForSingleObject(g_clipboardPasteEvent, INFINITE) == WAIT_OBJECT_0)
            SendMessage(g_messageReceiver, WM_CLIPBOARD_PASTE, 0, 0);
    }
    return 0;
}

void setupKeyBoardHook(HWND messageReceiver)
{
    g_messageReceiver = messageReceiver;
    g_listenerThread = CreateThread(NULL, 0, &listenerThread, NULL, 0, NULL);
    g_hook = SetWindowsHookEx(WH_KEYBOARD, &winKeyboardHook, g_dllInstance, 0);
    g_isHookActive = true;
}

void removeKeyBoardHook()
{
    g_isHookActive = false;
    UnhookWindowsHookEx(g_hook);
    TerminateThread(g_listenerThread, 0);
    CloseHandle(g_listenerThread);
}

#ifdef __cplusplus
}
#endif
