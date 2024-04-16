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

#undef DECLARE_IUNKNOWN

template <typename T>
T *CreateOrGetDitto(void *object, REFIID riid, Ditto::InterfaceType type) {
  return dynamic_cast<T *>(Ditto::CreateOrGet(object, riid, type));
}
