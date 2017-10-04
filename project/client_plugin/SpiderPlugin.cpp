#if defined _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#include "SpiderPlugin.h"
#include "CPUID.h"
#include <string>

namespace spider
{
namespace client
{
namespace library
{
SpiderPlugin::SpiderPlugin()
{
}

SpiderPlugin::~SpiderPlugin()
{
}

bool SpiderPlugin::init()
{
    bool ret;
#if defined _WIN32
    ret = initWindows();
#elif defined __APPLE__
    ret = initOSX();
#elif defined __linux__
    ret = initLinux();
#endif
    return ret;
}

bool SpiderPlugin::deinit()
{
    bool ret;
#if defined _WIN32
    ret = deinitWindows();
#elif defined __APPLE__
    ret = deinitOSX();
#elif defined __linux__
    ret = deinitLinux();
#endif
    return ret;
}

SystemInfos SpiderPlugin::getInfos() const
{
#if defined _WIN32
    return getInfosWindows();
#elif defined __APPLE__
    return getInfosOSX();
#elif defined __linux__
    return getInfosLinux();
#endif
}

#if defined __APPLE__ || defined __linux__

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

std::uint16_t SpiderPlugin::getNumberProcessors() const
{
    std::uint16_t nbCPUs = 0;
    char vendor[12];

    {
        CPUID cpuID(0);

        reinterpret_cast<std::uint32_t *>(vendor)[0] = cpuID.EBX();
        reinterpret_cast<std::uint32_t *>(vendor)[1] = cpuID.EDX();
        reinterpret_cast<std::uint32_t *>(vendor)[2] = cpuID.ECX();
    }
    std::string const cpuVendor = std::string(vendor, 12);

    // Get CPU features
    {
        CPUID cpuID(1);

        std::uint16_t cpuFeatures = cpuID.EDX();            // EDX
        std::uint16_t logical = (cpuID.EBX() >> 16) & 0xff; // EBX[23:16]
        nbCPUs = logical;
    }

    if (cpuVendor == "GenuineIntel")
    {
        // Get DCP cache info
        CPUID cpuID(4);

        nbCPUs = ((cpuID.EAX() >> 26) & 0x3f) + 1; // EAX[31:26] + 1
    }
    else if (cpuVendor == "AuthenticAMD")
    {
        // Get NC: Number of CPU cores - 1
        CPUID cpuID(0x80000008);

        nbCPUs = (static_cast<std::uint16_t>((cpuID.ECX() & 0xff))) + 1; // ECX[7:0] + 1
    }

    return nbCPUs;
}
#endif
}
}
}

#if defined __linux__ || defined __APPLE__
#define SPIDER_API
#else
#define SPIDER_API __declspec(dllexport)
#endif

static spider::client::library::SpiderPlugin payload;

extern "C" {
SPIDER_API spider::client::library::IPayload *getPayload()
{
    return static_cast<spider::client::library::IPayload *>(&payload);
}

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    return (true);
}
#endif
}