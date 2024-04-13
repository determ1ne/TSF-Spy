#include <windows.h>

extern HINSTANCE g_hInstance;
extern CRITICAL_SECTION g_cs;
extern LONG g_cDllRef;
extern bool g_enabled;

void DllAddRef();
void DllRelease();
