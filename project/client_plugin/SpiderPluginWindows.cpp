#if defined _WIN32

#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initWindows()
{
    return true;
}

bool SpiderPlugin::deinitWindows()
{
    return false;
}
}
}
}
#endif