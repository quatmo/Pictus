#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_about.h"
#include "version.h"

namespace App {
	using namespace Intl;

	bool SetAbout::PerformOnInitPage() {
		Caption(App::SIDAbout);

		ControlText(IDC_TEXT_ABOUT_FREEWARE, SIDAboutFreeware);
		ControlText(IDC_TEXT_ABOUT_LIBJPEG, SIDAboutLibJpeg);
		ControlText(IDC_TEXT_ABOUT_LIBPNG, SIDAboutLibPng);

		std::wstring about(TX("Version "));

		about += ToWString(CurrentVersion);

		ControlText(IDC_TEXT_ABOUT_VERSION, about.c_str());

		m_headerFont = CreateFont(128, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, \
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
			DEFAULT_PITCH | FF_SWISS, L"Microsoft Sans Serif");

		SendDlgItemMessage(Handle(), IDC_ABOUT_HEADER, WM_SETFONT, (WPARAM)m_headerFont, 0);

		return true;
	}

	SetAbout::SetAbout():App::SettingsPage(IDD_SET_ABOUT), m_headerFont(0) {}
	SetAbout::~SetAbout() {
		if (m_headerFont != 0) {
			DeleteObject(m_headerFont);
			m_headerFont = 0;
		}
	}
}
