#if defined __linux__

#include "SpiderPlugin.h"
#include <string>
#include <fstream>
#include <limits>

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

#if INTPTR_MAX == INT64_MAX
    infos.pArch = ProcArchitecture::AMD64; // Assume x86_64
    infos.arch = Architecture::BITS_64;
#elif INTPTR_MAX == INT32_MAX
    infos.pArch = ProcArchitecture::x86; // Assume x86
    infos.arch = Architecture::BITS_32;
#endif
    infos.os = OperatingSystem::Linux;
    infos.pageSize = getPageSize();
    infos.nbProc = getNumberProcessors();
    infos.ram = getRAMLinux();
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
                    return mem / (1024);
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