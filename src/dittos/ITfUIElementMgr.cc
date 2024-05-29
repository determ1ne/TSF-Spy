#include "ditto.h"

STDMETHODIMP DTfUIElementMgr::BeginUIElement(
    /* [in] */ ITfUIElement *pElement,
    /* [out][in] */ BOOL *pbShow,
    /* [out] */ DWORD *pdwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->BeginUIElement(pElement, pbShow, pdwUIElementId);
  log(type_, "ITfUIElementMgr", "BeginUIElement", "(_, {}, 0x{:x})->0x{:x}", intToBool(*pbShow), *pdwUIElementId,
      ul(hr));
  return hr;
}

STDMETHODIMP DTfUIElementMgr::UpdateUIElement(
    /* [in] */ DWORD dwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->UpdateUIElement(dwUIElementId);
  log(type_, "ITfUIElementMgr", "UpdateUIElement", "(0x{:x})->0x{:x}", dwUIElementId, ul(hr));
  return hr;
}

STDMETHODIMP DTfUIElementMgr::EndUIElement(
    /* [in] */ DWORD dwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->EndUIElement(dwUIElementId);
  log(type_, "ITfUIElementMgr", "EndUIElement", "(0x{:x})->0x{:x}", dwUIElementId, ul(hr));
  return hr;
}

STDMETHODIMP DTfUIElementMgr::GetUIElement(
    /* [in] */ DWORD dwUIELementId,
    /* [out] */ ITfUIElement **ppElement) {
  auto hr = ((ITfUIElementMgr *)object_)->GetUIElement(dwUIELementId, ppElement);
  log(type_, "ITfUIElementMgr", "GetUIElement", "(0x{:x}, _)->0x{:x}", dwUIELementId, ul(hr));
  return hr;
}

STDMETHODIMP DTfUIElementMgr::EnumUIElements(
    /* [out] */ IEnumTfUIElements **ppEnum) {
  auto hr = ((ITfUIElementMgr *)object_)->EnumUIElements(ppEnum);
  log(type_, "ITfUIElementMgr", "EnumUIElements", "(_)->0x{:x}", ul(hr));
  return hr;
}
