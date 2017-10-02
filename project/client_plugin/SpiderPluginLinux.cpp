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

SystemInfos SpiderPlugin::getInfosLinux() const
{
    SystemInfos infos;

    infos.arch = Architecture::BITS_UNKNOWN;
    infos.os = OperatingSystem::Linux;
    return infos;
}
}
}
}
#endif