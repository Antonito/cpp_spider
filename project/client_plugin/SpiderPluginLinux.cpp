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
    // Get informations
    getInfosLinux();

    return true;
}

bool SpiderPlugin::deinitLinux()
{
    return false;
}

void SpiderPlugin::getInfosLinux()
{
#if INTPTR_MAX == INT64_MAX
    m_infos.pArch = ProcArchitecture::AMD64; // Assume x86_64
    m_infos.arch = Architecture::BITS_64;
#elif INTPTR_MAX == INT32_MAX
    m_infos.pArch = ProcArchitecture::x86; // Assume x86
    m_infos.arch = Architecture::BITS_32;
#endif
    m_infos.os = OperatingSystem::Linux;
    m_infos.pageSize = getPageSize();
    m_infos.nbProc = getNumberProcessors();
    m_infos.ram = getRAMLinux();
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

bool SpiderPlugin::hookKeyboardLinux() const
{

    return false;
}

bool SpiderPlugin::unHookKeyboardLinux() const
{

    return false;
}

bool SpiderPlugin::hookMouseOSX() const
{
    return false;
}

bool SpiderPlugin::unHookMouseOSX() const
{
    return false;
}

void SpiderPlugin::runLinux() const
{
}
}
}
}
#endif