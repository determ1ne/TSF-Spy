#include "ditto.h"

STDMETHODIMP DTfKeystrokeMgr::AdviseKeyEventSink(
    /* [in] */ TfClientId tid,
    /* [in] */ ITfKeyEventSink *pSink,
    /* [in] */ BOOL fForeground) {
  pSink = CreateOrGetDitto<ITfKeyEventSink>(pSink, IID_ITfKeyEventSink, LogType::TextService);
  auto hr = ((ITfKeystrokeMgr *)object_)->AdviseKeyEventSink(tid, pSink, fForeground);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::AdviseKeyEventSink({:x}, {}, {})->{:x}", tid,
                                fmt::ptr(pSink), intToBool(fForeground), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::UnadviseKeyEventSink(
    /* [in] */ TfClientId tid) {

  auto hr = ((ITfKeystrokeMgr *)object_)->UnadviseKeyEventSink(tid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::UnadviseKeyEventSink({:x})->{:x}", tid, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}

STDMETHODIMP DTfKeystrokeMgr::GetForeground(
    /* [out] */ CLSID *pclsid) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetForeground(pclsid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetForeground({})->{:x}", guidToString(pclsid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::TestKeyDown(
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->TestKeyDown(wParam, lParam, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::TestKeyDown({:x}, {:x}, {})->{:x}", wParam, lParam,
                                intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::TestKeyUp(
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->TestKeyUp(wParam, lParam, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::TestKeyUp({:x}, {:x}, {})->{:x}", wParam, lParam,
                                intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::KeyDown(
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->KeyDown(wParam, lParam, pfEaten);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::KeyDown({:x}, {:x}, {})->{:x}", wParam, lParam, intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::KeyUp(
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->KeyUp(wParam, lParam, pfEaten);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::KeyUp({:x}, {:x}, {})->{:x}", wParam, lParam, intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::GetPreservedKey(
    /* [in] */ ITfContext *pic,
    /* [in] */ const TF_PRESERVEDKEY *pprekey,
    /* [out] */ GUID *pguid) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetPreservedKey(pic, pprekey, pguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetPreservedKey(&{:x}, &{}, {})->{:x}", fmt::ptr(pic),
                                *pprekey, guidToString(pguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::IsPreservedKey(
    /* [in] */ REFGUID rguid,
    /* [in] */ const TF_PRESERVEDKEY *pprekey,
    /* [out] */ BOOL *pfRegistered) {
  auto hr = ((ITfKeystrokeMgr *)object_)->IsPreservedKey(rguid, pprekey, pfRegistered);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::IsPreservedKey({}, {}, {})->{:x}", guidToString(rguid),
                                *pprekey, intToBool(*pfRegistered), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::PreserveKey(
    /* [in] */ TfClientId tid,
    /* [in] */ REFGUID rguid,
    /* [in] */ const TF_PRESERVEDKEY *prekey,
    /* [size_is][in] */ const WCHAR *pchDesc,
    /* [in] */ ULONG cchDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->PreserveKey(tid, rguid, prekey, pchDesc, cchDesc);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::PreserveKey({:x}, {}, {}, {}, {})->{:x}", tid,
                                guidToString(rguid), *prekey, rawWstr{pchDesc}, cchDesc, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::UnpreserveKey(
    /* [in] */ REFGUID rguid,
    /* [in] */ const TF_PRESERVEDKEY *pprekey) {
  auto hr = ((ITfKeystrokeMgr *)object_)->UnpreserveKey(rguid, pprekey);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::UnpreserveKey({}, {})->{:x}", guidToString(rguid), *pprekey, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::SetPreservedKeyDescription(
    /* [in] */ REFGUID rguid,
    /* [size_is][in] */ const WCHAR *pchDesc,
    /* [in] */ ULONG cchDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->SetPreservedKeyDescription(rguid, pchDesc, cchDesc);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::SetPreservedKeyDescription({}, {}, {})->{:x}",
                                guidToString(rguid), rawWstr{pchDesc}, cchDesc, hr);
  // OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::GetPreservedKeyDescription(
    /* [in] */ REFGUID rguid,
    /* [out] */ BSTR *pbstrDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetPreservedKeyDescription(rguid, pbstrDesc);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetPreservedKeyDescription({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP DTfKeystrokeMgr::SimulatePreservedKey(
    /* [in] */ ITfContext *pic,
    /* [in] */ REFGUID rguid,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->SimulatePreservedKey(pic, rguid, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::SimulatePreservedKey(&{:x}, {}, {})->{:x}", fmt::ptr(pic),
                                guidToString(rguid), intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
