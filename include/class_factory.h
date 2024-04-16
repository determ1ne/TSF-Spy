#pragma once
#include <windows.h>

class ClassFactory : public IClassFactory {
public:
  ClassFactory(IClassFactory *tsfClassFactory) : tsfClassFactory_(tsfClassFactory) {}
  virtual ~ClassFactory() = default;

  // IUnknown
  STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  // IClassFactory
  STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv);
  STDMETHODIMP LockServer(BOOL fLock);

private:
  IClassFactory *tsfClassFactory_{nullptr};
};