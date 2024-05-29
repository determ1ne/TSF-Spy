#include "ditto.h"

STDMETHODIMP DTfThreadMgr::Activate(
    /* [out] */ TfClientId *ptid) {
  auto hr = ((ITfThreadMgr *)object_)->Activate(ptid);
  log(type_, "ITfThreadMgr", "Activate", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::Deactivate() {
  auto hr = ((ITfThreadMgr *)object_)->Deactivate();
  log(type_, "ITfThreadMgr", "Deactivate", "()->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::CreateDocumentMgr(
    /* [out] */ ITfDocumentMgr **ppdim) {

  auto hr = ((ITfThreadMgr *)object_)->CreateDocumentMgr(ppdim);
  log(type_, "ITfThreadMgr", "CreateDocumentMgr", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::EnumDocumentMgrs(
    /* [out] */ IEnumTfDocumentMgrs **ppEnum) {
  auto hr = ((ITfThreadMgr *)object_)->EnumDocumentMgrs(ppEnum);
  log(type_, "ITfThreadMgr", "EnumDocumentMgrs", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetFocus(
    /* [out] */ ITfDocumentMgr **ppdimFocus) {
  auto hr = ((ITfThreadMgr *)object_)->GetFocus(ppdimFocus);
  log(type_, "ITfThreadMgr", "GetFocus", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::SetFocus(
    /* [in] */ ITfDocumentMgr *pdimFocus) {
  auto hr = ((ITfThreadMgr *)object_)->SetFocus(pdimFocus);
  log(type_, "ITfThreadMgr", "SetFocus", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::AssociateFocus(
    /* [in] */ HWND hwnd,
    /* [unique][in] */ ITfDocumentMgr *pdimNew,
    /* [out] */ ITfDocumentMgr **ppdimPrev) {
  auto hr = ((ITfThreadMgr *)object_)->AssociateFocus(hwnd, pdimNew, ppdimPrev);
  log(type_, "ITfThreadMgr", "AssociateFocus", "(0x{:x}, _, _)->0x{:x}", fmt::ptr(hwnd), ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::IsThreadFocus(
    /* [out] */ BOOL *pfThreadFocus) {
  auto hr = ((ITfThreadMgr *)object_)->IsThreadFocus(pfThreadFocus);
  log(type_, "ITfThreadMgr", "IsThreadFocus", "(_)->0x{:x}", *pfThreadFocus, ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetFunctionProvider(
    /* [in] */ REFCLSID clsid,
    /* [out] */ ITfFunctionProvider **ppFuncProv) {
  auto hr = ((ITfThreadMgr *)object_)->GetFunctionProvider(clsid, ppFuncProv);
  log(type_, "ITfThreadMgr", "GetFunctionProvider", "({}, _)->0x{:x}", getCLSIDName(clsid), ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::EnumFunctionProviders(
    /* [out] */ IEnumTfFunctionProviders **ppEnum) {
  auto hr = ((ITfThreadMgr *)object_)->EnumFunctionProviders(ppEnum);
  log(type_, "ITfThreadMgr", "EnumFunctionProviders", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetGlobalCompartment(
    /* [out] */ ITfCompartmentMgr **ppCompMgr) {
  auto hr = ((ITfThreadMgr *)object_)->GetGlobalCompartment(ppCompMgr);
  log(type_, "ITfThreadMgr", "GetGlobalCompartment", "(_)->0x{:x}", ul(hr));
  return hr;
}
