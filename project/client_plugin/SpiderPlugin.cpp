#if defined _WIN32
#include <windows.h>
#endif
#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
SpiderPlugin::SpiderPlugin()
{
}

SpiderPlugin::~SpiderPlugin()
{
}

bool SpiderPlugin::init()
{
    bool ret;
#if defined _WIN32
    ret = initWindows();
#elif defined __APPLE__
    ret = initOSX();
#elif defined __linux__
    ret = initLinux();
#endif
    return ret;
}

bool SpiderPlugin::deinit()
{
    bool ret;
#if defined _WIN32
    ret = deinitWindows();
#elif defined __APPLE__
    ret = deinitOSX();
#elif defined __linux__
    ret = deinitLinux();
#endif
    return ret;
}
}
}
}

#if defined(__linux__) || (__APPLE__)
#define SPIDER_API
#else
#define SPIDER_API __declspec(dllexport)
#endif

static spider::client::library::SpiderPlugin payload;

extern "C" {
SPIDER_API spider::client::library::IPayload *getPayload()
{
    return static_cast<spider::client::library::IPayload *>(&payload);
}

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    return (true);
}
#endif
}