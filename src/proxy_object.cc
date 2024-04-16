#include "proxy_object.h"
#include "trace_object.h"
#include "util.h"
#include <fmt/core.h>

namespace {

void *castAs(REFIID riid, ProxyObject *obj) {
#define SUPPORT_INTERFACE(Interface)                                                                                   \
  if (IsEqualIID(riid, IID_##Interface)) {                                                                             \
    return (Interface *)obj;                                                                                           \
  }
  if (IsEqualIID(riid, IID_IUnknown))
    return (ITfTextInputProcessor *)obj;

  SUPPORT_INTERFACE(ITfTextInputProcessor)
  SUPPORT_INTERFACE(ITfTextInputProcessorEx)
  SUPPORT_INTERFACE(ITfThreadMgrEventSink)
  SUPPORT_INTERFACE(ITfKeyEventSink)
  SUPPORT_INTERFACE(ITfThreadFocusSink)
  SUPPORT_INTERFACE(ITfCompositionSink)
  SUPPORT_INTERFACE(ITfEditSession)
  SUPPORT_INTERFACE(ITfActiveLanguageProfileNotifySink)
  SUPPORT_INTERFACE(ITfDisplayAttributeProvider)

  return nullptr;
}

bool isIIDSupported(REFIID riid) {
  if (IsEqualIID(riid, IID_IUnknown)
      /* clang-format off */
    || IsEqualIID(riid, IID_ITfTextInputProcessor)
    || IsEqualIID(riid, IID_ITfTextInputProcessorEx)
    || IsEqualIID(riid, IID_ITfThreadMgrEventSink)
    || IsEqualIID(riid, IID_ITfKeyEventSink)
    || IsEqualIID(riid, IID_ITfThreadFocusSink) 
    || IsEqualIID(riid, IID_ITfCompositionSink)
    || IsEqualIID(riid, IID_ITfEditSession) 
    || IsEqualIID(riid, IID_ITfActiveLanguageProfileNotifySink)
    || IsEqualIID(riid, IID_ITfDisplayAttributeProvider)
      /* clang-format on */
  ) {
    return true;
  }
  return false;
}
} // namespace

void *ProxyObject::CastAs(REFIID riid) {
  void *obj = castAs(riid, this);
  if (obj == nullptr) {
    log(LogType::TextService, "ProxyObject", "CastAs", "({})!!UNSUPPORTED_INTERFACE", getIIDName(riid));
  }
  return obj;
}

STDAPI ProxyObject::QueryInterface(REFIID riid, void **ppvObject) {
  if (ppvObject == nullptr) {
    return E_INVALIDARG;
  }
  *ppvObject = nullptr;

  if (IsEqualIID(riid, IID_ITraceObject)) {
    *ppvObject = this;
    return E_TRACE_ALREADY_SET;
  }
  auto hr = ((IUnknown *)tsfObject_)->QueryInterface(IID_ITraceObject, ppvObject);
  bool isTraceObject = hr == E_TRACE_ALREADY_SET;

  hr = ((IUnknown *)tsfObject_)->QueryInterface(riid, ppvObject);
  auto logContent =
      formatLog(LogType::TextService, "IUnknown", "QueryInterface", "({})->0x{:x}", getIIDName(riid), ul(hr));
  if (hr == S_OK) {
    if (isTraceObject) {
      // do nothing
    } else if (isIIDSupported(riid)) {
      *ppvObject = castAs(riid, new ProxyObject(*ppvObject));
    } else {
      logContent += "!!UNSUPPORTED_INTERFACE";
    }
  }
  AddRef();
  OutputDebugStringA(logContent.c_str());
  return hr;
}
STDAPI_(ULONG) ProxyObject::AddRef() { return ((IUnknown *)tsfObject_)->AddRef(); }
STDAPI_(ULONG) ProxyObject::Release() { return ((IUnknown *)tsfObject_)->Release(); }

/* ITfTextInputProcessor */
STDAPI ProxyObject::Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId) {
  auto logContent =
      fmt::format("TSFSPY: [T]ITfTextInputProcessor::Activate(0x{:x}, {:x})", (uint64_t)pThreadMgr, tfClientId);
  OutputDebugStringA(logContent.c_str());
  pThreadMgr = new TraceObject(pThreadMgr, "ITfThreadMgr");
  return ((ITfTextInputProcessor *)tsfObject_)->Activate(pThreadMgr, tfClientId);
}
STDAPI ProxyObject::Deactivate() {
  OutputDebugStringA("TSFSPY: [T]ITfTextInputProcessor::Deactivate()");
  return ((ITfTextInputProcessor *)tsfObject_)->Deactivate();
}

