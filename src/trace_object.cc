#include "trace_object.h"
#include "util.h"
#include <fmt/core.h>

bool isIIDSupported(REFIID riid) {
  if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfThreadMgr)) {
    return true;
  }
  return false;
}

// IUnknown
STDMETHODIMP TraceObject::QueryInterface(REFIID riid, void **ppvObject) {
  if (ppvObject == nullptr) {
    return E_INVALIDARG;
  }
  *ppvObject = nullptr;

  auto hr = tsfObject_->QueryInterface(riid, ppvObject);
  auto logContent =
      fmt::format("TSFSPY: [T]{}::QueryInterface({}, _)->{:x}", interfaceType_, getIIDName(riid), (long)hr);
  if (hr == S_OK && isIIDSupported(riid)) {
    if (isIIDSupported(riid)) {
      *ppvObject = new TraceObject((IUnknown *)*ppvObject, proxyObject_, getIIDName(riid));
    } else {
      logContent += "!!UNSUPPORTED_INTERFACE";
    }
  }
  OutputDebugStringA(logContent.c_str());

  return hr;
}

STDMETHODIMP_(ULONG) TraceObject::AddRef() { return tsfObject_->AddRef(); }

STDMETHODIMP_(ULONG) TraceObject::Release() { return tsfObject_->Release(); }

// ITfThreadMgr
STDMETHODIMP TraceObject::Activate(TfClientId *ptid) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::Activate({:x})", (DWORD)*ptid);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->Activate(ptid);
}
STDMETHODIMP TraceObject::Deactivate(void) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::Deactivate()");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->Deactivate();
}
STDMETHODIMP TraceObject::CreateDocumentMgr(ITfDocumentMgr **ppdim) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::CreateDocumentMgr(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->CreateDocumentMgr(ppdim);
}
STDMETHODIMP TraceObject::EnumDocumentMgrs(IEnumTfDocumentMgrs **ppEnum) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::EnumDocumentMgrs(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->EnumDocumentMgrs(ppEnum);
}
STDMETHODIMP TraceObject::GetFocus(ITfDocumentMgr **ppdimFocus) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::GetFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->GetFocus(ppdimFocus);
}
STDMETHODIMP TraceObject::SetFocus(ITfDocumentMgr *pdimFocus) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::SetFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->SetFocus(pdimFocus);
}
STDMETHODIMP TraceObject::AssociateFocus(HWND hwnd, ITfDocumentMgr *pdimNew, ITfDocumentMgr **ppdimPrev) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::AssociateFocus({:x}, _, _)", (void *)hwnd);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->AssociateFocus(hwnd, pdimNew, ppdimPrev);
}
STDMETHODIMP TraceObject::IsThreadFocus(BOOL *pfThreadFocus) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::IsThreadFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->IsThreadFocus(pfThreadFocus);
}
STDMETHODIMP TraceObject::GetFunctionProvider(REFCLSID clsid, ITfFunctionProvider **ppFuncProv) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::GetFunctionProvider({}, _)", guidToString(clsid));
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->GetFunctionProvider(clsid, ppFuncProv);
}
STDMETHODIMP TraceObject::EnumFunctionProviders(IEnumTfFunctionProviders **ppEnum) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::EnumFunctionProviders(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->EnumFunctionProviders(ppEnum);
}
STDMETHODIMP TraceObject::GetGlobalCompartment(ITfCompartmentMgr **ppCompMgr) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgr::GetGlobalCompartment(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)tsfObject_)->GetGlobalCompartment(ppCompMgr);
}
