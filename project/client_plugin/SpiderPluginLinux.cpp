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
    infos.pageSize = getPageSize();
    infos.ram = getRAMLinux();
    infos.nbProc = getNumberProcessors();
    return infos;
}

std::uint64_t SpiderPlugin::getRAMLinux() const
{
    std::string token;
    std::ifstream file("/proc/meminfo");

    if (file.is_open())
    {
        while (file >> token)
        {
            if (token == "MemTotal:")
            {
                std::uint64_t mem;
                if (file >> mem)
                {
                    return mem;
                }
                break;
            }
            // ignore rest of the line
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    // Nothing found
    return 0;
}
}
}
}
#endif