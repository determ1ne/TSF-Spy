#include "ditto.h"
#include "fmt/format.h"
#include <algorithm>
#include <detours.h>
#include <functional>
#include <unordered_map>
#include <vector>

template <>
struct std::hash<IID> {
  std::size_t operator()(const IID &iid) const noexcept {
    const uint64_t *pGuid1 = reinterpret_cast<const uint64_t *>(&iid);
    const uint64_t *pGuid2 = pGuid1 + 1;
    return std::hash<uint64_t>()(*pGuid1) ^ std::hash<uint64_t>()(*pGuid2);
  }
};

std::unordered_map<void *, std::vector<Ditto *>> g_dittoMap{};
std::unordered_map<IID, Ditto *(*)(void *, REFIID, Ditto::InterfaceType)> g_ctorMap{};

template <typename T>
Ditto *dittoCtor(void *object, REFIID riid, Ditto::InterfaceType type) {
  return new T(object);
}

void initCtorMap() {
  g_ctorMap[IID_ITfKeyEventSink] = &dittoCtor<DTfKeyEventSink>;
  g_ctorMap[IID_ITfThreadMgr] = &dittoCtor<DTfThreadMgr>;
  g_ctorMap[IID_ITfUIElementMgr] = &dittoCtor<DTfUIElementMgr>;
  g_ctorMap[IID_ITfSource] = &dittoCtor<DTfSource>;
  g_ctorMap[IID_ITfCategoryMgr] = &dittoCtor<DTfCategoryMgr>;
  g_ctorMap[IID_ITfKeystrokeMgr] = &dittoCtor<DTfKeystrokeMgr>;
  g_ctorMap[IID_ITfLangBarItemMgr] = &dittoCtor<DTfLangBarItemMgr>;
  g_ctorMap[IID_ITfCompartmentMgr] = &dittoCtor<DTfCompartmentMgr>;
}

