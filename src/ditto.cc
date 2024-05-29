#include "ditto.h"
#include <algorithm>
#include <detours.h>
#include <functional>
#include <unordered_map>
#include <vector>

extern template struct std::hash<IID>;

std::unordered_map<void *, std::vector<Ditto *>> g_dittoMap{};
std::unordered_map<IID, Ditto *(*)(void *, REFIID, Ditto::InterfaceType), IIDHasher> g_ctorMap{};

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
