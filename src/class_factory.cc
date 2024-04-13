#include "dllglobals.h"
#include "class_factory.h"
#include "proxy_object.h"
#include "util.h"
#include <fmt/core.h>

STDAPI ClassFactory::QueryInterface(REFIID riid, void **ppv) {
  if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown)) {
    *ppv = static_cast<IClassFactory *>(this);
    AddRef();
    return NOERROR;
  }
  *ppv = NULL;
  return E_NOINTERFACE;
}

STDAPI_(ULONG) ClassFactory::AddRef() {
  DllAddRef();
  return g_cDllRef + 1;
}

STDAPI_(ULONG) ClassFactory::Release() {
  DllRelease();
  return g_cDllRef + 1;
}

STDAPI ClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv) {
  if (ppv == nullptr) {
    return E_INVALIDARG;
  }
  *ppv = nullptr;
  if (pUnkOuter != nullptr) {
    return CLASS_E_NOAGGREGATION;
  }

  void *pv = nullptr;
  auto hr = tsfClassFactory_->CreateInstance(pUnkOuter, riid, &pv);
  if (hr != S_OK) {
    return hr;
  }

  if (g_enabled) {
    auto logContent = fmt::format("TSFSPY: ClassFactory::CreateInstance(_, {}, _)", getIIDName(riid));
    OutputDebugStringA(logContent.c_str());
    auto *proxyObj = new ProxyObject(pv);
    hr = proxyObj->QueryInterface(riid, ppv);
    proxyObj->Release();
    *ppv = proxyObj;
    return S_OK;
  }

  *ppv = pv;
  return S_OK;
}

STDAPI ClassFactory::LockServer(BOOL fLock) {
  if (fLock) {
    DllAddRef();
  } else {
    DllRelease();
  }
  return S_OK;
}
