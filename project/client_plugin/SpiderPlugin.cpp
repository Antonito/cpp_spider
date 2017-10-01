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

static spider::client::library::SpiderPlugin payload;

extern "C" spider::client::library::IPayload *getPayload()
{
    return static_cast<spider::client::library::IPayload *>(&payload);
}