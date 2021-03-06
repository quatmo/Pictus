#ifndef FOLDER_MONITOR_H
#define FOLDER_MONITOR_H

#include "folder_types.h"
#include "folder_monitor_imp.h"
#include "folder.h"

namespace IO {
	class FolderMonitor:boost::noncopyable {
	public:
		ChangeEventCallback OnEvent;

		void Start(const std::wstring& path);
		void Close();

		~FolderMonitor();

	private:
		FolderMonitorImplementation* doCreateImp(const std::wstring& path, ChangeEventCallback& c);

		std::auto_ptr<FolderMonitorImplementation> m_imp;
	};
}

#endif
