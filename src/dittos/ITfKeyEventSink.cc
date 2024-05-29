#include "ditto.h"

STDMETHODIMP DTfKeyEventSink::OnSetFocus(
    /* [in] */ BOOL fForeground) {
  auto hr = ((ITfKeyEventSink *)object_)->OnSetFocus(fForeground);
  log(type_, "ITfKeyEventSink", "OnSetFocus", "({})->0x{:x}", fForeground, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnTestKeyDown(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnTestKeyDown(pic, wParam, lParam, pfEaten);
  log(type_, "ITfKeyEventSink", "OnTestKeyDown", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnTestKeyUp(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnTestKeyUp(pic, wParam, lParam, pfEaten);
  log(type_, "ITfKeyEventSink", "OnTestKeyUp", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnKeyDown(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnKeyDown(pic, wParam, lParam, pfEaten);
  log(type_, "ITfKeyEventSink", "OnKeyDown", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnKeyUp(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnKeyUp(pic, wParam, lParam, pfEaten);
  log(type_, "ITfKeyEventSink", "OnKeyUp", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnPreservedKey(
    /* [in] */ ITfContext *pic,
    /* [in] */ REFGUID rguid,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnPreservedKey(pic, rguid, pfEaten);
  log(type_, "ITfKeyEventSink", "OnPreservedKey", "(_, {}, {})->0x{:x}", getIIDName(rguid), *pfEaten, ul(hr));
  return hr;
}
