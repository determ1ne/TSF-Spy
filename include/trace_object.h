#include <msctf.h>
#include <string>
#include <windows.h>

class TraceObject : public ITfThreadMgr {
public:
  TraceObject(IUnknown *tsfObject, void *proxyObject, const std::string &as) : tsfObject_(tsfObject), proxyObject_(proxyObject), interfaceType_(as) {}
  virtual ~TraceObject() = default;

  /* IUnknown */
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  /* ITfThreadMgr */
  STDMETHODIMP Activate(TfClientId *ptid);
  STDMETHODIMP Deactivate(void);
  STDMETHODIMP CreateDocumentMgr(ITfDocumentMgr **ppdim);
  STDMETHODIMP EnumDocumentMgrs(IEnumTfDocumentMgrs **ppEnum);
  STDMETHODIMP GetFocus(ITfDocumentMgr **ppdimFocus);
  STDMETHODIMP SetFocus(ITfDocumentMgr *pdimFocus);
  STDMETHODIMP AssociateFocus(HWND hwnd, ITfDocumentMgr *pdimNew, ITfDocumentMgr **ppdimPrev);
  STDMETHODIMP IsThreadFocus(BOOL *pfThreadFocus);
  STDMETHODIMP GetFunctionProvider(REFCLSID clsid, ITfFunctionProvider **ppFuncProv);
  STDMETHODIMP EnumFunctionProviders(IEnumTfFunctionProviders **ppEnum);
  STDMETHODIMP GetGlobalCompartment(ITfCompartmentMgr **ppCompMgr);

private:
  IUnknown *tsfObject_;
  void *proxyObject_;
  std::string interfaceType_;
};
