#include "ditto.h"

STDMETHODIMP DTfCompartmentMgr::GetCompartment(
    /* [in] */ REFGUID rguid,
    /* [out] */ ITfCompartment **ppcomp) {
  auto hr = ((ITfCompartmentMgr *)object_)->GetCompartment(rguid, ppcomp);
  log(type_, "ITfCompartmentMgr", "GetCompartment", "({}, _)->{:x}", guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP DTfCompartmentMgr::ClearCompartment(
    /* [in] */ TfClientId tid,
    /* [in] */ REFGUID rguid) {
  auto hr = ((ITfCompartmentMgr *)object_)->ClearCompartment(tid, rguid);
  log(type_, "ITfCompartmentMgr", "ClearCompartment", "({:x}, {})->{:x}", tid, guidToString(rguid), hr);
  return hr;
}

STDMETHODIMP DTfCompartmentMgr::EnumCompartments(
    /* [out] */ IEnumGUID **ppEnum) {
  auto hr = ((ITfCompartmentMgr *)object_)->EnumCompartments(ppEnum);
  log(type_, "ITfCompartmentMgr", "EnumCompartments", "(_)->{:x}", hr);
  return hr;
}
