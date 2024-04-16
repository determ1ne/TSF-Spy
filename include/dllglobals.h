#pragma once
#include <windows.h>

extern HINSTANCE g_hInstance;
extern CRITICAL_SECTION g_cs;
extern LONG g_cDllRef;
extern bool g_enabled;
// extern ProxyObject *g_proxyObject;

void DllAddRef();
void DllRelease();