void *castAs(Ditto *ditto, REFIID riid) {
#define ADD_CAST(cls)                                                                                                  \
  if (IsEqualIID(riid, IID_##cls)) {                                                                                   \
    return dynamic_cast<cls *>(ditto);                                                                                 \
  }

  ADD_CAST(IUnknown)
  ADD_CAST(ITfKeyEventSink)
  ADD_CAST(ITfThreadMgr)
  ADD_CAST(ITfUIElementMgr)
  ADD_CAST(ITfSource)
  ADD_CAST(ITfCategoryMgr)
  ADD_CAST(ITfKeystrokeMgr)
  ADD_CAST(ITfLangBarItemMgr)
  ADD_CAST(ITfCompartmentMgr)

  throw std::exception("Cast type not found");
}

#pragma region Ditto

Ditto::Ditto(void *object, REFIID riid, InterfaceType type) : object_(object), iid_(riid), type_(type) {
  auto it = g_dittoMap.find(object);
  if (it == g_dittoMap.end()) {
    g_dittoMap[object] = std::vector<Ditto *>();
  }
  g_dittoMap[object].emplace_back(this);
}

Ditto::~Ditto() {
  auto it = g_dittoMap.find(object_);
  if (it == g_dittoMap.end())
    return;

  std::vector<Ditto *> vec(it->second);
  g_dittoMap.erase(it);
  auto itVec = std::find(vec.cbegin(), vec.cend(), this);
  if (itVec == vec.cend())
    return;

  vec.erase(itVec);

  for (auto &ditto : vec) {
    delete ditto;
  }
}

Ditto *Ditto::CreateOrGet(void *object, REFIID riid, InterfaceType type) {
  return CreateOrGet(object, riid, false, type);
}

Ditto *Ditto::CreateOrGet(void *object, const IID &riid, bool checked, InterfaceType type) {
  if (!checked && !IsIIDSupported(riid)) {
    return nullptr;
  }

  auto it = g_dittoMap.find(object);
  if (it == g_dittoMap.end()) {
    g_dittoMap[object] = std::vector<Ditto *>();
  }
  auto &vec = g_dittoMap[object];

  for (auto &ditto : vec) {
    if (IsEqualIID(ditto->iid_, riid)) {
      return ditto;
    }
  }

  return g_ctorMap[riid](object, riid, type);
}

bool Ditto::IsIIDSupported(REFIID riid) { return g_ctorMap.find(riid) != g_ctorMap.end(); }

#pragma endregion Ditto

#pragma region IUnknown(Ditto)

STDMETHODIMP Ditto::QueryInterface(REFIID riid, void **ppvObject) {
  if (ppvObject == nullptr) {
    return E_INVALIDARG;
  }
  *ppvObject = nullptr;

  if (IsEqualIID(riid, this->iid_) || IsEqualIID(riid, IID_IUnknown)) {
    *ppvObject = this;
  }

  bool hr = ((IUnknown *)object_)->QueryInterface(riid, ppvObject);
  auto logContent = formatLog(type_, getIIDName(iid_), "QueryInterface", "({}, _)->0x{:x}", getIIDName(riid), ul(hr));
  if (hr == S_OK) {
    AddRef();
    if (IsIIDSupported(riid)) {
      *ppvObject = castAs(CreateOrGet(*ppvObject, riid, true, type_), riid);
    } else {
      logContent += "!!UNSUPPORTED_INTERFACE";
    }
  }

  OutputDebugStringA(logContent.c_str());
  return hr;
}

STDMETHODIMP_(ULONG) Ditto::AddRef() { return ((IUnknown *)object_)->AddRef(); }
STDMETHODIMP_(ULONG) Ditto::Release() { return ((IUnknown *)object_)->Release(); }

#pragma endregion IUnknown(Ditto)

#define DEFINE_IUNKNOWN(cls)                                                                                           \
  STDMETHODIMP cls::QueryInterface(REFIID riid, void **ppvObject) { return Ditto::QueryInterface(riid, ppvObject); }   \
  STDMETHODIMP_(ULONG) cls::AddRef() { return Ditto::AddRef(); }                                                       \
  STDMETHODIMP_(ULONG) cls::Release() { return Ditto::Release(); }

#pragma region DTfKeyEventSink

DEFINE_IUNKNOWN(DTfKeyEventSink)

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

#pragma endregion DTfKeyEventSink

#pragma region DTfThreadMgr

DEFINE_IUNKNOWN(DTfThreadMgr)

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

#pragma endregion DTfThreadMgr

#pragma region DTfUIElementMgr

DEFINE_IUNKNOWN(DTfUIElementMgr)
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

#pragma endregion DTfUIElementMgr

#pragma region DTfSource

DEFINE_IUNKNOWN(DTfSource)
STDMETHODIMP DTfSource::AdviseSink(
    /* [in] */ REFIID riid,
    /* [iid_is][in] */ IUnknown *punk,
    /* [out] */ DWORD *pdwCookie) {
  auto hr = ((ITfSource *)object_)->AdviseSink(riid, punk, pdwCookie);
  log(type_, "ITfSource", "AdviseSink", "({}, _, 0x{:x})->0x{:x}", getIIDName(riid), *pdwCookie, ul(hr));
  return hr;
}

STDMETHODIMP DTfSource::UnadviseSink(
    /* [in] */ DWORD dwCookie) {
  auto hr = ((ITfSource *)object_)->UnadviseSink(dwCookie);
  log(type_, "ITfSource", "UnadviseSink", "(0x{:x})->0x{:x}", dwCookie, ul(hr));
  return hr;
}

#pragma endregion DTfSource

#pragma region DTfCategoryMgr

DEFINE_IUNKNOWN(DTfCategoryMgr)
STDMETHODIMP DTfCategoryMgr::RegisterCategory(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rcatid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterCategory(rclsid, rcatid, rguid);
  log(type_, "ITfCategoryMgr", "RegisterCategory", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rcatid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterCategory(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rcatid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterCategory(rclsid, rcatid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterCategory", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rcatid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::EnumCategoriesInItem(
    /* [in] */ REFGUID rguid,
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumCategoriesInItem(rguid, ppEnum);
  log(type_, "ITfCategoryMgr", "EnumCategoriesInItem", "({}, _)->0x{:x}", getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::EnumItemsInCategory(
    /* [in] */ REFGUID rcatid,
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumItemsInCategory(rcatid, ppEnum);
  log(type_, "ITfCategoryMgr", "EnumItemsInCategory", "({}, _)->0x{:x}", getGUIDName(rcatid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::FindClosestCategory(
    /* [in] */ REFGUID rguid,
    /* [out] */ GUID *pcatid,
    /* [size_is][in] */ const GUID **ppcatidList,
    /* [in] */ ULONG ulCount) {
  auto hr = ((ITfCategoryMgr *)object_)->FindClosestCategory(rguid, pcatid, ppcatidList, ulCount);
  log(type_, "ITfCategoryMgr", "FindClosestCategory", "({}, {}, _, {})->0x{:x}", getGUIDName(rguid),
      getGUIDName(*pcatid), ulCount, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUIDDescription(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid,
    /* [size_is][in] */ const WCHAR *pchDesc,
    /* [in] */ ULONG cch) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDescription(rclsid, rguid, pchDesc, cch);
  log(type_, "ITfCategoryMgr", "RegisterGUIDDescription", "({}, {}, {}, {})->0x{:x}", getCLSIDName(rclsid),
      getGUIDName(rguid), rawWstr{pchDesc}, cch, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterGUIDDescription(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDescription(rclsid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterGUIDDescription", "({}, {})->0x{:x}", getCLSIDName(rclsid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUIDDescription(
    /* [in] */ REFGUID rguid,
    /* [out] */ BSTR *pbstrDesc) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDescription(rguid, pbstrDesc);
  log(type_, "ITfCategoryMgr", "GetGUIDDescription", "({}, {})->0x{:x}", getGUIDName(rguid), rawWstr{*pbstrDesc},
      ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUIDDWORD(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid,
    /* [in] */ DWORD dw) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDWORD(rclsid, rguid, dw);
  log(type_, "ITfCategoryMgr", "RegisterGUIDDWORD", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rguid),
      dw, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterGUIDDWORD(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDWORD(rclsid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterGUIDDWORD", "({}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rguid),
      ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUIDDWORD(
    /* [in] */ REFGUID rguid,
    /* [out] */ DWORD *pdw) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDWORD(rguid, pdw);
  log(type_, "ITfCategoryMgr", "GetGUIDDWORD", "({}, 0x{:x})->0x{:x}", getGUIDName(rguid), *pdw, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUID(
    /* [in] */ REFGUID rguid,
    /* [out] */ TfGuidAtom *pguidatom) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUID(rguid, pguidatom);
  log(type_, "ITfCategoryMgr", "RegisterGUID", "({}, _)->0x{:x}", getGUIDName(rguid), *pguidatom);
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUID(
    /* [in] */ TfGuidAtom guidatom,
    /* [out] */ GUID *pguid) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUID(guidatom, pguid);
  log(type_, "ITfCategoryMgr", "GetGUID", "(0x{:x}, {})->0x{:x}", guidatom, getGUIDName(*pguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::IsEqualTfGuidAtom(
    /* [in] */ TfGuidAtom guidatom,
    /* [in] */ REFGUID rguid,
    /* [out] */ BOOL *pfEqual) {
  auto hr = ((ITfCategoryMgr *)object_)->IsEqualTfGuidAtom(guidatom, rguid, pfEqual);
  log(type_, "ITfCategoryMgr", "IsEqualTfGuidAtom", "(0x{:x}, {}, {})->0x{:x}", guidatom, getGUIDName(rguid), *pfEqual,
      ul(hr));
  return hr;
}

#pragma endregion DTfCategoryMgr

#pragma region DTfKeystrokeMgr

DEFINE_IUNKNOWN(DTfKeystrokeMgr)

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

#pragma endregion DTfKeystrokeMgr

#pragma region DTfLangBarItemMgr

DEFINE_IUNKNOWN(DTfLangBarItemMgr)

STDMETHODIMP DTfLangBarItemMgr::EnumItems(
    /* [out] */ IEnumTfLangBarItems **ppEnum) {
  auto hr = ((ITfLangBarItemMgr *)object_)->EnumItems(ppEnum);
  log(type_, "ITfLangBarItemMgr", "EnumItems", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItem(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfLangBarItem **ppItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItem(rguid, ppItem);
  log(type_, "ITfLangBarItemMgr", "GetItem", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AddItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AddItem(punk);
  log(type_, "ITfLangBarItemMgr", "AddItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::RemoveItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->RemoveItem(punk);
  log(type_, "ITfLangBarItemMgr", "RemoveItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AdviseItemSink(
    /* [in] */ ITfLangBarItemSink *punk,
    /* [out] */ DWORD *pdwCookie,
    /* [in] */ REFGUID rguidItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemSink(punk, pdwCookie, rguidItem);
  log(type_, "ITfLangBarItemMgr", "AdviseItemSink", "({}, {:x}, {})->{:x}", fmt::ptr(punk), *pdwCookie,
      guidToString(rguidItem), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::UnadviseItemSink(
    /* [in] */ DWORD dwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemSink(dwCookie);
  log(type_, "ITfLangBarItemMgr", "UnadviseItemSink", "({:x})->{:x}", dwCookie, hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemFloatingRect(
    /* [in] */ DWORD dwThreadId,
    /* [in] */ REFGUID rguid,
    /* [out] */ RECT *prc) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemFloatingRect(dwThreadId, rguid, prc);
  log(type_, "ITfLangBarItemMgr", "GetItemFloatingRect", "({:x}, {}, _)->{:x}", dwThreadId, guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemsStatus(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ const GUID *prgguid,
    /* [size_is(ulCount)][out] */ DWORD *pdwStatus) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemsStatus(ulCount, prgguid, pdwStatus);
  log(type_, "ITfLangBarItemMgr", "GetItemsStatus", "({}, {}, _)->{:x}", ulCount, guidToString(prgguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemNum(
    /* [out] */ ULONG *pulCount) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemNum(pulCount);
  log(type_, "ITfLangBarItemMgr", "GetItemNum", "(_)->{:x}", *pulCount, hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItems(
    /* [in] */ ULONG ulCount,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ ITfLangBarItem **ppItem,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ TF_LANGBARITEMINFO *pInfo,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ DWORD *pdwStatus,
    /* [unique][out][in] */ ULONG *pcFetched) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItems(ulCount, ppItem, pInfo, pdwStatus, pcFetched);
  log(type_, "ITfLangBarItemMgr", "GetItems", "({}, {}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppItem), fmt::ptr(pInfo),
      fmt::ptr(pdwStatus), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AdviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ ITfLangBarItemSink **ppunk,
    /* [size_is(ulCount)][in] */ const GUID *pguidItem,
    /* [size_is(ulCount)][out] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemsSink(ulCount, ppunk, pguidItem, pdwCookie);
  log(type_, "ITfLangBarItemMgr", "AdviseItemsSink", "({}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppunk),
      guidToString(pguidItem), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::UnadviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemsSink(ulCount, pdwCookie);
  log(type_, "ITfLangBarItemMgr", "UnadviseItemsSink", "({}, _)->{:x}", ulCount, hr);
  return hr;
}

#pragma endregion DTfLangBarItemMgr

#pragma region DTfCompartmentMgr

DEFINE_IUNKNOWN(DTfCompartmentMgr)

STDMETHODIMP DTfCompartmentMgr::GetCompartment(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfCompartment **ppcomp) {
  auto hr = ((ITfCompartmentMgr *)object_)->GetCompartment(rguid, ppcomp);
  log(type_, "ITfCompartmentMgr", "GetCompartment", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP DTfCompartmentMgr::ClearCompartment(
    /* [in] */ TfClientId tid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCompartmentMgr *)object_)->ClearCompartment(tid, rguid);
  log(type_, "ITfCompartmentMgr", "ClearCompartment", "({:x}, {})->{:x}", tid, guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP DTfCompartmentMgr::EnumCompartments(
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCompartmentMgr *)object_)->EnumCompartments(ppEnum);
  log(type_, "ITfCompartmentMgr", "EnumCompartments", "(_)->{:x}", hr);
  return hr;
}

#pragma endregion DTfCompartmentMgr

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

    Ditto *pv = Ditto::CreateOrGet(pResults->pItf, *pResults->pIID, LogType::Manager);
    if (pv == nullptr)
      return hr;

    log(LogType::System, "", "CoCreateInstance", "({}, {})->0x{:x}", getCLSIDName(rclsid), getIIDName(*pResults->pIID),
        hr);
    pResults->pItf = reinterpret_cast<IUnknown *>(castAs(pv, *pResults->pIID));
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
