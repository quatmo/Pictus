#ifndef SETTINGS_H
#define SETTINGS_H

#include "dialog.h"

namespace Img {
	class CodecFactoryStore;
}

namespace App {
	class SettingsPage;

	class Settings:public Win::Dialog {
	public:
		boost::signals2::signal<void()> OnSettingsChanged;

		Geom::PointInt GetPagePosition();

		Settings(Img::CodecFactoryStore* cfs);

		typedef std::shared_ptr<Settings> Ptr;

	private:
		void OnOk();
		void OnApply();
		void OnCancel();

		bool PerformOnInitDialog();
		bool PerformOnNotify(DWORD id, LPNMHDR pnmh);
		void PerformOnCreate();

		void FreeResources();

		HTREEITEM AddSettingsPageRoot(const std::wstring& name, int index, HTREEITEM hPrev);
		HTREEITEM AddSettingsPageChild(const std::wstring& name, int index, HTREEITEM hRoot, HTREEITEM hPrev);

		void SetTextTVITEM(TVITEM* tvi, const std::wstring &name);
		bool UpdateTreeList();
		void updateTreeItem(HTREEITEM);

		typedef std::shared_ptr<App::SettingsPage> PagePtr;
		typedef std::vector<PagePtr> PageList;
		PageList m_pages;
		Geom::PointInt m_pagePos;

	};
}

#endif
