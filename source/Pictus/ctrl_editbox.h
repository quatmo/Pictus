#ifndef CTRL_EDITBOX_H
#define CTRL_EDITBOX_H

#include "control.h"
#include "orz/intl.h"

namespace Win {
	class EditBox:public Control {
	public:
		enum FilterCriteria {
			FilterNone,
			FilterInvalidFilename,
			FilterNotNumerical, 
		};

		void Filterchars(FilterCriteria criteria, int sidBalloonMessage=-1);

		std::wstring Text();
		void Text(const std::wstring& s);

	protected:
		// Block invalid construction
		EditBox(int id, HWND hwnd):Control(id, hwnd), m_balloonText(-1), m_criteria(FilterNone), m_prevEditWndProc(0) {}

		friend class BaseWindow;

	private:
		EditBox();

		bool isValidchar(char c);
		void showBalloon(const std::wstring& str);

		// Sub classing
		FilterCriteria m_criteria;
		HWND m_hOriginal;
		int m_balloonText;
		WNDPROC m_prevEditWndProc;

		static LRESULT CALLBACK	FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};
}

#endif
