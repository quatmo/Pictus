#include "StdAfx.h"

int RoundCast(float rhs) {
	return static_cast<int>(rhs + 0.5f);
}

bool IsDecimal(const std::wstring& s) {
	std::wstringstream iss(s);
	int i;
	return !(iss >> std::dec >> i).fail();
}

std::wstring ToUpper(const std::wstring& s) {
	std::wstring b(s);
	std::transform(b.begin(), b.end(), b.begin(), std::towupper);
	return b;
}

std::wstring ToLower(const std::wstring& s) {
	std::wstring b(s);
	std::transform(b.begin(), b.end(), b.begin(), std::towlower);
	return b;
}

std::wstring UTF8ToWString(const char* utf8) {
	int requiredBufferSizeInwchar_ts = MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(strlen(utf8) + 1), 0, 0);

	auto destinationBuffer = std::make_unique<wchar_t[]>(requiredBufferSizeInwchar_ts);

	MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(strlen(utf8) + 1), destinationBuffer.get(), requiredBufferSizeInwchar_ts);
	return std::wstring(destinationBuffer.get());
}

std::wstring ToWString( uint32_t i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( int i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( uint8_t i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring ToWString( const char* i ) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::wstring DoThrowBuildDescription(const wchar_t* filename, int lineNum, const wchar_t* functionName, const std::wstring& description) {
	return L"\nFile: " + ::std::wstring(filename) + L"\nLine: " + ::ToWString(lineNum) + L"\nFunction: " + ::std::wstring(functionName) + L"\n\nDescription: " + description;
}

std::string ToAString( const std::wstring& s ) {
	return std::string(s.begin(), s.end());
}
