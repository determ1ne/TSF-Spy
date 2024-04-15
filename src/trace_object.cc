#include "trace_object.h"
#include "util.h"
#include <detours.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <windows.h>

// {63D45364-7291-4789-BF12-F5F50D68FB8C}
static const GUID IID_ITraceObject = {0x63d45364, 0x7291, 0x4789, {0xbf, 0x12, 0xf5, 0xf5, 0xd, 0x68, 0xfb, 0x8c}};
constexpr HRESULT E_TRACE_ALREADY_SET = 0xDEADBEEF;

// helpers
inline bool intToBool(BOOL i) { return i != FALSE; }
auto format_as(const TF_PRESERVEDKEY &preKey) {
  return fmt::format("TF_PRESERVEDKEY{{uVKey: {:x}, uModifiers: {:x}}}", preKey.uVKey, preKey.uModifiers);
}
struct rawWstr {
  const WCHAR *str;
};
auto format_as(const rawWstr &w) {
  if (w.str == nullptr)
    return std::string();
  int srcLen = wcslen(w.str);
  int len = WideCharToMultiByte(CP_UTF8, 0, w.str, srcLen, 0, 0, NULL, NULL);
  std::unique_ptr<char[]> buf(new char[len + 1]);
  WideCharToMultiByte(CP_UTF8, 0, w.str, srcLen, buf.get(), len, NULL, NULL);
  return fmt::format("\"&{}\"", std::string_view(buf.get(), len));
}

bool isIIDSupported(REFIID riid) {
  if (IsEqualIID(riid, IID_IUnknown)
      /* clang-format off */
    || IsEqualIID(riid, IID_ITfThreadMgr) 
    || IsEqualIID(riid, IID_ITfSource)
    || IsEqualIID(riid, IID_ITfKeystrokeMgr)
    || IsEqualIID(riid, IID_ITfCategoryMgr)
    || IsEqualIID(riid, IID_ITfLangBarItemMgr)
    || IsEqualIID(riid, IID_ITfCompartmentMgr)
    || IsEqualIID(riid, IID_ITfUIElementMgr)
      /* clang-format on */
  ) {
    return true;
  }
  return false;
}

