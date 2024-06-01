#include "ditto.h"

STDMETHODIMP DTfTextInputProcessor::Activate(
    /* [in] */ ITfThreadMgr *ptim,
    /* [in] */ TfClientId tid) {
  auto _ptim = ptim;
  ptim = CreateOrGetDitto<ITfThreadMgr>(ptim, IID_ITfThreadMgr, LogType::Manager);

  auto hr = ((ITfTextInputProcessor *)object_)->Activate(ptim, tid);

  log(type_, "ITfTextInputProcessor", "Activate", "({}, {})->0x{:x}", fmt::ptr(_ptim), tid, hr);
  return hr;
}

STDMETHODIMP DTfTextInputProcessor::Deactivate(void) {
  auto hr = ((ITfTextInputProcessor *)object_)->Deactivate();

  log(type_, "ITfTextInputProcessor", "Deactivate", "()->0x{:x}", hr);
  return hr;
}
