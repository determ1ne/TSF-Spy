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
    public ITfLangBarItemMgr,
    public ITfCompartmentMgr,
    public ITfUIElementMgr,
    public ITfDocumentMgr,
    public ITfContext
/*clang-format on*/
{
public:
  TraceObject(void *object, const std::string &as) : object_(object), interfaceType_(as) {}
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

  /* ITfCompartmentMgr */
  STDMETHODIMP GetCompartment(REFGUID rguid, ITfCompartment **ppcomp);
  STDMETHODIMP ClearCompartment(TfClientId tid, REFGUID rguid);
  STDMETHODIMP EnumCompartments(IEnumGUID **ppEnum);

  /* ITfUIElementMgr */
  STDMETHODIMP BeginUIElement(ITfUIElement *pElement, BOOL *pbShow, DWORD *pdwUIElementId);
  STDMETHODIMP UpdateUIElement(DWORD dwUIElementId);
  STDMETHODIMP EndUIElement(DWORD dwUIElementId);
  STDMETHODIMP GetUIElement(DWORD dwUIELementId, ITfUIElement **ppElement);
  STDMETHODIMP EnumUIElements(IEnumTfUIElements **ppEnum);

  /* ITfDocumentMgr */
  STDMETHODIMP CreateContext(TfClientId tidOwner, DWORD dwFlags, IUnknown *punk, ITfContext **ppic,
                             TfEditCookie *pecTextStore);
  STDMETHODIMP Push(ITfContext *pic);
  STDMETHODIMP Pop(DWORD dwFlags);
  STDMETHODIMP GetTop(ITfContext **ppic);
  STDMETHODIMP GetBase(ITfContext **ppic);
  STDMETHODIMP EnumContexts(IEnumTfContexts **ppEnum);

  /* ITfContext */
  STDMETHODIMP RequestEditSession(TfClientId tid, ITfEditSession *pes, DWORD dwFlags, HRESULT *phrSession);
  STDMETHODIMP InWriteSession(TfClientId tid, BOOL *pfWriteSession);
  STDMETHODIMP GetSelection(TfEditCookie ec, ULONG ulIndex, ULONG ulCount, TF_SELECTION *pSelection, ULONG *pcFetched);
  STDMETHODIMP SetSelection(TfEditCookie ec, ULONG ulCount, const TF_SELECTION *pSelection);
  STDMETHODIMP GetStart(TfEditCookie ec, ITfRange **ppStart);
  STDMETHODIMP GetEnd(TfEditCookie ec, ITfRange **ppEnd);
  STDMETHODIMP GetActiveView(ITfContextView **ppView);
  STDMETHODIMP EnumViews(IEnumTfContextViews **ppEnum);
  STDMETHODIMP GetStatus(TF_STATUS *pdcs);
  STDMETHODIMP GetProperty(REFGUID guidProp, ITfProperty **ppProp);
  STDMETHODIMP GetAppProperty(REFGUID guidProp, ITfReadOnlyProperty **ppProp);
  STDMETHODIMP TrackProperties(const GUID **prgProp, ULONG cProp, const GUID **prgAppProp, ULONG cAppProp, ITfReadOnlyProperty **ppProperty);
  STDMETHODIMP EnumProperties(IEnumTfProperties **ppEnum);
  STDMETHODIMP GetDocumentMgr(ITfDocumentMgr **ppDm);
  STDMETHODIMP CreateRangeBackup(TfEditCookie ec, ITfRange *pRange, ITfRangeBackup **ppBackup);

  /* other functions */
  template <typename T>
  static T *newAs(REFIID riid, void *pv) {
    return reinterpret_cast<T*>((new TraceObject(pv, getIIDName(riid)))->castAs(riid));
  }
  void *castAs(REFIID riid);

private:
  void *object_;
  std::string interfaceType_;
};

bool hookCoCreateInstance();
void restoreCoCreateInstance();
