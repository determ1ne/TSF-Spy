#include "ditto.h"

STDMETHODIMP DTfSource::AdviseSink(
    /* [in] */ REFIID riid,
    /* [iid_is][in] */ IUnknown *punk,
    /* [out] */ DWORD *pdwCookie) {
  auto hr = ((ITfSource *)object_)->AdviseSink(riid, punk, pdwCookie);
  log(type_, "ITfSource", "AdviseSink", "({}, _, 0x{:x})->0x{:x}", getIIDName(riid), *pdwCookie, ul(hr));
  return hr;
}

STDMETHODIMP DTfSource::UnadviseSink(
    /* [in] */ DWORD dwCookie) {
  auto hr = ((ITfSource *)object_)->UnadviseSink(dwCookie);
  log(type_, "ITfSource", "UnadviseSink", "(0x{:x})->0x{:x}", dwCookie, ul(hr));
  return hr;
}
