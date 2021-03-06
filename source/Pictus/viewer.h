
#ifndef VIEWER_H
#define VIEWER_H

#include "orz/folder_monitor.h"

#include "imagecache.h"

#include "w32_rendertarget.h"
#include "window.h"

#include "adjust.h"
#include "settings.h"
#include "viewport.h"

#include "wallpaper.h"

#include "illa/codecmgr.h"
#include "appreg.h"

#include "viewer_contextmenu.h"
#include "viewer_keyboard.h"
#include "actionmap.h"
#include "ctrl_statusbar.h"
#include "imagelist.h"

namespace App {
	class ViewPort;

	class Viewer:public Win::Window, public Img::MessageReceiver {
	public:
		static const wchar_t* ClassName;
		static const wchar_t* AppTitle;

		bool Show(bool doShow);
		void ActiveImage(Img::Image::Ptr pImage);

		explicit Viewer(Img::CodecFactoryStore* cfs, const std::wstring params=TX(""));
		~Viewer();

	private:
		bool m_attemptToShow;

		friend class ViewerContextMenu;
		friend class ViewerKeyboard;

		enum {
			KeyPanSize = 100,
			MinWindowWidth = 180,
			MinWindowHeight	= 180,
			FileSizeDivider = 10,
			ShiftSkipLength = 5,
		};

		enum StatusBarField {
			StatusZoom=0,
			StatusName,
			StatusResolution,
			StatusPosition,
			StatusProgress,
			StatusFileSize,
			StatusLastModified,
			StatusNumParts
		};

		enum RemoveOp {
			RemoveOnly,
			RemoveDelete,
			RemoveRecycle
		};

		enum {
			StatFieldZoomWidth =	50,
			StatFieldImageDimWidth= 80,
			StatFieldPosWidth =		50,
			StatFieldTimeWidth =	110,
			StatFieldFileSizeWidth=	80,
			StatFieldLastModified = 110,
		};

		void AdjustChange(int brightness, int contrast, int gamma);
		void ShowAdjust();
		void ShowSettings();
		void OpenDirectoryInExplorer();
		void SettingsChanged();
		void Sort(Img::Cacher::SortMethod m);

		bool PerformOnCreate();

		bool PerformOnWindowCreate();
		bool PerformOnClose();
		bool PerformOnchar(uint32_t key, int count, bool pressed);

		bool PerformOnSize(const Geom::SizeInt& sz);
		bool PerformOnMove(const Geom::PointInt& pt, bool byUser);
		bool PerformOnDropFiles(const StringVector& files);

		bool PerformOnApp(int index, WPARAM wParam, LPARAM lParam);

		void SetImageLocation(const std::wstring& path);

		bool PerformOnCopyData(const COPYDATASTRUCT* pcds);
		bool PerformOnCreateTaskbar();
		bool PerformOnTaskbarButton(int id);

		void UpdateImageInformation();

		std::wstring UII_MemoryUsage(FileInt size);
		std::wstring UII_LoadProgress(Img::Image::Ptr image);
		std::wstring UII_ImageResolution(Img::Image::Ptr image);
		std::wstring UII_LastModified(FileInt date);

		void ZoomIn();
		void ZoomOut();
		void ZoomToggleFullSizeDefaultZoom();
		void ZoomFullSize();

		void UpdateMemoryLimits();

		void ZoomMode(App::ZoomMode mode);

		enum ScreenMode {
			SM_Normal,
			SM_Fullscreen
		};

		void ViewportMode(ScreenMode newMode);
		ScreenMode ViewportMode() const;
		ScreenMode m_screenMode;

		void PanUp();
		void PanRight();
		void PanDown();
		void PanLeft();

		void Rotate(Filter::RotationAngle r);

		void ToStart();
		void ImagePrev(unsigned numSteps = 1);
		void ImageNext(unsigned numSteps = 1);
		void ImageRandom();
		void ToEnd();

		void RemoveImage(RemoveOp op);

		Geom::SizeInt calculateCappedImageSize( const Geom::SizeInt& imageSize, const Geom::SizeInt &windowEdges );

		void ImageChanged();

		bool ApplyWallpaper(Win::Wallpaper::Mode orientation);

		void RenameCurrent();
		void OpenFolder();

		void OnLoadMessage(Img::MessageReceiver::LoadMessage s, Img::Image* pImage, const std::wstring& desc);
		void FolderEvent(IO::FileEvent e);

		// Disable copy and assignment
		Viewer(const Viewer&);

		void ShowContextMenu(Win::MouseEvent e);
		Viewer& operator=(const Viewer& rhs);

		struct CacheNotification {
			Img::Image*				image;
			Img::MessageReceiver::LoadMessage message;
			std::wstring					desc;
		};

		void AnchorTL(const Geom::PointInt& pt);
		Geom::PointInt AnchorTL() const;
		void AnchorCenter(const Geom::PointInt& pt);
		Geom::PointInt AnchorCenter() const;

		void AddNotification(const IO::FileEvent& notification);
		void HandleFolderNotification();

		void AddNotification(const CacheNotification& notification);
		void HandleCacheNotification();

		Geom::PointInt calculateWindowTopLeft( ResizePositionMethod method, const Geom::SizeInt &newSize );
		Geom::SizeInt calculateImageSize( ResizeBehaviour mode, float xratio, float yratio, const Geom::SizeInt &imageSize, const Geom::SizeInt &windowEdges );

		void ToggleFullscreenMode();

		std::wstring m_sDirectory;

		boost::signals2::connection m_lang;

		LONG_PTR m_previousWindowStyle;
		Geom::RectInt m_previousWindowRegion;
		Geom::RectInt m_previousNonMaximizedWindowRegion;

		// Cacher
		Img::Cacher m_cacher;
		IO::Folder m_folder;
		IO::FolderMonitor m_folderMonitor;

		// Window size and such
		bool m_doMaximize;

		// Settings, adjust, stuff like that
		Settings::Ptr m_settings;
		Adjust m_adjust;

		ViewPort m_viewPort;

		Win::StatusBar::Ptr m_statusBar;
		Win::StatusBarPart m_statusParts[StatusNumParts];

		std::mutex m_mutexNotification;

		typedef std::list<CacheNotification> CacheNotificationList;
		CacheNotificationList m_cacheNotifications;
		typedef std::list<IO::FileEvent> FolderNotificationList;
		FolderNotificationList m_folderNotifications;

		Img::CodecFactoryStore* m_codecs;

		ViewerContextMenu m_contextMenu;
		ViewerKeyboard m_keys;
		Win::ImageList m_shellTaskbarImageList;

		HANDLE m_singleMutex;

		typedef ActionMapParam<MouseAction, Win::MouseEvent> MouseActionMap;
		MouseActionMap m_mouseMap;

	};
}

#endif
