#include <fstream>
#include <string>
#include <vector>
#include "class_factory.h"
#include "dllglobals.h"
#include "nlohmann/json.hpp"
#include "util.h"

#define _CRT_SECURE_NO_WARNINGS

using json = nlohmann::json;
using DllGetClassObjectType = HRESULT (__stdcall *)(REFCLSID rclsid, REFIID riid, LPVOID *ppv);

const wchar_t c_overrideText[] = L".override";

HINSTANCE g_hInstance;
HINSTANCE g_hTsfDll;
CRITICAL_SECTION g_cs;
LONG g_cDllRef{0};
ClassFactory *g_classFactory{nullptr};
DllGetClassObjectType g_tsfGetClassObject{nullptr};
bool g_enabled{false};

void DllAddRef() { InterlockedIncrement(&g_cDllRef); }

void DllRelease() { InterlockedDecrement(&g_cDllRef); }

STDAPI DllCanUnloadNow() {
  if (g_cDllRef >= 0) {
    return S_FALSE;
  }
  return S_OK;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv) {
  OutputDebugStringA("WHAT???");
  bool failed = false;
  if (g_classFactory == nullptr) {
    EnterCriticalSection(&g_cs);
    if (g_classFactory == nullptr) {
      if (g_tsfGetClassObject == nullptr) {
        OutputDebugStringA("TSFSPY: TSF GetClassObject not found");
        failed = true;
      } else {
        LPVOID pv = nullptr; 
        auto tsfClassFactory = g_tsfGetClassObject(rclsid, riid, &pv);
        if (tsfClassFactory != S_OK) {
          OutputDebugStringA("TSFSPY: TSF GetClassObject failed");
          failed = true;
        } else {
          g_classFactory = new ClassFactory(reinterpret_cast<IClassFactory *>(pv));
        }
      }
    }
    LeaveCriticalSection(&g_cs);

    return failed ? E_FAIL : S_OK;
  }

  if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown)) {
    *ppv = g_classFactory;
    DllAddRef();
    return S_OK;
  }
  *ppv = NULL;
  return CLASS_E_CLASSNOTAVAILABLE;
}

__declspec(dllexport)
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID pvReserved) {
  switch (dwReason) {
  case DLL_PROCESS_ATTACH: {
    g_hInstance = hInstance;
    if (!InitializeCriticalSectionAndSpinCount(&g_cs, 0)) {
      return FALSE;
    }

    // Load original dll
    wchar_t buffer[4096];
    char *pBuffer = reinterpret_cast<char *>(buffer);
    auto fileNameSize = GetModuleFileNameW(hInstance, buffer, ARRAYSIZE(buffer) - 1);
    if (fileNameSize <= 0 || fileNameSize + ARRAYSIZE(c_overrideText) > ARRAYSIZE(buffer)) {
      OutputDebugStringW(L"TSFSPY: file path too long");
      return false;
    }
    for (int i = 0; i < ARRAYSIZE(c_overrideText); ++i) {
      buffer[fileNameSize + i] = c_overrideText[i];
    }
    g_hTsfDll = LoadLibraryW(buffer);
    if (!g_hInstance) {
      OutputDebugStringW(L"TSFSPY: failed to load original dll");
      return false;
    }
    g_tsfGetClassObject = reinterpret_cast<DllGetClassObjectType>(GetProcAddress(g_hTsfDll, "DllGetClassObject"));
    if (!g_tsfGetClassObject) {
      OutputDebugStringW(L"TSFSPY: DllGetClassObject entry point not found");
      return false;
    }

    // Get current host process name
    auto exePathSize = GetModuleFileNameA(NULL, pBuffer, ARRAYSIZE(buffer) * sizeof(wchar_t) / sizeof(char) - 1);
    if (exePathSize <= 0) {
      OutputDebugStringW(L"TSFSPY: failed to get exe path");
      return true;
    }
    auto exePath = std::string(pBuffer, exePathSize+1);

    // Load config path
    std::ifstream configFile("C:/tsfspy.json");
    if (!configFile.is_open()) {
      OutputDebugStringW(L"TSFSPY: failed to open config file");
      return true;
    }
    json data = json::parse(configFile);

    try {
      std::vector<std::string> filter = data.at("filter");
      for (auto &str : filter) {
        if (exePath.find(str) != std::string::npos) {
          g_enabled = true;
          break;
        }
      }
    } catch (json::out_of_range &e) {
      OutputDebugStringW(L"TSFSPY: filter not found in config file");
      return true;
    }

    if (g_enabled) {
      OutputDebugStringW(L"TSFSPY: TSF Spy loaded");
    }
    break;
  }
  case DLL_PROCESS_DETACH:
    DeleteCriticalSection(&g_cs);
    break;
  }
  return TRUE;
}