void *TraceObject::castAs(REFIID riid) {
#define SUPPORT_INTERFACE(Interface)                                                                                   \
  if (IsEqualIID(riid, IID_##Interface)) {                                                                             \
    return (Interface *)this;                                                                                          \
  }
  if (IsEqualIID(riid, IID_IUnknown))
    return (ITfThreadMgr *)this;

  SUPPORT_INTERFACE(ITfThreadMgr)
  SUPPORT_INTERFACE(ITfSource)
  SUPPORT_INTERFACE(ITfKeystrokeMgr)
  SUPPORT_INTERFACE(ITfCategoryMgr)
  SUPPORT_INTERFACE(ITfLangBarItemMgr)
  SUPPORT_INTERFACE(ITfCompartmentMgr)
  SUPPORT_INTERFACE(ITfUIElementMgr)

  return nullptr;
#undef SUPPORT_INTERFACE
}

#pragma region IUnknown
STDMETHODIMP TraceObject::QueryInterface(REFIID riid, void **ppvObject) {
  if (ppvObject == nullptr) {
    return E_INVALIDARG;
  }
  *ppvObject = nullptr;

  // check if the object already has a trace object
  if (IsEqualIID(riid, IID_ITraceObject)) {
    *ppvObject = this;
    return E_TRACE_ALREADY_SET;
  }
  auto hr = ((IUnknown *)object_)->QueryInterface(IID_ITraceObject, ppvObject);
  bool isTraceObject = hr == E_TRACE_ALREADY_SET;

  hr = ((IUnknown *)object_)->QueryInterface(riid, ppvObject);
  auto logContent =
      fmt::format("TSFSPY: [C]{}::QueryInterface({}, _)->{:x}", interfaceType_, getIIDName(riid), (long)hr);
  if (hr == S_OK) {
    if (isTraceObject) {
      // do nothing
    } else if (isIIDSupported(riid)) {
      *ppvObject = (new TraceObject(*ppvObject, proxyObject_, getIIDName(riid)))->castAs(riid);
    } else {
      logContent += "!!UNSUPPORTED_INTERFACE";
    }
  }
  OutputDebugStringA(logContent.c_str());

  return hr;
}

STDMETHODIMP_(ULONG) TraceObject::AddRef() { return ((IUnknown *)object_)->AddRef(); }
STDMETHODIMP_(ULONG) TraceObject::Release() { return ((IUnknown *)object_)->Release(); }
#pragma endregion IUnknown

#pragma region ITfThreadMgr
STDMETHODIMP TraceObject::Activate(TfClientId *ptid) {
  auto hr = ((ITfThreadMgr *)object_)->Activate(ptid);
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::Activate({:x})->{:x}", (DWORD)*ptid, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::Deactivate(void) {
  auto hr = ((ITfThreadMgr *)object_)->Deactivate();
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::Deactivate()->{:x}", hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::CreateDocumentMgr(ITfDocumentMgr **ppdim) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::CreateDocumentMgr(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->CreateDocumentMgr(ppdim);
}
STDMETHODIMP TraceObject::EnumDocumentMgrs(IEnumTfDocumentMgrs **ppEnum) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::EnumDocumentMgrs(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->EnumDocumentMgrs(ppEnum);
}
STDMETHODIMP TraceObject::GetFocus(ITfDocumentMgr **ppdimFocus) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::GetFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->GetFocus(ppdimFocus);
}
STDMETHODIMP TraceObject::SetFocus(ITfDocumentMgr *pdimFocus) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::SetFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->SetFocus(pdimFocus);
}
STDMETHODIMP TraceObject::AssociateFocus(HWND hwnd, ITfDocumentMgr *pdimNew, ITfDocumentMgr **ppdimPrev) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::AssociateFocus({:x}, _, _)", fmt::ptr(hwnd));
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->AssociateFocus(hwnd, pdimNew, ppdimPrev);
}
STDMETHODIMP TraceObject::IsThreadFocus(BOOL *pfThreadFocus) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::IsThreadFocus(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->IsThreadFocus(pfThreadFocus);
}
STDMETHODIMP TraceObject::GetFunctionProvider(REFCLSID clsid, ITfFunctionProvider **ppFuncProv) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::GetFunctionProvider({}, _)", guidToString(clsid));
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->GetFunctionProvider(clsid, ppFuncProv);
}
STDMETHODIMP TraceObject::EnumFunctionProviders(IEnumTfFunctionProviders **ppEnum) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::EnumFunctionProviders(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->EnumFunctionProviders(ppEnum);
}
STDMETHODIMP TraceObject::GetGlobalCompartment(ITfCompartmentMgr **ppCompMgr) {
  auto logContent = fmt::format("TSFSPY: [C]ITfThreadMgr::GetGlobalCompartment(_)");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgr *)object_)->GetGlobalCompartment(ppCompMgr);
}
#pragma endregion ITfThreadMgr

