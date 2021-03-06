﻿#include "StdAfx.h"
#include "settings.h"
#include "res_settings.h"

// Pages
#include "settings_page.h"
#include "dlg_view.h"
#include "dlg_interface.h"
#include "dlg_color.h"
#include "dlg_language.h"
#include "dlg_controls.h"
#include "dlg_advanced.h"
#include "dlg_cache.h"
#include "dlg_about.h"

#include "ctrl_button.h"

namespace App {
	using namespace Intl;
	using Geom::PointInt;

	void Settings::PerformOnCreate() {
		Caption(SIDSettingsWindow);
		ControlText(IDOK, SIDOK);
		ControlText(IDCANCEL, SIDCancel);
		ControlText(IDC_APPLY, SIDApply);

		CreateButton(IDOK)->OnClick.connect([this]() { OnOk(); });
		CreateButton(IDC_APPLY)->OnClick.connect([this]() { OnApply(); });
		CreateButton(IDCANCEL)->OnClick.connect([this]() { OnCancel(); });
	}


	bool Settings::PerformOnInitDialog() {
		RECT rect;
		GetWindowRect(GetDlgItem(Handle(), IDC_TREE_NAV), &rect);
		POINT pt = {rect.right, rect.top};

		ScreenToClient(Handle(), &pt);

		m_pagePos = PointInt(pt.x, pt.y);

		// Create treeview
		HTREEITEM hPrev  = 0;
		HTREEITEM hRoot  = 0;
		HTREEITEM hFirst = 0;

		for (size_t i = 0; i < m_pages.size(); ++i) {
			m_pages[i]->DoModeless(this);
			if (m_pages[i]->IsRootPage()) {
				if (hRoot != 0)
					hPrev = hRoot;

				hPrev = AddSettingsPageRoot(m_pages[i]->Caption(), static_cast<int>(i), hPrev);
				hRoot = 0;
			}
			else {
				if (hRoot == 0) {
					hRoot = hPrev;
					hPrev = 0;
				}

				hPrev = AddSettingsPageChild(m_pages[i]->Caption(), static_cast<int>(i), hRoot, hPrev);
			}

			if (hFirst == 0)
				hFirst = hPrev;
		}

		// Pick first page by default
		TreeView_SelectItem(GetDlgItem(Handle(), IDC_TREE_NAV), hFirst);
		return true;
	}

	bool Settings::PerformOnNotify(DWORD id, LPNMHDR pnmh) {
		switch(id) {
			case IDC_TREE_NAV:
				{
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
					switch(pnmh->code) {
						// Windows tends to send the wrong message, so look for either version
						case TVN_SELCHANGEDW:
						case TVN_SELCHANGEDA:
							m_pages[pnmtv->itemOld.lParam]->Show(false);
							m_pages[pnmtv->itemNew.lParam]->Show(true);
							break;
					}
					break;
				}
			default:
				return false;
		}
		return true;
	}

	PointInt Settings::GetPagePosition() {
		return m_pagePos;
	}

	void Settings::FreeResources() {
		TreeView_DeleteAllItems(GetDlgItem(Handle(), IDC_TREE_NAV));
	}

	Settings::Settings(Img::CodecFactoryStore* cfs):Win::Dialog(IDD_SETTINGS) {
		m_pages.push_back(PagePtr(new SetView));
		m_pages.push_back(PagePtr(new SetInterface));
		m_pages.push_back(PagePtr(new SetColor));
		m_pages.push_back(PagePtr(new SetLanguage));
		m_pages.push_back(PagePtr(new SetControls));
		m_pages.push_back(PagePtr(new SetAdvanced));
		m_pages.push_back(PagePtr(new SetPageCache));
		m_pages.push_back(PagePtr(new SetAbout));
	}

	HTREEITEM Settings::AddSettingsPageRoot(const std::wstring& name, int index, HTREEITEM hPrev) {
		return AddSettingsPageChild(name, index, TVI_ROOT, hPrev);
	}

	HTREEITEM Settings::AddSettingsPageChild(const std::wstring& name, int index, HTREEITEM hRoot, HTREEITEM hPrev) {
		TVINSERTSTRUCT	tvins;

		SetTextTVITEM(&tvins.item, name);
		tvins.item.mask = TVIF_TEXT | TVIF_PARAM;
		tvins.item.lParam = index;
		tvins.hInsertAfter = hPrev;
		tvins.hParent	= hRoot;

		HWND hView = GetDlgItem(Handle(), IDC_TREE_NAV);
		HTREEITEM hItem = reinterpret_cast<HTREEITEM>(SendMessage(hView, TVM_INSERTITEM, 0, (LPARAM)&tvins));
		TreeView_SetItemState(hView, hItem, TVIS_EXPANDED, TVIS_EXPANDED);
		return hItem;
	}

	bool Settings::UpdateTreeList() {
		HWND hView = GetDlgItem(Handle(), IDC_TREE_NAV);
		HTREEITEM hRoot = TreeView_GetRoot(hView);
		updateTreeItem(hRoot);
		return true;
	}

	void Settings::updateTreeItem(HTREEITEM hItem) {
		TVITEM tvi;
		HWND hView = GetDlgItem(Handle(), IDC_TREE_NAV);

		if (!hItem) return;

		tvi.hItem = hItem;
		tvi.mask = TVIF_PARAM | TVIF_HANDLE;
		TreeView_GetItem(hView, &tvi);

		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		// Make a scoped copy of the string, or TreeView_SetItem will end up playing with a deleted string.
		std::wstring name = m_pages[tvi.lParam]->Caption();
		SetTextTVITEM(&tvi, name);
		TreeView_SetItem(hView, &tvi);

		updateTreeItem(TreeView_GetChild(hView, hItem));
		updateTreeItem(TreeView_GetNextSibling(hView, hItem));
	}

	void Settings::SetTextTVITEM(TVITEM* tvi, const std::wstring &name) {
		tvi->pszText = const_cast<LPWSTR>(name.c_str());
		tvi->cchTextMax	= static_cast<int>(name.length());
	}

	void Settings::OnOk() {
		OnApply();
		FreeResources();
		DestroyWindow(Handle());
	}

	void Settings::OnApply() {
		// Store all pages data and update strings
		for (size_t i = 0; i < m_pages.size(); ++i)
			m_pages[i]->WriteSettings();

		UpdateTreeList();
		OnSettingsChanged();
	}

	void Settings::OnCancel() {
		OnSettingsChanged();
		FreeResources();
		DestroyWindow(Handle());
	}
}