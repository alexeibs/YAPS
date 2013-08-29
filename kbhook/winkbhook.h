#ifndef WINKBHOOK_H
#define WINKBHOOK_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WINKBHOOK_EXPORTS
#define WINKBHOOK_API __declspec(dllexport)
#else
#define WINKBHOOK_API __declspec(dllimport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define WM_CLIPBOARD_PASTE (WM_USER+1)

WINKBHOOK_API void setupKeyBoardHook(HWND messageReceiver);
WINKBHOOK_API void removeKeyBoardHook();

#ifdef __cplusplus
}
#endif

#endif // WINKBHOOK_H