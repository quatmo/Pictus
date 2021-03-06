#include "StdAfx.h"
#include "ctrl_listview.h"

namespace Win {
	void ListView::Clear() {
		ListView_DeleteAllItems(Handle());

		for (const auto& i : m_cols)
			ListView_DeleteColumn(Handle(), i);
		m_cols.clear();

		m_index = 0;
	}

	int ListView::AddColumn(const std::wstring& name, const int width, int index) {
		LVCOLUMN lvc;
		ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT | LVCF_WIDTH;
		lvc.pszText = (wchar_t*)name.c_str(); // Doesn't write, this cast _shouldn't_ be dangerous
		lvc.cx = width;
		if (index!=0) m_cols.insert(index);
		return ListView_InsertColumn(Handle(), index, &lvc);
	}

	void ListView::ColumnName(const int index, const std::wstring& name) {
		LVCOLUMN lvc;
		ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT;
		lvc.pszText = (wchar_t*)name.c_str(); // Doesn't write, this cast _shouldn't_ be dangerous
		SendMessage(Handle(), LVM_SETCOLUMN, index, (LPARAM)&lvc);
	}

	int ListView::AddItem(const std::wstring& caption) {
		LVITEM lvi;
		ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_TEXT;
		lvi.pszText = (wchar_t*)caption.c_str();
		lvi.iItem = m_index;
		ListView_InsertItem(Handle(), &lvi);

		return m_index++;
	}

	void ListView::ItemColumn(const int row, const int column, const std::wstring& caption) {
		LVITEM lvi;
		ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_TEXT;
		lvi.pszText = (wchar_t*)caption.c_str();
		lvi.iItem = row;
		lvi.iSubItem = column;
		ListView_SetItem(Handle(), &lvi);
	}

	void ListView::Check(size_t index, bool doCheck) {
		ListView_SetCheckState(Handle(), index, doCheck);
	}

	bool ListView::IsChecked(size_t index) {
		return ListView_GetCheckState(Handle(), index)!=0;
	}

	void ListView::Style(DWORD flags) {
		ListView_SetExtendedListViewStyle(Handle(), flags);
	}
}
