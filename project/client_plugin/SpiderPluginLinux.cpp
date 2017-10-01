#if defined __linux__

#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initLinux()
{
    return true;
}

bool SpiderPlugin::deinitLinux()
{
    return false;
}
}
}
}
#endif