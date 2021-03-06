#ifndef BASEWIN_H
#define BASEWIN_H

#include "string.h"
#include "wintypes.h"
#include "orz/intl.h"

namespace Win {
	enum WinapiWrapperMessage {
		WWM_REGISTERMODELESS = WM_APP + 100,
		WWM_UNREGISTERMODELESS,
		WWM_NOTIFICATIONAREAICON_CLICK,
	};

	class ListView;
	class EditBox;
	class ComboBox;
	class Button;

	class BaseWindow {
	public:
		typedef std::vector<std::wstring>	StringVector;

		static const wchar_t*			ClassPtrProp;

	public:
		boost::signals2::signal<bool(Win::MouseEvent)> OnMouseButtonDown;
		boost::signals2::signal<bool(Win::MouseEvent)> OnMouseButtonUp;
		boost::signals2::signal<bool(Win::MouseEvent)> OnMouseMove;
		boost::signals2::signal<bool(Win::MouseEvent)> OnMouseWheel;
		boost::signals2::signal<bool(Win::MouseEvent)> OnMouseButtonDoubleClick;

		ListView* CreateListView(const DWORD id);
		EditBox* CreateEditBox(const DWORD id);
		ComboBox* CreateComboBox(const DWORD id);
		Button* CreateButton(DWORD id);

		virtual bool Show(bool doShow);

        const int Width() const;

        const int Height() const;

		const Geom::SizeInt GetSize() const;
		Geom::RectInt ClientRect();
		Geom::PointInt PositionScreen();
		Geom::PointInt CenterPositionScreen();
		Geom::PointInt PositionToParent();
		BaseWindow* Parent();
		HWND& Handle();
		const HWND& Handle() const;

        HINSTANCE & GetHINSTANCE();
		void Redraw();
		void Update();

		void Close();

		virtual void Caption(const std::wstring& str);
		virtual void Caption(const int sidCaption);
		virtual const std::wstring Caption() const;

		void Resize(const Geom::SizeInt& newSize);

		void Move(const Geom::PointInt& newPos);
		void MoveResize(const Geom::PointInt& newPos, const Geom::SizeInt& newSize);
		void MoveResize(const Geom::RectInt& newRect);

		const Geom::RectInt WindowRect() const;

		static void ShowCursor(bool doShow);

        static bool CursorVisible();

        bool DisplayContextMenu(DWORD idMenu, const Geom::PointInt& pos, bool absoluteCoords=false);
		bool DisplayContextMenu(HMENU hMenu, const Geom::PointInt& pos, bool absoluteCoords=false);

		void QueueMessage(unsigned message, WPARAM wParam, LPARAM lParam);

		bool OnClose();
		bool OnPaint();
		bool OnEraseBackground(HDC hdc);

		bool OnContextMenu(const Geom::PointInt& pt);

		bool Onchar(uint32_t key, int count, bool pressed);

		bool OnCommand(int id, WPARAM wp, LPARAM lp);
		bool OnSysCommand(int id, LPARAM lp);
		bool OnMove(const Geom::PointInt& pt);
		bool OnSize(const Geom::SizeInt& sz);
		bool OnHScroll(WPARAM wp, LPARAM lp);
		bool OnVScroll(WPARAM wp, LPARAM lp);
		bool OnNotificationAreaIconClick(size_t id);

		bool OnApp(int index, WPARAM wp, LPARAM lp);
		bool OnCopyData(const COPYDATASTRUCT* pcds);
		void OnCreateContextMenu(HMENU hMenu);

		void OnButtonClicked(int id);
		void OnComboSelectionChanged(int id);

		static Geom::PointInt MouseCursorPos();

		const std::wstring OpenFileDialog(const wchar_t* title, const wchar_t* filters, size_t defaultFilterIndex);

		explicit BaseWindow();
		virtual ~BaseWindow();

		static LRESULT CALLBACK WndProcDelegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
        void Parent(Win::BaseWindow* pParent);

        void Handle(HWND hwnd);
		void performRedraw();

		LRESULT baseWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, bool doDefProc);

		MouseButton getButton(WPARAM wParam);
		virtual LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)=0;

		static BOOL CALLBACK DelPropProc(HWND hwnd, LPCTSTR propName, HANDLE data);

		static bool UnhandledExceptionOcurred();
		static std::wstring UnhandledExceptionDescription();

		static bool m_exceptionOcurred;
		static std::wstring m_exceptionDescription;

	private:
		void UpdateCaption();


        virtual bool PerformOnClose();
        virtual bool PerformOnPaint();
        virtual bool PerformOnEraseBackground(HDC hdc);
        virtual bool PerformOnContextMenu(const Geom::PointInt& pt);
        virtual bool PerformOnchar(uint32_t key, int count, bool pressed);
        virtual bool PerformOnCommand(int id, WPARAM wp, LPARAM lp);
        virtual bool PerformOnMove(const Geom::PointInt& pt, bool isByUser);
        virtual bool PerformOnSize(const Geom::SizeInt& sz);
        virtual bool PerformOnHScroll(WPARAM wp, LPARAM lp);
        virtual bool PerformOnVScroll(WPARAM wp, LPARAM lp);
        virtual bool PerformOnNotificationAreaIconClick(size_t id);
        virtual bool PerformOnApp(int index, WPARAM wp, LPARAM lp);
        virtual bool PerformOnCopyData(const COPYDATASTRUCT* pcds);
        virtual void PerformOnCreateContextMenu(HMENU hMenu);

        // Generic
		HWND m_hwnd;
		HINSTANCE m_hInstance;

		// Windowing system
		BaseWindow* m_pParent;

		// Window move niftyness
		bool m_isMoveByUser;

		static bool m_isCursorVisible;

		int m_sidCaption;

		boost::signals2::connection m_lang;

		BaseWindow(const BaseWindow&);
		BaseWindow& operator=(const BaseWindow& rhs);
	};

}

#endif
