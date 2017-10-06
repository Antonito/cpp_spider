#if defined __APPLE__

#include "SpiderPlugin.h"
#include <sys/types.h>
#include <sys/sysctl.h>

namespace spider
{
namespace client
{
namespace library
{
bool SpiderPlugin::initOSX()
{
    // Get informations
    getInfosOSX();

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

void SpiderPlugin::getInfosOSX()
{
    m_infos.pArch = ProcArchitecture::AMD64; // Assume x86_64
    m_infos.arch = Architecture::BITS_64;
    m_infos.os = OperatingSystem::MacOS;
    m_infos.pageSize = getPageSize();
    m_infos.ram = getRAMOSX();
    m_infos.nbProc = getNumberProcessors();
}

bool SpiderPlugin::hookKeyboardOSX() const
{

    return false;
}

bool SpiderPlugin::unHookKeyboardOSX() const
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

void SpiderPlugin::runOSX() const
{
}
}
}
}
#endif