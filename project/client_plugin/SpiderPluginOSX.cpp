#if defined __APPLE__

#include "SpiderPlugin.h"

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initOSX()
{
    return true;
}

bool SpiderPlugin::deinitOSX()
{
    return false;
}
}
}
}
#endif