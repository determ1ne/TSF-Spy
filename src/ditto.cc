#include "ditto.h"
#include "fmt/format.h"
#include <algorithm>
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
}

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

  if (!checked && !IsIIDSupported(riid)) {
    return nullptr;
  }
  return g_ctorMap[riid](object, riid, type);
}

bool Ditto::IsIIDSupported(REFIID riid) { return g_ctorMap.find(riid) != g_ctorMap.end(); }

#pragma region Ditto

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
      *ppvObject = CreateOrGet(*ppvObject, riid, true, type_);
    } else {
      logContent += "!!UNSUPPORTED_INTERFACE";
    }
  }

  OutputDebugStringA(logContent.c_str());
  return hr;
}

STDMETHODIMP_(ULONG) Ditto::AddRef() { return ((IUnknown *)object_)->AddRef(); }
STDMETHODIMP_(ULONG) Ditto::Release() { return ((IUnknown *)object_)->Release(); }

#pragma endregion Ditto

#define DEFINE_IUNKNOWN(cls)                                                                                           \
  STDMETHODIMP cls::QueryInterface(REFIID riid, void **ppvObject) { return Ditto::QueryInterface(riid, ppvObject); }   \
  STDMETHODIMP_(ULONG) cls::AddRef() { return Ditto::AddRef(); }                                                       \
  STDMETHODIMP_(ULONG) cls::Release() { return Ditto::Release(); }

#pragma region DTfKeyEventSink

DEFINE_IUNKNOWN(DTfKeyEventSink)

STDMETHODIMP DTfKeyEventSink::OnSetFocus(
    /* [in] */ BOOL fForeground) {
  auto hr = ((ITfKeyEventSink *)object_)->OnSetFocus(fForeground);
  log(LogType::TextService, "ITfKeyEventSink", "OnSetFocus", "({})->0x{:x}", fForeground, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnTestKeyDown(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnTestKeyDown(pic, wParam, lParam, pfEaten);
  log(LogType::TextService, "ITfKeyEventSink", "OnTestKeyDown", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten,
      ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnTestKeyUp(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnTestKeyUp(pic, wParam, lParam, pfEaten);
  log(LogType::TextService, "ITfKeyEventSink", "OnTestKeyUp", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten,
      ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnKeyDown(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnKeyDown(pic, wParam, lParam, pfEaten);
  log(LogType::TextService, "ITfKeyEventSink", "OnKeyDown", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten,
      ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnKeyUp(
    /* [in] */ ITfContext *pic,
    /* [in] */ WPARAM wParam,
    /* [in] */ LPARAM lParam,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnKeyUp(pic, wParam, lParam, pfEaten);
  log(LogType::TextService, "ITfKeyEventSink", "OnKeyUp", "(_, {}, {}, {})->0x{:x}", wParam, lParam, *pfEaten, ul(hr));
  return hr;
}

STDMETHODIMP DTfKeyEventSink::OnPreservedKey(
    /* [in] */ ITfContext *pic,
    /* [in] */ REFGUID rguid,
    /* [out] */ BOOL *pfEaten) {
  auto hr = ((ITfKeyEventSink *)object_)->OnPreservedKey(pic, rguid, pfEaten);
  log(LogType::TextService, "ITfKeyEventSink", "OnPreservedKey", "(_, {}, {})->0x{:x}", getIIDName(rguid), *pfEaten,
      ul(hr));
  return hr;
}

#pragma endregion DTfKeyEventSink

#pragma region DTfThreadMgr

DEFINE_IUNKNOWN(DTfThreadMgr)

STDMETHODIMP DTfThreadMgr::Activate(
    /* [out] */ TfClientId *ptid) {
  auto hr = ((ITfThreadMgr *)object_)->Activate(ptid);
  log(LogType::TextService, "ITfThreadMgr", "Activate", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::Deactivate() {
  auto hr = ((ITfThreadMgr *)object_)->Deactivate();
  log(LogType::TextService, "ITfThreadMgr", "Deactivate", "()->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::CreateDocumentMgr(
    /* [out] */ ITfDocumentMgr **ppdim) {

  auto hr = ((ITfThreadMgr *)object_)->CreateDocumentMgr(ppdim);
  log(LogType::TextService, "ITfThreadMgr", "CreateDocumentMgr", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::EnumDocumentMgrs(
    /* [out] */ IEnumTfDocumentMgrs **ppEnum) {
  auto hr = ((ITfThreadMgr *)object_)->EnumDocumentMgrs(ppEnum);
  log(LogType::TextService, "ITfThreadMgr", "EnumDocumentMgrs", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetFocus(
    /* [out] */ ITfDocumentMgr **ppdimFocus) {
  auto hr = ((ITfThreadMgr *)object_)->GetFocus(ppdimFocus);
  log(LogType::TextService, "ITfThreadMgr", "GetFocus", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::SetFocus(
    /* [in] */ ITfDocumentMgr *pdimFocus) {
  auto hr = ((ITfThreadMgr *)object_)->SetFocus(pdimFocus);
  log(LogType::TextService, "ITfThreadMgr", "SetFocus", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::AssociateFocus(
    /* [in] */ HWND hwnd,
    /* [unique][in] */ ITfDocumentMgr *pdimNew,
    /* [out] */ ITfDocumentMgr **ppdimPrev) {
  auto hr = ((ITfThreadMgr *)object_)->AssociateFocus(hwnd, pdimNew, ppdimPrev);
  log(LogType::TextService, "ITfThreadMgr", "AssociateFocus", "(0x{:x}, _, _)->0x{:x}", fmt::ptr(hwnd), ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::IsThreadFocus(
    /* [out] */ BOOL *pfThreadFocus) {
  auto hr = ((ITfThreadMgr *)object_)->IsThreadFocus(pfThreadFocus);
  log(LogType::TextService, "ITfThreadMgr", "IsThreadFocus", "(_)->0x{:x}", *pfThreadFocus, ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetFunctionProvider(
    /* [in] */ REFCLSID clsid,
    /* [out] */ ITfFunctionProvider **ppFuncProv) {
  auto hr = ((ITfThreadMgr *)object_)->GetFunctionProvider(clsid, ppFuncProv);
  log(LogType::TextService, "ITfThreadMgr", "GetFunctionProvider", "({}, _)->0x{:x}", getCLSIDName(clsid), ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::EnumFunctionProviders(
    /* [out] */ IEnumTfFunctionProviders **ppEnum) {
  auto hr = ((ITfThreadMgr *)object_)->EnumFunctionProviders(ppEnum);
  log(LogType::TextService, "ITfThreadMgr", "EnumFunctionProviders", "(_)->0x{:x}", ul(hr));
  return hr;
}

STDMETHODIMP DTfThreadMgr::GetGlobalCompartment(
    /* [out] */ ITfCompartmentMgr **ppCompMgr) {
  auto hr = ((ITfThreadMgr *)object_)->GetGlobalCompartment(ppCompMgr);
  log(LogType::TextService, "ITfThreadMgr", "GetGlobalCompartment", "(_)->0x{:x}", ul(hr));
  return hr;
}

#pragma endregion DTfThreadMgr
