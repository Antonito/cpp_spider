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

std::uint32_t SpiderPlugin::getPageSize() const
{
    // We're not using bitshifts -> undefined behavior
    for (std::uint32_t n = 1; n; n *= 2)
    {
        // Allocate a memory page
        void *p = mmap(0, n * 2, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

        // If any error, stop everything
        if (p == MAP_FAILED)
        {
            break;
        }

        // Unmap datas
        std::int32_t u = munmap(reinterpret_cast<char *>(p) + n, n);
        munmap(p, n * 2);
        if (!u)
        {
            // We found the page size
            return n;
        }
    }
    return 0;
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

    // TODO: Use intrinsec cpuid
    infos.arch = Architecture::BITS_64;
    infos.os = OperatingSystem::MacOS;
    infos.pageSize = getPageSize();
    infos.ram = getRAMOSX();
    return infos;
}
}
}
}
#endif