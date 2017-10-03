#if defined __APPLE__

#include "SpiderPlugin.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/mman.h>

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

std::uint64_t SpiderPlugin::getRAMOSX() const
{
    std::int32_t mib[2] = {CTL_HW, HW_MEMSIZE};
    std::uint64_t value = 0;
    size_t length = sizeof(value);

    if (sysctl(mib, 2, &value, &length, nullptr, 0) == -1)
    {
        return 0;
    }
    return value / (1024ull * 1024ull);
}

SystemInfos SpiderPlugin::getInfosOSX() const
{
    SystemInfos infos;

    infos.arch = Architecture::BITS_64;
    infos.os = OperatingSystem::MacOS;
    infos.pageSize = getPageSize();
    infos.ram = getRAMOSX();
    infos.nbProc = getNumberProcessors();
    return infos;
}
}
}
}
#endif