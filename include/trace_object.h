#pragma once
#include <msctf.h>
#include <string>
#include <windows.h>

class TraceObject :
    /*clang-format off*/
    public ITfThreadMgr,
    public ITfSource,
    public ITfKeystrokeMgr,
    public ITfCategoryMgr,
    public ITfLangBarItemMgr
/*clang-format on*/
{
public:
  TraceObject(void *object, void *proxyObject, const std::string &as)
      : object_(object), proxyObject_(proxyObject), interfaceType_(as) {}
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

  /* ITfSource */
  STDMETHODIMP AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie);
  STDMETHODIMP UnadviseSink(DWORD dwCookie);

  /* ITfKeystrokeMgr */
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

  /* ITfCategoryMgr */
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

  /* ITfLangBarItemMgr */
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

  /* other functions */
  void *castAs(REFIID riid);

private:
  void *object_;
  void *proxyObject_;
  std::string interfaceType_;
};

bool hookCoCreateInstance();
void restoreCoCreateInstance();
