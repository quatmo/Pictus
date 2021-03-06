#ifndef VERSION_H
#define VERSION_H

#include "res_version.h"

namespace App {
	struct Version {
		int Major, Minor, Revision;
		bool operator >(const Version& rhs);
	};

	const Version CurrentVersion = {PICTUS_VER_MAJOR, PICTUS_VER_MINOR, PICTUS_VER_REVISION};
}

std::wstring ToWString(const App::Version ver);
#endif
