#if defined _WIN32

#include <windows.h>
#include <cstdio>
#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initWindows()
{
	// Hide window
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// Check if already loaded
	HANDLE hMutex = CreateMutex(NULL, FALSE, "winSp1d3r");
	if (hMutex && GetLastError() != 0) {
		return false;
	}

    return true;
}

bool SpiderPlugin::deinitWindows()
{
	system("pause");
    return false;
}
}
}
}
#endif