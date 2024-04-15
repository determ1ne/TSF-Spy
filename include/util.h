#pragma once
#include <fmt/core.h>
#include <msctf.h>
#include <string>
#include <windows.h>

enum class LogType { System, Common, TextService, Unknown };

std::string guidToString(const GUID &guid);
std::string getIIDName(REFIID riid);
std::string getCLSIDName(REFCLSID rclsid);
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
  case LogType::Common: logTypeChar = 'C'; break;
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
