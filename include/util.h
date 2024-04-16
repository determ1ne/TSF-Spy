#pragma once
#include <fmt/core.h>
#include <msctf.h>
#include <string>
#include <windows.h>

enum class LogType { System, Manager, TextService, Unknown };

// {63D45364-7291-4789-BF12-F5F50D68FB8C}
static const GUID IID_ITraceObject = {0x63d45364, 0x7291, 0x4789, {0xbf, 0x12, 0xf5, 0xf5, 0xd, 0x68, 0xfb, 0x8c}};
constexpr HRESULT E_TRACE_ALREADY_SET = 0xDEADBEEF;

std::string guidToString(const GUID &guid);
std::string getIIDName(REFIID riid);
std::string getCLSIDName(REFCLSID rclsid);
std::string getGUIDName(REFGUID rguid);
bool shouldLogContent(LogType logType, std::string_view objectClass, std::string_view funcName);

inline std::string guidToString(const GUID *guid) {
  if (guid == nullptr) {
    return "null";
  }
  return guidToString(*guid);
}

template <typename... Args>
std::string formatLog(LogType logType, std::string_view objectClass, std::string_view funcName, const char *fmt,
                      Args... args) {
  auto content = fmt::format(fmt, args...);
  char logTypeChar = 'U';
  switch (logType) {
    /* clang-format off */
  case LogType::System: logTypeChar = 'S'; break;
  case LogType::Manager: logTypeChar = 'M'; break;
  case LogType::TextService: logTypeChar = 'T'; break;
  default: break;
    /* clang-format on */
  }
  return fmt::format("TSFSPY: [{}]{}:{}{}", logTypeChar, objectClass, funcName, content);
}

template <typename... Args>
void log(LogType logType, std::string_view objectClass, std::string_view funcName, const char *fmt, Args... args) {
  if (shouldLogContent(logType, objectClass, funcName)) {
    auto content = formatLog(logType, objectClass, funcName, fmt, args...);
    OutputDebugStringA(content.c_str());
  }
}

__forceinline unsigned long ul(long n) { return static_cast<unsigned long>(n); }
__forceinline bool intToBool(BOOL i) { return i != FALSE; }

struct rawWstr {
  const WCHAR *str;
};
std::string format_as(const rawWstr &w);
std::string format_as(const TF_PRESERVEDKEY &preKey);