/* ITfTextInputProcessorEx */
STDAPI ProxyObject::ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags) {
  auto logContent = fmt::format("TSFSPY: [T]ITfTextInputProcessorEx::ActivateEx(0x{:x}, {:x}, {:x})",
                                (uint64_t)pThreadMgr, tfClientId, dwFlags);
  OutputDebugStringA(logContent.c_str());
  pThreadMgr = new TraceObject(pThreadMgr, "ITfThreadMgr");
  return ((ITfTextInputProcessorEx *)tsfObject_)->ActivateEx(pThreadMgr, tfClientId, dwFlags);
}

/* ITfThreadMgrEventSink */
STDAPI ProxyObject::OnInitDocumentMgr(ITfDocumentMgr *pDocMgr) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgrEventSink::OnInitDocumentMgr(0x{:x})", (uint64_t)pDocMgr);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgrEventSink *)tsfObject_)->OnInitDocumentMgr(pDocMgr);
}
STDAPI ProxyObject::OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgrEventSink::OnUninitDocumentMgr(0x{:x})", (uint64_t)pDocMgr);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgrEventSink *)tsfObject_)->OnUninitDocumentMgr(pDocMgr);
}
STDAPI ProxyObject::OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgrEventSink::OnSetFocus(0x{:x}, 0x{:x})", (uint64_t)pDocMgrFocus,
                                (uint64_t)pDocMgrPrevFocus);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgrEventSink *)tsfObject_)->OnSetFocus(pDocMgrFocus, pDocMgrPrevFocus);
}
STDAPI ProxyObject::OnPushContext(ITfContext *pContext) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgrEventSink::OnPushContext(0x{:x})", (uint64_t)pContext);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgrEventSink *)tsfObject_)->OnPushContext(pContext);
}
STDAPI ProxyObject::OnPopContext(ITfContext *pContext) {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadMgrEventSink::OnPopContext(0x{:x})", (uint64_t)pContext);
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadMgrEventSink *)tsfObject_)->OnPopContext(pContext);
}

/* ITfTextEditSink */
STDAPI ProxyObject::OnEndEdit(ITfContext *pic, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord) {
  auto logContent = fmt::format("TSFSPY: [T]ITfTextEditSink::OnEndEdit(0x{:x}, {:x}, 0x{:x})", (uint64_t)pic,
                                ecReadOnly, (uint64_t)pEditRecord);
  OutputDebugStringA(logContent.c_str());
  return ((ITfTextEditSink *)tsfObject_)->OnEndEdit(pic, ecReadOnly, pEditRecord);
}

/* ITfTextLayoutSink */
STDAPI ProxyObject::OnLayoutChange(ITfContext *pContext, TfLayoutCode lcode, ITfContextView *pContextView) {
  auto logContent = fmt::format("TSFSPY: [T]ITfTextLayoutSink::OnLayoutChange(0x{:x}, {:x}, 0x{:x})",
                                (unsigned long long)pContext, (int)lcode, (uint64_t)pContextView);
  OutputDebugStringA(logContent.c_str());
  return ((ITfTextLayoutSink *)tsfObject_)->OnLayoutChange(pContext, lcode, pContextView);
}

