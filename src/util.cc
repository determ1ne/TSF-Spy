#include "util.h"
#include "dllglobals.h"
#include <InputScope.h>
#include <MSAAText.h>
#include <ctffunc.h>
#include <ctfspui.h>
#include <ctfutb.h>
#include <msctf.h>
#include <windows.h>

#define STRINGIFY(x) #x

static const GUID IID_IDittoObject = {0x11111111, 0x2222, 0x3333, {0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb}};
static const GUID IID_ITfDisplayAttributeCollectionProvider = {
    0x3977526d, 0x1a0a, 0x435a, {0x8d, 0x06, 0xec, 0xc9, 0x51, 0x6b, 0x48, 0x4f}};

std::string guidToString(const GUID &guid) {
  char buf[40];
  sprintf_s(buf, sizeof(buf), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6],
            guid.Data4[7]);
  return std::string(buf);
}

std::string getIIDName(REFIID riid) {
  /* clang-format off */
#define IF_TYPE_RETURN(_type) if (IsEqualIID(riid, IID_##_type)) { return STRINGIFY(_type); }
  IF_TYPE_RETURN(IUnknown)
  IF_TYPE_RETURN(IClassFactory) 
  // IF_TYPE_RETURN(IAccClientDocMgr) 
  // IF_TYPE_RETURN(IAccDictionary) 
  // IF_TYPE_RETURN(IAccServerDocMgr) 
  // IF_TYPE_RETURN(IAccStore) 
  IF_TYPE_RETURN(IAnchor) 
  // IF_TYPE_RETURN(IClonableWrapper) 
  // IF_TYPE_RETURN(ICoCreateLocally) 
  // IF_TYPE_RETURN(ICoCreatedLocally) 
  // IF_TYPE_RETURN(IDocWrap) 
  IF_TYPE_RETURN(IEnumITfCompositionView) 
  IF_TYPE_RETURN(IEnumSpeechCommands) 
  IF_TYPE_RETURN(IEnumTfCandidates) 
  IF_TYPE_RETURN(IEnumTfContextViews) 
  IF_TYPE_RETURN(IEnumTfContexts) 
  IF_TYPE_RETURN(IEnumTfDisplayAttributeInfo) 
  IF_TYPE_RETURN(IEnumTfDocumentMgrs) 
  IF_TYPE_RETURN(IEnumTfFunctionProviders) 
  IF_TYPE_RETURN(IEnumTfInputProcessorProfiles) 
  IF_TYPE_RETURN(IEnumTfLangBarItems) 
  IF_TYPE_RETURN(IEnumTfLanguageProfiles) 
  IF_TYPE_RETURN(IEnumTfLatticeElements) 
  IF_TYPE_RETURN(IEnumTfProperties) 
  IF_TYPE_RETURN(IEnumTfPropertyValue) 
  IF_TYPE_RETURN(IEnumTfRanges) 
  IF_TYPE_RETURN(IEnumTfUIElements) 
  // IF_TYPE_RETURN(IInternalDocWrap) 
  IF_TYPE_RETURN(ISpeechCommandProvider) 
  IF_TYPE_RETURN(ITextStoreACP2) 
  IF_TYPE_RETURN(ITextStoreACP) 
  // IF_TYPE_RETURN(ITextStoreACPEx) 
  IF_TYPE_RETURN(ITextStoreACPServices) 
  IF_TYPE_RETURN(ITextStoreACPSink) 
  // IF_TYPE_RETURN(ITextStoreACPSinkEx) 
  IF_TYPE_RETURN(ITextStoreAnchor) 
  // IF_TYPE_RETURN(ITextStoreAnchorEx) 
  IF_TYPE_RETURN(ITextStoreAnchorSink) 
  // IF_TYPE_RETURN(ITextStoreSinkAnchorEx) 
  IF_TYPE_RETURN(ITfActiveLanguageProfileNotifySink) 
  IF_TYPE_RETURN(ITfCandidateList) 
  IF_TYPE_RETURN(ITfCandidateListUIElement) 
  IF_TYPE_RETURN(ITfCandidateListUIElementBehavior) 
  IF_TYPE_RETURN(ITfCandidateString) 
  IF_TYPE_RETURN(ITfCategoryMgr) 
  IF_TYPE_RETURN(ITfCleanupContextDurationSink) 
  IF_TYPE_RETURN(ITfCleanupContextSink) 
  IF_TYPE_RETURN(ITfClientId) 
  IF_TYPE_RETURN(ITfCompartment) 
  IF_TYPE_RETURN(ITfCompartmentEventSink) 
  IF_TYPE_RETURN(ITfCompartmentMgr) 
  IF_TYPE_RETURN(ITfComposition) 
  IF_TYPE_RETURN(ITfCompositionSink) 
  IF_TYPE_RETURN(ITfCompositionView) 
  IF_TYPE_RETURN(ITfConfigureSystemKeystrokeFeed) 
  IF_TYPE_RETURN(ITfContext) 
  IF_TYPE_RETURN(ITfContextComposition) 
  IF_TYPE_RETURN(ITfContextKeyEventSink) 
  IF_TYPE_RETURN(ITfContextOwner) 
  IF_TYPE_RETURN(ITfContextOwnerCompositionServices) 
  IF_TYPE_RETURN(ITfContextOwnerCompositionSink) 
  IF_TYPE_RETURN(ITfContextOwnerServices) 
  IF_TYPE_RETURN(ITfContextView) 
  IF_TYPE_RETURN(ITfCreatePropertyStore) 
  IF_TYPE_RETURN(ITfDisplayAttributeInfo) 
  IF_TYPE_RETURN(ITfDisplayAttributeMgr) 
  IF_TYPE_RETURN(ITfDisplayAttributeNotifySink) 
  IF_TYPE_RETURN(ITfDisplayAttributeProvider) 
  IF_TYPE_RETURN(ITfDocumentMgr) 
  IF_TYPE_RETURN(ITfEditRecord) 
  IF_TYPE_RETURN(ITfEditSession) 
  IF_TYPE_RETURN(ITfEditTransactionSink) 
  IF_TYPE_RETURN(ITfFnAdviseText) 
  IF_TYPE_RETURN(ITfFnBalloon) 
  IF_TYPE_RETURN(ITfFnConfigure) 
  IF_TYPE_RETURN(ITfFnConfigureRegisterEudc) 
  IF_TYPE_RETURN(ITfFnConfigureRegisterWord) 
  IF_TYPE_RETURN(ITfFnCustomSpeechCommand) 
  IF_TYPE_RETURN(ITfFnGetLinguisticAlternates) 
  IF_TYPE_RETURN(ITfFnGetPreferredTouchKeyboardLayout) 
  IF_TYPE_RETURN(ITfFnGetSAPIObject) 
  IF_TYPE_RETURN(ITfFnLMInternal) 
  IF_TYPE_RETURN(ITfFnLMProcessor) 
  IF_TYPE_RETURN(ITfFnLangProfileUtil) 
  IF_TYPE_RETURN(ITfFnPlayBack) 
  IF_TYPE_RETURN(ITfFnPropertyUIStatus) 
  IF_TYPE_RETURN(ITfFnReconversion) 
  IF_TYPE_RETURN(ITfFnSearchCandidateProvider) 
  IF_TYPE_RETURN(ITfFnShowHelp) 
  IF_TYPE_RETURN(ITfFunction) 
  IF_TYPE_RETURN(ITfFunctionProvider) 
  IF_TYPE_RETURN(ITfInputProcessorProfileActivationSink) 
  IF_TYPE_RETURN(ITfInputProcessorProfileMgr) 
  IF_TYPE_RETURN(ITfInputProcessorProfileSubstituteLayout) 
  IF_TYPE_RETURN(ITfInputProcessorProfiles) 
  IF_TYPE_RETURN(ITfInputProcessorProfilesEx) 
  IF_TYPE_RETURN(ITfInputScope2) 
  IF_TYPE_RETURN(ITfInputScope) 
  IF_TYPE_RETURN(ITfInsertAtSelection) 
  IF_TYPE_RETURN(ITfIntegratableCandidateListUIElement) 
  IF_TYPE_RETURN(ITfKeyEventSink) 
  IF_TYPE_RETURN(ITfKeyTraceEventSink) 
  IF_TYPE_RETURN(ITfKeystrokeMgr) 
  IF_TYPE_RETURN(ITfLMLattice) 
  IF_TYPE_RETURN(ITfLangBarEventSink) 
  IF_TYPE_RETURN(ITfLangBarItem) 
  IF_TYPE_RETURN(ITfLangBarItemBalloon) 
  IF_TYPE_RETURN(ITfLangBarItemBitmap) 
  IF_TYPE_RETURN(ITfLangBarItemBitmapButton) 
  IF_TYPE_RETURN(ITfLangBarItemButton) 
  IF_TYPE_RETURN(ITfLangBarItemMgr) 
  IF_TYPE_RETURN(ITfLangBarItemSink) 
  IF_TYPE_RETURN(ITfLangBarMgr) 
  IF_TYPE_RETURN(ITfLanguageProfileNotifySink) 
  // IF_TYPE_RETURN(ITfMSAAControl) 
  IF_TYPE_RETURN(ITfMenu) 
  IF_TYPE_RETURN(ITfMessagePump) 
  IF_TYPE_RETURN(ITfMouseSink) 
  IF_TYPE_RETURN(ITfMouseTracker) 
  IF_TYPE_RETURN(ITfMouseTrackerACP) 
  IF_TYPE_RETURN(ITfPersistentPropertyLoaderACP) 
  IF_TYPE_RETURN(ITfPreservedKeyNotifySink) 
  IF_TYPE_RETURN(ITfProperty) 
  IF_TYPE_RETURN(ITfPropertyStore) 
  IF_TYPE_RETURN(ITfQueryEmbedded) 
  IF_TYPE_RETURN(ITfRange) 
  IF_TYPE_RETURN(ITfRangeACP) 
  IF_TYPE_RETURN(ITfRangeBackup) 
  IF_TYPE_RETURN(ITfReadOnlyProperty) 
  IF_TYPE_RETURN(ITfReadingInformationUIElement) 
  IF_TYPE_RETURN(ITfReverseConversion) 
  IF_TYPE_RETURN(ITfReverseConversionList) 
  IF_TYPE_RETURN(ITfReverseConversionMgr) 
  IF_TYPE_RETURN(ITfSource) 
  IF_TYPE_RETURN(ITfSourceSingle) 
  IF_TYPE_RETURN(ITfSpeechUIServer) 
  IF_TYPE_RETURN(ITfStatusSink) 
  IF_TYPE_RETURN(ITfSystemDeviceTypeLangBarItem) 
  IF_TYPE_RETURN(ITfSystemLangBarItem) 
  IF_TYPE_RETURN(ITfSystemLangBarItemSink) 
  IF_TYPE_RETURN(ITfSystemLangBarItemText) 
  IF_TYPE_RETURN(ITfTextEditSink) 
  IF_TYPE_RETURN(ITfTextInputProcessor) 
  IF_TYPE_RETURN(ITfTextInputProcessorEx) 
  IF_TYPE_RETURN(ITfTextLayoutSink) 
  IF_TYPE_RETURN(ITfThreadFocusSink) 
  IF_TYPE_RETURN(ITfThreadMgr2) 
  IF_TYPE_RETURN(ITfThreadMgr) 
  IF_TYPE_RETURN(ITfThreadMgrEventSink) 
  IF_TYPE_RETURN(ITfThreadMgrEx) 
  IF_TYPE_RETURN(ITfToolTipUIElement) 
  IF_TYPE_RETURN(ITfTransitoryExtensionSink) 
  IF_TYPE_RETURN(ITfTransitoryExtensionUIElement) 
  IF_TYPE_RETURN(ITfUIElement) 
  IF_TYPE_RETURN(ITfUIElementMgr) 
  IF_TYPE_RETURN(ITfUIElementSink) 
  IF_TYPE_RETURN(IUIManagerEventSink) 
  // IF_TYPE_RETURN(IVersionInfo)
  
  IF_TYPE_RETURN(ITfDisplayAttributeCollectionProvider)

  return getGUIDName(riid);
#undef IF_TYPE_RETURN
  /* clang-format on */
}

