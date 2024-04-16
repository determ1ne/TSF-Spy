#pragma once
#include <msctf.h>
#include <windows.h>

class ProxyObject : public ITfTextInputProcessorEx,
                    public ITfThreadMgrEventSink,
                    public ITfTextEditSink,
                    public ITfTextLayoutSink,
                    public ITfKeyEventSink,
                    public ITfCompositionSink,
                    public ITfThreadFocusSink,
                    public ITfActiveLanguageProfileNotifySink,
                    public ITfEditSession,
                    public ITfDisplayAttributeProvider {

public:
  ProxyObject(void *pv) : tsfObject_(pv) {}
  virtual ~ProxyObject() = default;

  /* IUnknown */
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  /* ITfTextInputProcessor */
  STDMETHODIMP Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId);
  STDMETHODIMP Deactivate();

  /* ITfTextInputProcessorEx */
  STDMETHODIMP ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags);

  /* ITfThreadMgrEventSink */
  STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr *pDocMgr);
  STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr);
  STDMETHODIMP OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus);
  STDMETHODIMP OnPushContext(ITfContext *pContext);
  STDMETHODIMP OnPopContext(ITfContext *pContext);

  /* ITfTextEditSink */
  STDMETHODIMP OnEndEdit(ITfContext *pic, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord);

  /* ITfTextLayoutSink */
  STDMETHODIMP OnLayoutChange(ITfContext *pContext, TfLayoutCode lcode, ITfContextView *pContextView);

  /* ITfKeyEventSink */
  STDMETHODIMP OnSetFocus(BOOL fForeground);
  STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten);

  // ITfThreadFocusSink
  STDMETHODIMP OnSetThreadFocus();
  STDMETHODIMP OnKillThreadFocus();

  /* ITfCompositionSink */
  STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition);

  /* ITfEditSession */
  STDMETHODIMP DoEditSession(TfEditCookie ec);

  /* ITfActiveLanguageProfileNotifySink */
  STDMETHODIMP OnActivated(REFCLSID clsid, REFGUID guidProfile, BOOL isActivated);

  // ITfDisplayAttributeProvider
  STDMETHODIMP EnumDisplayAttributeInfo(__RPC__deref_out_opt IEnumTfDisplayAttributeInfo **ppEnum);
  STDMETHODIMP GetDisplayAttributeInfo(__RPC__in REFGUID guidInfo,
                                       __RPC__deref_out_opt ITfDisplayAttributeInfo **ppInfo);

  // other functions
  void *CastAs(REFIID riid);
private:
  void *tsfObject_{nullptr};
};