/* ITfKeyEventSink */
STDAPI ProxyObject::OnSetFocus(BOOL fForeground) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnSetFocus({})", fForeground);
  OutputDebugStringA(logContent.c_str());

  return ((ITfKeyEventSink *)tsfObject_)->OnSetFocus(fForeground);
}
STDAPI ProxyObject::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnTestKeyDown(0x{:x}, {:x}, {:x}, {})", (uint64_t)pContext,
                                wParam, lParam, *pfEaten);
  OutputDebugStringA(logContent.c_str());
  return ((ITfKeyEventSink *)tsfObject_)->OnTestKeyDown(pContext, wParam, lParam, pfEaten);
}
STDAPI ProxyObject::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnKeyDown(0x{:x}, {:x}, {:x}, {})", (uint64_t)pContext,
                                wParam, lParam, *pfEaten);
  OutputDebugStringA(logContent.c_str());
  return ((ITfKeyEventSink *)tsfObject_)->OnKeyDown(pContext, wParam, lParam, pfEaten);
}
STDAPI ProxyObject::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnTestKeyUp(0x{:x}, {:x}, {:x}, {})", (uint64_t)pContext,
                                wParam, lParam, *pfEaten);
  OutputDebugStringA(logContent.c_str());
  return ((ITfKeyEventSink *)tsfObject_)->OnTestKeyUp(pContext, wParam, lParam, pfEaten);
}
STDAPI ProxyObject::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnKeyUp(0x{:x}, {:x}, {:x}, {})", (uint64_t)pContext,
                                wParam, lParam, *pfEaten);
  OutputDebugStringA(logContent.c_str());
  return ((ITfKeyEventSink *)tsfObject_)->OnKeyUp(pContext, wParam, lParam, pfEaten);
}
STDAPI ProxyObject::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten) {
  auto logContent = fmt::format("TSFSPY: [T]ITfKeyEventSink::OnPreservedKey(0x{:x}, {}, {})", (uint64_t)pContext,
                                guidToString(rguid), *pfEaten);
  OutputDebugStringA(logContent.c_str());

  return ((ITfKeyEventSink *)tsfObject_)->OnPreservedKey(pContext, rguid, pfEaten);
}

// ITfThreadFocusSink
STDAPI ProxyObject::OnSetThreadFocus() {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadFocusSink::OnSetThreadFocus()");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadFocusSink *)tsfObject_)->OnSetThreadFocus();
}
STDAPI ProxyObject::OnKillThreadFocus() {
  auto logContent = fmt::format("TSFSPY: [T]ITfThreadFocusSink::OnKillThreadFocus()");
  OutputDebugStringA(logContent.c_str());
  return ((ITfThreadFocusSink *)tsfObject_)->OnKillThreadFocus();
}

/* ITfCompositionSink */
STDAPI ProxyObject::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition) {
  auto logContent = fmt::format("TSFSPY: [T]ITfCompositionSink::OnCompositionTerminated({}, 0x{:x})", ecWrite,
                                (uint64_t)pComposition);
  OutputDebugStringA(logContent.c_str());
  return ((ITfCompositionSink *)tsfObject_)->OnCompositionTerminated(ecWrite, pComposition);
}

/* ITfEditSession */
STDAPI ProxyObject::DoEditSession(TfEditCookie ec) {
  auto logContent = fmt::format("TSFSPY: [T]ITfEditSession::DoEditSession({})", ec);
  OutputDebugStringA(logContent.c_str());
  return ((ITfEditSession *)tsfObject_)->DoEditSession(ec);
}

/* ITfActiveLanguageProfileNotifySink */
STDAPI ProxyObject::OnActivated(REFCLSID clsid, REFGUID guidProfile, BOOL isActivated) {
  auto logContent = fmt::format("TSFSPY: [T]ITfActiveLanguageProfileNotifySink::OnActivated({}, {}, {})",
                                guidToString(clsid), guidToString(guidProfile), isActivated);
  OutputDebugStringA(logContent.c_str());
  return ((ITfActiveLanguageProfileNotifySink *)tsfObject_)->OnActivated(clsid, guidProfile, isActivated);
}

// ITfDisplayAttributeProvider
STDAPI ProxyObject::EnumDisplayAttributeInfo(__RPC__deref_out_opt IEnumTfDisplayAttributeInfo **ppEnum) {
  auto logContent = fmt::format("TSFSPY: [T]ITfDisplayAttributeProvider::EnumDisplayAttributeInfo()");
  OutputDebugStringA(logContent.c_str());
  return ((ITfDisplayAttributeProvider *)tsfObject_)->EnumDisplayAttributeInfo(ppEnum);
}
STDAPI ProxyObject::GetDisplayAttributeInfo(__RPC__in REFGUID guidInfo,
                                            __RPC__deref_out_opt ITfDisplayAttributeInfo **ppInfo) {
  auto logContent =
      fmt::format("TSFSPY: [T]ITfDisplayAttributeProvider::GetDisplayAttributeInfo({})", guidToString(guidInfo));
  OutputDebugStringA(logContent.c_str());
  return ((ITfDisplayAttributeProvider *)tsfObject_)->GetDisplayAttributeInfo(guidInfo, ppInfo);
}
