#ifndef MENU_H
#define MENU_H

#include "app_types.h"
#include "basewin.h"

namespace Win {
	class Menu {
	public:
		typedef std::shared_ptr<Menu> Ptr;
		DWORD Display(const Win::BaseWindow* pParent, const Geom::PointInt& position, bool absoluteCoords=false);
		void CheckMenuItem(uint32_t id, bool status);
		void AddItem(uint32_t id, App::StringID str_id);
		Ptr AddSubMenu(App::StringID id);
		void AddMenuSeparator();

		Menu();
		virtual ~Menu();

		class IMenuItem;
		void RebuildMenu();
		HMENU Handle();

	private:
		typedef std::shared_ptr<IMenuItem> MenuPtr;

		int m_iCurrIndex;
		HMENU m_hMenu;

		boost::signals2::connection m_lang;

		typedef std::deque<MenuPtr> MIQueue;
		MIQueue m_items;
		typedef std::map<DWORD, bool> CheckMap;
		CheckMap m_checked;
	};
}

#endif