#pragma region ITfSource
STDMETHODIMP TraceObject::AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie) {
  auto hr = ((ITfSource *)object_)->AdviseSink(riid, punk, pdwCookie);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfSource::AdviseSink({}, _, {:x})->{:x}", getIIDName(riid), *pdwCookie, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnadviseSink(DWORD dwCookie) {
  auto hr = ((ITfSource *)object_)->UnadviseSink(dwCookie);
  auto logContent = fmt::format("TSFSPY: [C]ITfSource::UnadviseSink({:x})->{:x}", dwCookie, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
#pragma endregion ITfSource

#pragma region ITfKeystrokeMgr
STDMETHODIMP TraceObject::AdviseKeyEventSink(TfClientId tid, ITfKeyEventSink *pSink, BOOL fForeground) {
  auto hr = ((ITfKeystrokeMgr *)object_)->AdviseKeyEventSink(tid, pSink, fForeground);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::AdviseKeyEventSink({:x}, {}, {})->{:x}", tid,
                                fmt::ptr(pSink), intToBool(fForeground), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnadviseKeyEventSink(TfClientId tid) {
  auto hr = ((ITfKeystrokeMgr *)object_)->UnadviseKeyEventSink(tid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::UnadviseKeyEventSink({:x})->{:x}", tid, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetForeground(CLSID *pclsid) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetForeground(pclsid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetForeground({})->{:x}", guidToString(pclsid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::TestKeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->TestKeyDown(wParam, lParam, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::TestKeyDown({:x}, {:x}, {})->{:x}", wParam, lParam,
                                intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::TestKeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->TestKeyUp(wParam, lParam, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::TestKeyUp({:x}, {:x}, {})->{:x}", wParam, lParam,
                                intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::KeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->KeyDown(wParam, lParam, pfEaten);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::KeyDown({:x}, {:x}, {})->{:x}", wParam, lParam, intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::KeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->KeyUp(wParam, lParam, pfEaten);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::KeyUp({:x}, {:x}, {})->{:x}", wParam, lParam, intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetPreservedKey(ITfContext *pic, const TF_PRESERVEDKEY *pprekey, GUID *pguid) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetPreservedKey(pic, pprekey, pguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetPreservedKey(&{:x}, &{}, {})->{:x}", fmt::ptr(pic),
                                *pprekey, guidToString(pguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::IsPreservedKey(REFGUID rguid, const TF_PRESERVEDKEY *pprekey, BOOL *pfRegistered) {
  auto hr = ((ITfKeystrokeMgr *)object_)->IsPreservedKey(rguid, pprekey, pfRegistered);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::IsPreservedKey({}, {}, {})->{:x}", guidToString(rguid),
                                *pprekey, intToBool(*pfRegistered), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::PreserveKey(TfClientId tid, REFGUID rguid, const TF_PRESERVEDKEY *prekey,
                                      const WCHAR *pchDesc, ULONG cchDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->PreserveKey(tid, rguid, prekey, pchDesc, cchDesc);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::PreserveKey({:x}, {}, {}, {}, {})->{:x}", tid,
                                guidToString(rguid), *prekey, rawWstr{pchDesc}, cchDesc, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnpreserveKey(REFGUID rguid, const TF_PRESERVEDKEY *pprekey) {
  auto hr = ((ITfKeystrokeMgr *)object_)->UnpreserveKey(rguid, pprekey);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::UnpreserveKey({}, {})->{:x}", guidToString(rguid), *pprekey, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::SetPreservedKeyDescription(REFGUID rguid, const WCHAR *pchDesc, ULONG cchDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->SetPreservedKeyDescription(rguid, pchDesc, cchDesc);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::SetPreservedKeyDescription({}, {}, {})->{:x}",
                                guidToString(rguid), rawWstr{pchDesc}, cchDesc, hr);
  // OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetPreservedKeyDescription(REFGUID rguid, BSTR *pbstrDesc) {
  auto hr = ((ITfKeystrokeMgr *)object_)->GetPreservedKeyDescription(rguid, pbstrDesc);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfKeystrokeMgr::GetPreservedKeyDescription({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::SimulatePreservedKey(ITfContext *pic, REFGUID rguid, BOOL *pfEaten) {
  auto hr = ((ITfKeystrokeMgr *)object_)->SimulatePreservedKey(pic, rguid, pfEaten);
  auto logContent = fmt::format("TSFSPY: [C]ITfKeystrokeMgr::SimulatePreservedKey(&{:x}, {}, {})->{:x}", fmt::ptr(pic),
                                guidToString(rguid), intToBool(*pfEaten), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
#pragma endregion ITfKeystrokeMgr

#pragma region ITfCategoryMgr
STDMETHODIMP TraceObject::RegisterCategory(REFCLSID rclsid, REFGUID rcatid, REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterCategory(rclsid, rcatid, rguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::RegisterCategory({}, {}, {})->{:x}", guidToString(rclsid),
                                guidToString(rcatid), guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnregisterCategory(REFCLSID rclsid, REFGUID rcatid, REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterCategory(rclsid, rcatid, rguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::UnregisterCategory({}, {}, {})->{:x}", guidToString(rclsid),
                                guidToString(rcatid), guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::EnumCategoriesInItem(REFGUID rguid, IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumCategoriesInItem(rguid, ppEnum);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfCategoryMgr::EnumCategoriesInItem({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::EnumItemsInCategory(REFGUID rcatid, IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumItemsInCategory(rcatid, ppEnum);
  auto logContent =
      fmt::format("TSFSPY: [C]ITfCategoryMgr::EnumItemsInCategory({}, _)->{:x}", guidToString(rcatid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::FindClosestCategory(REFGUID rguid, GUID *pcatid, const GUID **ppcatidList, ULONG ulCount) {
  auto hr = ((ITfCategoryMgr *)object_)->FindClosestCategory(rguid, pcatid, ppcatidList, ulCount);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::FindClosestCategory({}, {}, _, {})->{:x}",
                                guidToString(rguid), guidToString(pcatid), ulCount, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::RegisterGUIDDescription(REFCLSID rclsid, REFGUID rguid, const WCHAR *pchDesc, ULONG cch) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDescription(rclsid, rguid, pchDesc, cch);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::RegisterGUIDDescription({}, {}, {}, {})->{:x}",
                                guidToString(rclsid), guidToString(rguid), rawWstr{pchDesc}, cch, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnregisterGUIDDescription(REFCLSID rclsid, REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDescription(rclsid, rguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::UnregisterGUIDDescription({}, {})->{:x}",
                                guidToString(rclsid), guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetGUIDDescription(REFGUID rguid, BSTR *pbstrDesc) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDescription(rguid, pbstrDesc);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::GetGUIDDescription({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::RegisterGUIDDWORD(REFCLSID rclsid, REFGUID rguid, DWORD dw) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDWORD(rclsid, rguid, dw);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::RegisterGUIDDWORD({}, {}, {})->{:x}", guidToString(rclsid),
                                guidToString(rguid), dw, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::UnregisterGUIDDWORD(REFCLSID rclsid, REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDWORD(rclsid, rguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::UnregisterGUIDDWORD({}, {})->{:x}", guidToString(rclsid),
                                guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetGUIDDWORD(REFGUID rguid, DWORD *pdw) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDWORD(rguid, pdw);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::GetGUIDDWORD({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::RegisterGUID(REFGUID rguid, TfGuidAtom *pguidatom) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUID(rguid, pguidatom);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::RegisterGUID({}, _)->{:x}", guidToString(rguid), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::GetGUID(TfGuidAtom guidatom, GUID *pguid) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUID(guidatom, pguid);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::GetGUID({}, _)->{:x}", guidatom, hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDMETHODIMP TraceObject::IsEqualTfGuidAtom(TfGuidAtom guidatom, REFGUID rguid, BOOL *pfEqual) {
  auto hr = ((ITfCategoryMgr *)object_)->IsEqualTfGuidAtom(guidatom, rguid, pfEqual);
  auto logContent = fmt::format("TSFSPY: [C]ITfCategoryMgr::IsEqualTfGuidAtom({}, {}, {})->{:x}", guidatom,
                                guidToString(rguid), intToBool(*pfEqual), hr);
  OutputDebugStringA(logContent.c_str());
  return hr;
}
#pragma endregion ITfCategoryMgr

#pragma region ITfLangBarItemMgr
STDMETHODIMP TraceObject::EnumItems(
    /* [out] */ IEnumTfLangBarItems **ppEnum) {
  auto hr = ((ITfLangBarItemMgr *)object_)->EnumItems(ppEnum);
  log(LogType::Common, "ITfLangBarItemMgr", "EnumItems", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP TraceObject::GetItem(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfLangBarItem **ppItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItem(rguid, ppItem);
  log(LogType::Common, "ITfLangBarItemMgr", "GetItem", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP TraceObject::AddItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AddItem(punk);
  log(LogType::Common, "ITfLangBarItemMgr", "AddItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP TraceObject::RemoveItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->RemoveItem(punk);
  log(LogType::Common, "ITfLangBarItemMgr", "RemoveItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP TraceObject::AdviseItemSink(
    /* [in] */ ITfLangBarItemSink *punk,
    /* [out] */ DWORD *pdwCookie,
    /* [in] */ REFGUID rguidItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemSink(punk, pdwCookie, rguidItem);
  log(LogType::Common, "ITfLangBarItemMgr", "AdviseItemSink", "({}, {:x}, {})->{:x}", fmt::ptr(punk), *pdwCookie,
      guidToString(rguidItem), hr);
  return hr;
}
STDMETHODIMP TraceObject::UnadviseItemSink(
    /* [in] */ DWORD dwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemSink(dwCookie);
  log(LogType::Common, "ITfLangBarItemMgr", "UnadviseItemSink", "({:x})->{:x}", dwCookie, hr);
  return hr;
}
STDMETHODIMP TraceObject::GetItemFloatingRect(
    /* [in] */ DWORD dwThreadId,
    /* [in] */ REFGUID rguid,
    /* [out] */ RECT *prc) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemFloatingRect(dwThreadId, rguid, prc);
  log(LogType::Common, "ITfLangBarItemMgr", "GetItemFloatingRect", "({:x}, {}, _)->{:x}", dwThreadId,
      guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP TraceObject::GetItemsStatus(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ const GUID *prgguid,
    /* [size_is(ulCount)][out] */ DWORD *pdwStatus) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemsStatus(ulCount, prgguid, pdwStatus);
  log(LogType::Common, "ITfLangBarItemMgr", "GetItemsStatus", "({}, {}, _)->{:x}", ulCount, guidToString(prgguid), hr);
  return hr;
}
STDMETHODIMP TraceObject::GetItemNum(
    /* [out] */ ULONG *pulCount) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemNum(pulCount);
  log(LogType::Common, "ITfLangBarItemMgr", "GetItemNum", "(_)->{:x}", *pulCount, hr);
  return hr;
}
STDMETHODIMP TraceObject::GetItems(
    /* [in] */ ULONG ulCount,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ ITfLangBarItem **ppItem,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ TF_LANGBARITEMINFO *pInfo,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ DWORD *pdwStatus,
    /* [unique][out][in] */ ULONG *pcFetched) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItems(ulCount, ppItem, pInfo, pdwStatus, pcFetched);
  log(LogType::Common, "ITfLangBarItemMgr", "GetItems", "({}, {}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppItem),
      fmt::ptr(pInfo), fmt::ptr(pdwStatus), hr);
  return hr;
}
STDMETHODIMP TraceObject::AdviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ ITfLangBarItemSink **ppunk,
    /* [size_is(ulCount)][in] */ const GUID *pguidItem,
    /* [size_is(ulCount)][out] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemsSink(ulCount, ppunk, pguidItem, pdwCookie);
  log(LogType::Common, "ITfLangBarItemMgr", "AdviseItemsSink", "({}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppunk),
      guidToString(pguidItem), hr);
  return hr;
}
STDMETHODIMP TraceObject::UnadviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemsSink(ulCount, pdwCookie);
  log(LogType::Common, "ITfLangBarItemMgr", "UnadviseItemsSink", "({}, _)->{:x}", ulCount, hr);
  return hr;
}
#pragma endregion ITfLangBarItemMgr

#pragma region ITfCompartmentMgr

STDMETHODIMP TraceObject::GetCompartment(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfCompartment **ppcomp) {
  auto hr = ((ITfCompartmentMgr *)object_)->GetCompartment(rguid, ppcomp);
  log(LogType::Common, "ITfCompartmentMgr", "GetCompartment", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP TraceObject::ClearCompartment(
    /* [in] */ TfClientId tid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCompartmentMgr *)object_)->ClearCompartment(tid, rguid);
  log(LogType::Common, "ITfCompartmentMgr", "ClearCompartment", "({:x}, {})->{:x}", tid, guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP TraceObject::EnumCompartments(
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCompartmentMgr *)object_)->EnumCompartments(ppEnum);
  log(LogType::Common, "ITfCompartmentMgr", "EnumCompartments", "(_)->{:x}", hr);
  return hr;
}

#pragma endregion ITfCompartmentMgr

#pragma region ITfUIElementMgr
STDMETHODIMP TraceObject::BeginUIElement(
    /* [in] */ ITfUIElement *pElement,
    /* [out][in] */ BOOL *pbShow,
    /* [out] */ DWORD *pdwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->BeginUIElement(pElement, pbShow, pdwUIElementId);
  log(LogType::Common, "ITfUIElementMgr", "BeginUIElement", "({}, {}, _)->{:x}", fmt::ptr(pElement), intToBool(*pbShow),
      *pdwUIElementId, hr);
  return hr;
}

STDMETHODIMP TraceObject::UpdateUIElement(
    /* [in] */ DWORD dwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->UpdateUIElement(dwUIElementId);
  log(LogType::Common, "ITfUIElementMgr", "UpdateUIElement", "({:x})->{:x}", dwUIElementId, hr);
  return hr;
}

STDMETHODIMP TraceObject::EndUIElement(
    /* [in] */ DWORD dwUIElementId) {
  auto hr = ((ITfUIElementMgr *)object_)->EndUIElement(dwUIElementId);
  log(LogType::Common, "ITfUIElementMgr", "EndUIElement", "({:x})->{:x}", dwUIElementId, hr);
  return hr;
}

STDMETHODIMP TraceObject::GetUIElement(
    /* [in] */ DWORD dwUIELementId,
    /* [out] */ ITfUIElement **ppElement) {
  auto hr = ((ITfUIElementMgr *)object_)->GetUIElement(dwUIELementId, ppElement);
  log(LogType::Common, "ITfUIElementMgr", "GetUIElement", "({:x}, _)->{:x}", dwUIELementId, hr);
  return hr;
}

STDMETHODIMP TraceObject::EnumUIElements(
    /* [out] */ IEnumTfUIElements **ppEnum) {
  auto hr = ((ITfUIElementMgr *)object_)->EnumUIElements(ppEnum);
  log(LogType::Common, "ITfUIElementMgr", "EnumUIElements", "(_)->{:x}", hr);
  return hr;
}

#pragma endregion ITfUIElementMgr

#pragma region Detours

namespace {

static HRESULT(__stdcall *TrueCoCreateInstance)(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid,
                                                LPVOID *ppv) = CoCreateInstance;
static HRESULT(__stdcall *TrueCoCreateInstanceEx)(REFCLSID rclsid, IUnknown *pUnkOuter, DWORD dwClsContext,
                                                  COSERVERINFO *pServerInfo, DWORD dwCount,
                                                  MULTI_QI *pResults) = CoCreateInstanceEx;

HRESULT __stdcall HookedCoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid,
                                         LPVOID *ppv) {
  if (ppv == nullptr) {
    return E_INVALIDARG;
  }

  MULTI_QI qi;
  qi.pItf = NULL;
  qi.pIID = &riid;
  auto hr = CoCreateInstanceEx(rclsid, pUnkOuter, dwClsContext, NULL, 1, &qi);
  *ppv = qi.pItf;
  return hr;
}

HRESULT __stdcall HookedCoCreateInstanceEx(REFCLSID rclsid, IUnknown *pUnkOuter, DWORD dwClsContext,
                                           COSERVERINFO *pServerInfo, DWORD dwCount, MULTI_QI *pResults) {
  auto hr = TrueCoCreateInstanceEx(rclsid, pUnkOuter, dwClsContext, pServerInfo, dwCount, pResults);
  if (dwCount == 1 && pResults != nullptr) {
    if (hr != S_OK)
      return hr;

    if (!isIIDSupported(*pResults->pIID))
      return hr;
    auto logContent =
        fmt::format("TSFSPY: [S]::CoCreateInstance({}, {})", getCLSIDName(rclsid), getIIDName(*pResults->pIID));
    OutputDebugStringA(logContent.c_str());

    pResults->pItf = reinterpret_cast<IUnknown *>(
        (new TraceObject(pResults->pItf, nullptr, getIIDName(*pResults->pIID)))->castAs(*pResults->pIID));
  }
  return hr;
}
} // namespace

bool hookCoCreateInstance() {
  if (NO_ERROR != DetourTransactionBegin()) {
    return false;
  }
  if (NO_ERROR != DetourUpdateThread(GetCurrentThread())) {
    return false;
  }
  if (NO_ERROR != DetourAttach(&(PVOID &)TrueCoCreateInstance, (PVOID)HookedCoCreateInstance)) {
    return false;
  }
  if (NO_ERROR != DetourAttach(&(PVOID &)TrueCoCreateInstanceEx, (PVOID)HookedCoCreateInstanceEx)) {
    return false;
  }
  if (NO_ERROR != DetourTransactionCommit()) {
    return false;
  }
  return true;
}

void restoreCoCreateInstance() {
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID &)TrueCoCreateInstance, (PVOID)HookedCoCreateInstance);
  DetourDetach(&(PVOID &)TrueCoCreateInstanceEx, (PVOID)HookedCoCreateInstanceEx);
  DetourTransactionCommit();
}

#pragma endregion Detours
