#include "ditto.h"

STDMETHODIMP DTfLangBarItemMgr::EnumItems(
    /* [out] */ IEnumTfLangBarItems **ppEnum) {
  auto hr = ((ITfLangBarItemMgr *)object_)->EnumItems(ppEnum);
  log(type_, "ITfLangBarItemMgr", "EnumItems", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItem(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfLangBarItem **ppItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItem(rguid, ppItem);
  log(type_, "ITfLangBarItemMgr", "GetItem", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AddItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AddItem(punk);
  log(type_, "ITfLangBarItemMgr", "AddItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::RemoveItem(
    /* [in] */ ITfLangBarItem *punk) {
  auto hr = ((ITfLangBarItemMgr *)object_)->RemoveItem(punk);
  log(type_, "ITfLangBarItemMgr", "RemoveItem", "(_)->{:x}", hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AdviseItemSink(
    /* [in] */ ITfLangBarItemSink *punk,
    /* [out] */ DWORD *pdwCookie,
    /* [in] */ REFGUID rguidItem) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemSink(punk, pdwCookie, rguidItem);
  log(type_, "ITfLangBarItemMgr", "AdviseItemSink", "({}, {:x}, {})->{:x}", fmt::ptr(punk), *pdwCookie,
      guidToString(rguidItem), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::UnadviseItemSink(
    /* [in] */ DWORD dwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemSink(dwCookie);
  log(type_, "ITfLangBarItemMgr", "UnadviseItemSink", "({:x})->{:x}", dwCookie, hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemFloatingRect(
    /* [in] */ DWORD dwThreadId,
    /* [in] */ REFGUID rguid,
    /* [out] */ RECT *prc) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemFloatingRect(dwThreadId, rguid, prc);
  log(type_, "ITfLangBarItemMgr", "GetItemFloatingRect", "({:x}, {}, _)->{:x}", dwThreadId, guidToString(rguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemsStatus(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ const GUID *prgguid,
    /* [size_is(ulCount)][out] */ DWORD *pdwStatus) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemsStatus(ulCount, prgguid, pdwStatus);
  log(type_, "ITfLangBarItemMgr", "GetItemsStatus", "({}, {}, _)->{:x}", ulCount, guidToString(prgguid), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItemNum(
    /* [out] */ ULONG *pulCount) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItemNum(pulCount);
  log(type_, "ITfLangBarItemMgr", "GetItemNum", "(_)->{:x}", *pulCount, hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::GetItems(
    /* [in] */ ULONG ulCount,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ ITfLangBarItem **ppItem,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ TF_LANGBARITEMINFO *pInfo,
    /* [length_is(ulCount)][size_is(*pcFetched)][out] */ DWORD *pdwStatus,
    /* [unique][out][in] */ ULONG *pcFetched) {
  auto hr = ((ITfLangBarItemMgr *)object_)->GetItems(ulCount, ppItem, pInfo, pdwStatus, pcFetched);
  log(type_, "ITfLangBarItemMgr", "GetItems", "({}, {}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppItem), fmt::ptr(pInfo),
      fmt::ptr(pdwStatus), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::AdviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ ITfLangBarItemSink **ppunk,
    /* [size_is(ulCount)][in] */ const GUID *pguidItem,
    /* [size_is(ulCount)][out] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->AdviseItemsSink(ulCount, ppunk, pguidItem, pdwCookie);
  log(type_, "ITfLangBarItemMgr", "AdviseItemsSink", "({}, {}, {}, _)->{:x}", ulCount, fmt::ptr(ppunk),
      guidToString(pguidItem), hr);
  return hr;
}
STDMETHODIMP DTfLangBarItemMgr::UnadviseItemsSink(
    /* [in] */ ULONG ulCount,
    /* [size_is(ulCount)][in] */ DWORD *pdwCookie) {
  auto hr = ((ITfLangBarItemMgr *)object_)->UnadviseItemsSink(ulCount, pdwCookie);
  log(type_, "ITfLangBarItemMgr", "UnadviseItemsSink", "({}, _)->{:x}", ulCount, hr);
  return hr;
}

#pragma endregion DTfLangBarItemMgr