std::string getCLSIDName(REFCLSID rclsid) {
#define IF_TYPE_RETURN(_type)                                                                                          \
  if (IsEqualCLSID(rclsid, CLSID_##_type)) {                                                                           \
    return STRINGIFY(_type);                                                                                           \
  }
  IF_TYPE_RETURN(TF_CategoryMgr)

  return getGUIDName(rclsid);
#undef IF_TYPE_RETURN
}

std::string getGUIDName(REFGUID rguid) { return guidToString(rguid); }

bool shouldLogContent(LogType logType, std::string_view objectClass, std::string_view funcName) { return true; }

std::string format_as(const TF_PRESERVEDKEY &preKey) {
  return fmt::format("TF_PRESERVEDKEY{{uVKey: {:x}, uModifiers: {:x}}}", preKey.uVKey, preKey.uModifiers);
}
std::string format_as(const rawWstr &w) {
  if (w.str == nullptr)
    return std::string();
  int srcLen = wcslen(w.str);
  int len = WideCharToMultiByte(CP_UTF8, 0, w.str, srcLen, 0, 0, NULL, NULL);
  std::unique_ptr<char[]> buf(new char[len + 1]);
  WideCharToMultiByte(CP_UTF8, 0, w.str, srcLen, buf.get(), len, NULL, NULL);
  return fmt::format("\"&{}\"", std::string_view(buf.get(), len));
}
