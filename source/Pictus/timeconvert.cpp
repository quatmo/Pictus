#include "StdAfx.h"
#include "timeconvert.h"

namespace App {
	FILETIME ToFILETIME(FileInt i) {
		FILETIME f;
		f.dwLowDateTime = i & 0xffffffff;
		f.dwHighDateTime = (i >> 32) & 0xffffffff;
		return f;
	}

	SYSTEMTIME ToSYSTEMTIME(FileInt date) {
		SYSTEMTIME st;
		FILETIME ft = ToFILETIME(date);
		if(FileTimeToSystemTime(&ft, &st) == false) {
			DO_THROW(Err::InvalidCall, L"Failed converting to system time.");
		}
		return st;
	}

	_Use_decl_annotations_ std::wstring FormattedDate(FileInt date) {
		SYSTEMTIME st = ToSYSTEMTIME(date);
		int len=GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, 0, 0, 0);
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[len]);
		GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, 0, tmpStr.get(), len);
		return std::wstring(tmpStr.get());
	}

	_Use_decl_annotations_ std::wstring FormattedTime(FileInt date) {
		SYSTEMTIME st = ToSYSTEMTIME(date);
		int len=GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, 0, 0, 0);
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[len]);
		GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, 0, tmpStr.get(), len);
		return std::wstring(tmpStr.get());
	}
}
