#include "ditto.h"
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

void initCtorMap() { g_ctorMap[IID_ITfKeyEventSink] = &dittoCtor<DTfKeyEventSink>; }

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

  auto &vec = it->second;
  auto itVec = std::find(vec.cbegin(), vec.cend(), this);
  if (itVec == vec.cend())
    return;

  vec.erase(itVec);

  for (auto &ditto : vec) {
    delete ditto;
  }
  g_dittoMap.erase(it);
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

#pragma region IUnknown

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

#pragma endregion IUnknown

#define DEFINE_IUNKNOWN(cls)                                                                                           \
  STDMETHODIMP cls::QueryInterface(REFIID riid, void **ppvObject) { return Ditto::QueryInterface(riid, ppvObject); }    \
  STDMETHODIMP_(ULONG) cls::AddRef() { return Ditto::AddRef(); }                                                         \
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
