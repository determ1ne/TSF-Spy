#include "ditto.h"

STDMETHODIMP DTfTextInputProcessorEx::Activate(
    /* [in] */ ITfThreadMgr *ptim,
    /* [in] */ TfClientId tid) {
  auto _ptim = ptim;
  ptim = CreateOrGetDitto<ITfThreadMgr>(ptim, IID_ITfThreadMgr, LogType::Manager);

  auto hr = ((ITfTextInputProcessorEx *)object_)->Activate(ptim, tid);

  log(type_, "ITfTextInputProcessorEx", "Activate", "({}, {})->0x{:x}", fmt::ptr(_ptim), tid, hr);
  return hr;
}

STDMETHODIMP DTfTextInputProcessorEx::Deactivate(void) {
  auto hr = ((ITfTextInputProcessorEx *)object_)->Deactivate();

  log(type_, "ITfTextInputProcessorEx", "Deactivate", "()->0x{:x}", hr);
  return hr;
}

STDMETHODIMP DTfTextInputProcessorEx::ActivateEx(
    /* [in] */ ITfThreadMgr *ptim,
    /* [in] */ TfClientId tid,
    /* [in] */ DWORD dwFlags) {
  auto _ptim = ptim;
  ptim = CreateOrGetDitto<ITfThreadMgr>(ptim, IID_ITfThreadMgr, LogType::Manager);

  auto hr = ((ITfTextInputProcessorEx *)object_)->ActivateEx(ptim, tid, dwFlags);

  log(type_, "ITfTextInputProcessorEx", "ActivateEx", "({}, {}, {})->0x{:x}", fmt::ptr(_ptim), tid, dwFlags, hr);
  return hr;
}
