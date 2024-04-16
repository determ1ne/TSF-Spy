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

class DTfUIElementMgr : public Ditto, public ITfUIElementMgr {
public:
  DTfUIElementMgr(void *object) : Ditto(object, IID_ITfUIElementMgr, LogType::TextService) {}
  virtual ~DTfUIElementMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP BeginUIElement(ITfUIElement *pElement, BOOL *pbShow, DWORD *pdwUIElementId);
  STDMETHODIMP UpdateUIElement(DWORD dwUIElementId);
  STDMETHODIMP EndUIElement(DWORD dwUIElementId);
  STDMETHODIMP GetUIElement(DWORD dwUIELementId, ITfUIElement **ppElement);
  STDMETHODIMP EnumUIElements(IEnumTfUIElements **ppEnum);
};

class DTfSource : public Ditto, public ITfSource {
public:
  DTfSource(void *object) : Ditto(object, IID_ITfSource, LogType::TextService) {}
  virtual ~DTfSource() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie);
  STDMETHODIMP UnadviseSink(DWORD dwCookie);
};

class DTfCategoryMgr : public Ditto, public ITfCategoryMgr {
public:
  DTfCategoryMgr(void *object) : Ditto(object, IID_ITfCategoryMgr, LogType::TextService) {}
  virtual ~DTfCategoryMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP RegisterCategory(REFCLSID rclsid, REFGUID rcatid, REFGUID rguid);
  STDMETHODIMP UnregisterCategory(REFCLSID rclsid, REFGUID rcatid, REFGUID rguid);
  STDMETHODIMP EnumCategoriesInItem(REFGUID rguid, IEnumGUID **ppEnum);
  STDMETHODIMP EnumItemsInCategory(REFGUID rcatid, IEnumGUID **ppEnum);
  STDMETHODIMP FindClosestCategory(REFGUID rguid, GUID *pcatid, const GUID **ppcatidList, ULONG ulCount);
  STDMETHODIMP RegisterGUIDDescription(REFCLSID rclsid, REFGUID rguid, const WCHAR *pchDesc, ULONG cch);
  STDMETHODIMP UnregisterGUIDDescription(REFCLSID rclsid, REFGUID rguid);
  STDMETHODIMP GetGUIDDescription(REFGUID rguid, BSTR *pbstrDesc);
  STDMETHODIMP RegisterGUIDDWORD(REFCLSID rclsid, REFGUID rguid, DWORD dw);
  STDMETHODIMP UnregisterGUIDDWORD(REFCLSID rclsid, REFGUID rguid);
  STDMETHODIMP GetGUIDDWORD(REFGUID rguid, DWORD *pdw);
  STDMETHODIMP RegisterGUID(REFGUID rguid, TfGuidAtom *pguidatom);
  STDMETHODIMP GetGUID(TfGuidAtom guidatom, GUID *pguid);
  STDMETHODIMP IsEqualTfGuidAtom(TfGuidAtom guidatom, REFGUID rguid, BOOL *pfEqual);
};

class DTfKeystrokeMgr : public Ditto, public ITfKeystrokeMgr {
public:
  DTfKeystrokeMgr(void *object) : Ditto(object, IID_ITfKeystrokeMgr, LogType::TextService) {}
  virtual ~DTfKeystrokeMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP AdviseKeyEventSink(TfClientId tid, ITfKeyEventSink *pSink, BOOL fForeground);
  STDMETHODIMP UnadviseKeyEventSink(TfClientId tid);
  STDMETHODIMP GetForeground(CLSID *pclsid);
  STDMETHODIMP TestKeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP TestKeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP KeyDown(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP KeyUp(WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
  STDMETHODIMP GetPreservedKey(ITfContext *pic, const TF_PRESERVEDKEY *pprekey, GUID *pguid);
  STDMETHODIMP IsPreservedKey(REFGUID rguid, const TF_PRESERVEDKEY *pprekey, BOOL *pfRegistered);
  STDMETHODIMP PreserveKey(TfClientId tid, REFGUID rguid, const TF_PRESERVEDKEY *prekey, const WCHAR *pchDesc,
                           ULONG cchDesc);
  STDMETHODIMP UnpreserveKey(REFGUID rguid, const TF_PRESERVEDKEY *pprekey);
  STDMETHODIMP SetPreservedKeyDescription(REFGUID rguid, const WCHAR *pchDesc, ULONG cchDesc);
  STDMETHODIMP GetPreservedKeyDescription(REFGUID rguid, BSTR *pbstrDesc);
  STDMETHODIMP SimulatePreservedKey(ITfContext *pic, REFGUID rguid, BOOL *pfEaten);
};

class DTfLangBarItemMgr : public Ditto, public ITfLangBarItemMgr {
public:
  DTfLangBarItemMgr(void *object) : Ditto(object, IID_ITfLangBarItemMgr, LogType::TextService) {}
  virtual ~DTfLangBarItemMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP EnumItems(IEnumTfLangBarItems **ppEnum);
  STDMETHODIMP GetItem(REFGUID rguid, ITfLangBarItem **ppItem);
  STDMETHODIMP AddItem(ITfLangBarItem *punk);
  STDMETHODIMP RemoveItem(ITfLangBarItem *punk);
  STDMETHODIMP AdviseItemSink(ITfLangBarItemSink *punk, DWORD *pdwCookie, REFGUID rguidItem);
  STDMETHODIMP UnadviseItemSink(DWORD dwCookie);
  STDMETHODIMP GetItemFloatingRect(DWORD dwThreadId, REFGUID rguid, RECT *prc);
  STDMETHODIMP GetItemsStatus(ULONG ulCount, const GUID *prgguid, DWORD *pdwStatus);
  STDMETHODIMP GetItemNum(ULONG *pulCount);
  STDMETHODIMP GetItems(ULONG ulCount, ITfLangBarItem **ppItem, TF_LANGBARITEMINFO *pInfo, DWORD *pdwStatus,
                        ULONG *pcFetched);
  STDMETHODIMP AdviseItemsSink(ULONG ulCount, ITfLangBarItemSink **ppunk, const GUID *pguidItem, DWORD *pdwCookie);
  STDMETHODIMP UnadviseItemsSink(ULONG ulCount, DWORD *pdwCookie);
};

class DTfCompartmentMgr : public Ditto, public ITfCompartmentMgr {
public:
  DTfCompartmentMgr(void *object) : Ditto(object, IID_ITfCompartmentMgr, LogType::TextService) {}
  virtual ~DTfCompartmentMgr() = default;

  DECLARE_IUNKNOWN
  STDMETHODIMP GetCompartment(REFGUID rguid, ITfCompartment **ppcomp);
  STDMETHODIMP ClearCompartment(TfClientId tid, REFGUID rguid);
  STDMETHODIMP EnumCompartments(IEnumGUID **ppEnum);
};

#undef DECLARE_IUNKNOWN

template <typename T>
T *CreateOrGetDitto(void *object, REFIID riid, Ditto::InterfaceType type) {
  return dynamic_cast<T *>(Ditto::CreateOrGet(object, riid, type));
}

bool hookCoCreateInstance();
void restoreCoCreateInstance();
