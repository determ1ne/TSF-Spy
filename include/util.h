#pragma once
#include <fmt/core.h>
#include <msctf.h>
#include <string>
#include <windows.h>

std::string guidToString(const GUID &guid);
std::string getIIDName(REFIID riid);
std::string getCLSIDName(REFCLSID rclsid);

inline std::string guidToString(const GUID *guid) {
  if (guid == nullptr) {
    return "null";
  }
  return guidToString(*guid);
}
