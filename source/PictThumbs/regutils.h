#ifndef PICTTHUMBS_REGUTILS_H
#define PICTTHUMBS_REGUTILS_H


HRESULT SetHkcrRegistryKeyAndValue(const std::wstring& subKey, const wchar_t* valueName, const wchar_t* data);
std::tuple<HRESULT, std::wstring> GetHkcrRegistryKeyAndValue(const std::wstring& subKey, const wchar_t* valueName);

#endif
