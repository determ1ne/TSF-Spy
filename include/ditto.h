#pragma once
#include "util.h"
#include <msctf.h>
#include <windows.h>

void initCtorMap();

class Ditto : public IUnknown {
public:
  using InterfaceType = LogType;

  Ditto(void *object, REFIID riid, InterfaceType type);
  virtual ~Ditto();

  static Ditto *CreateOrGet(void *object, REFIID riid, InterfaceType type);
  static bool IsIIDSupported(REFIID riid);
  /* IUnknown */
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

protected:
  void *object_;
  IID iid_;
  InterfaceType type_;

private:
  static Ditto *CreateOrGet(void *object, REFIID riid, bool checked, InterfaceType type);
};

#define DECLARE_IUNKNOWN                                                                                               \
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);                                                          \
  STDMETHODIMP_(ULONG) AddRef();                                                                                       \
  STDMETHODIMP_(ULONG) Release();

class DTfKeyEventSink : public Ditto, public ITfKeyEventSink {
public:
  DTfKeyEventSink(void *object) : Ditto(object, IID_ITfKeyEventSink, LogType::TextService) {}
  virtual ~DTfKeyEventSink() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP OnSetFocus(BOOL fForeground);
  STDMETHODIMP OnTestKeyDown(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnTestKeyUp(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnKeyDown(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnKeyUp(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP OnPreservedKey(ITfContext *pic, REFGUID rguid, BOOL *pfEaten);
};

class DTfThreadMgr : public Ditto, public ITfThreadMgr {
public:
  DTfThreadMgr(void *object) : Ditto(object, IID_ITfThreadMgr, LogType::TextService) {}
  virtual ~DTfThreadMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP Activate(TfClientId *ptid);
  STDMETHODIMP Deactivate();
  STDMETHODIMP CreateDocumentMgr(ITfDocumentMgr **ppdim);
  STDMETHODIMP EnumDocumentMgrs(IEnumTfDocumentMgrs **ppEnum);
  STDMETHODIMP GetFocus(ITfDocumentMgr **ppdimFocus);
  STDMETHODIMP SetFocus(ITfDocumentMgr *pdimFocus);
  STDMETHODIMP AssociateFocus(HWND hwnd, ITfDocumentMgr *pdimNew, ITfDocumentMgr **ppdimPrev);
  STDMETHODIMP IsThreadFocus(BOOL *pfThreadFocus);
  STDMETHODIMP GetFunctionProvider(REFCLSID clsid, ITfFunctionProvider **ppFuncProv);
  STDMETHODIMP EnumFunctionProviders(IEnumTfFunctionProviders **ppEnum);
  STDMETHODIMP GetGlobalCompartment(ITfCompartmentMgr **ppCompMgr);
};

#undef DECLARE_IUNKNOWN

template <typename T>
T *CreateOrGetDitto(void *object, REFIID riid, Ditto::InterfaceType type) {
  return dynamic_cast<T *>(Ditto::CreateOrGet(object, riid, type));
}
