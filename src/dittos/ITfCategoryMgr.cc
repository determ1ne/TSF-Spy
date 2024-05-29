#include "ditto.h"

STDMETHODIMP DTfCategoryMgr::RegisterCategory(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rcatid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterCategory(rclsid, rcatid, rguid);
  log(type_, "ITfCategoryMgr", "RegisterCategory", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rcatid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterCategory(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rcatid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterCategory(rclsid, rcatid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterCategory", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rcatid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::EnumCategoriesInItem(
    /* [in] */ REFGUID rguid,
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumCategoriesInItem(rguid, ppEnum);
  log(type_, "ITfCategoryMgr", "EnumCategoriesInItem", "({}, _)->0x{:x}", getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::EnumItemsInCategory(
    /* [in] */ REFGUID rcatid,
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCategoryMgr *)object_)->EnumItemsInCategory(rcatid, ppEnum);
  log(type_, "ITfCategoryMgr", "EnumItemsInCategory", "({}, _)->0x{:x}", getGUIDName(rcatid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::FindClosestCategory(
    /* [in] */ REFGUID rguid,
    /* [out] */ GUID *pcatid,
    /* [size_is][in] */ const GUID **ppcatidList,
    /* [in] */ ULONG ulCount) {
  auto hr = ((ITfCategoryMgr *)object_)->FindClosestCategory(rguid, pcatid, ppcatidList, ulCount);
  log(type_, "ITfCategoryMgr", "FindClosestCategory", "({}, {}, _, {})->0x{:x}", getGUIDName(rguid),
      getGUIDName(*pcatid), ulCount, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUIDDescription(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid,
    /* [size_is][in] */ const WCHAR *pchDesc,
    /* [in] */ ULONG cch) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDescription(rclsid, rguid, pchDesc, cch);
  log(type_, "ITfCategoryMgr", "RegisterGUIDDescription", "({}, {}, {}, {})->0x{:x}", getCLSIDName(rclsid),
      getGUIDName(rguid), rawWstr{pchDesc}, cch, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterGUIDDescription(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDescription(rclsid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterGUIDDescription", "({}, {})->0x{:x}", getCLSIDName(rclsid),
      getGUIDName(rguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUIDDescription(
    /* [in] */ REFGUID rguid,
    /* [out] */ BSTR *pbstrDesc) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDescription(rguid, pbstrDesc);
  log(type_, "ITfCategoryMgr", "GetGUIDDescription", "({}, {})->0x{:x}", getGUIDName(rguid), rawWstr{*pbstrDesc},
      ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUIDDWORD(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid,
    /* [in] */ DWORD dw) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUIDDWORD(rclsid, rguid, dw);
  log(type_, "ITfCategoryMgr", "RegisterGUIDDWORD", "({}, {}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rguid),
      dw, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::UnregisterGUIDDWORD(
    /* [in] */ REFCLSID rclsid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCategoryMgr *)object_)->UnregisterGUIDDWORD(rclsid, rguid);
  log(type_, "ITfCategoryMgr", "UnregisterGUIDDWORD", "({}, {})->0x{:x}", getCLSIDName(rclsid), getGUIDName(rguid),
      ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUIDDWORD(
    /* [in] */ REFGUID rguid,
    /* [out] */ DWORD *pdw) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUIDDWORD(rguid, pdw);
  log(type_, "ITfCategoryMgr", "GetGUIDDWORD", "({}, 0x{:x})->0x{:x}", getGUIDName(rguid), *pdw, ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::RegisterGUID(
    /* [in] */ REFGUID rguid,
    /* [out] */ TfGuidAtom *pguidatom) {
  auto hr = ((ITfCategoryMgr *)object_)->RegisterGUID(rguid, pguidatom);
  log(type_, "ITfCategoryMgr", "RegisterGUID", "({}, _)->0x{:x}", getGUIDName(rguid), *pguidatom);
  return hr;
}

STDMETHODIMP DTfCategoryMgr::GetGUID(
    /* [in] */ TfGuidAtom guidatom,
    /* [out] */ GUID *pguid) {
  auto hr = ((ITfCategoryMgr *)object_)->GetGUID(guidatom, pguid);
  log(type_, "ITfCategoryMgr", "GetGUID", "(0x{:x}, {})->0x{:x}", guidatom, getGUIDName(*pguid), ul(hr));
  return hr;
}

STDMETHODIMP DTfCategoryMgr::IsEqualTfGuidAtom(
    /* [in] */ TfGuidAtom guidatom,
    /* [in] */ REFGUID rguid,
    /* [out] */ BOOL *pfEqual) {
  auto hr = ((ITfCategoryMgr *)object_)->IsEqualTfGuidAtom(guidatom, rguid, pfEqual);
  log(type_, "ITfCategoryMgr", "IsEqualTfGuidAtom", "(0x{:x}, {}, {})->0x{:x}", guidatom, getGUIDName(rguid), *pfEqual,
      ul(hr));
  return hr;
}
