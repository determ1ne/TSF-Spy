#include <string>
#include <windows.h>
#include <fmt/core.h>

std::string guidToString(const GUID &guid);
std::string getIIDName(REFIID riid);
