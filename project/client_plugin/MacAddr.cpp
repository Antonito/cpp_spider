#include "MacAddr.h"

#include <cstring>

#if defined _WIN32
#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")
#elif defined __linux__
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stropts.h>
#endif

void MacAddress::get(std::string &addr)
{
    std::array<std::uint8_t, 6> res;

    addr = "";
    if (!getAddress(res))
    {
        std::array<char, 64 * 8> tmp;

        std::snprintf(tmp.data(), sizeof(tmp), "%02X:%02X:%02X:%02X:%02X:%02X",
                      res[0], res[1], res[2],
                      res[3], res[4], res[5]);
        addr = tmp.data();
    }
}

std::int32_t MacAddress::getAddress(std::array<std::uint8_t, 6> &result)
{
    std::int32_t rc = -1;

    result.fill(0);
#if defined _WIN32
    rc = getAddressWindows(result);
#elif defined __APPLE__
    rc = getAddressMacOS(result);
#elif defined __linux__
    rc = getAddressLinux(result);
#endif
    return rc;
}

#if defined _WIN32
std::int32_t MacAddress::getAddressWindows(std::array<std::uint8_t, 6> &result)
{
    UUID uuid;

    if (UuidCreateSequential(&uuid) == RPC_S_UUID_NO_ADDRESS)
    {
        return -1;
    }
    std::memcpy(result.data(), reinterpret_cast<char *>(uuid.Data4 + 2), 6);
    return 0;
}

#elif defined __APPLE__

std::int32_t MacAddress::getAddressMacOS(std::array<std::uint8_t, 6> &result)
{
    return -1;
}

#elif defined __linux__

std::int32_t MacAddress::getAddressLinux(std::array<std::uint8_t, 6> &result)
{
    std::int32_t ret = -1;
    std::int32_t sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        return ret;
    }

    std::array<char, 1024> buf;
    struct ifconf interfaceConf;
    interfaceConf.ifc_len = sizeof(buf);
    interfaceConf.ifc_buf = buf.data();

    // TODO: Fix
    // Retrieve the interfaces names
    ioctl(socket, SIOCGIFCONF, &interfaceConf);

    struct ifreq *interfaceList = interfaceConf.ifc_req;
    for (std::int32_t i = interfaceConf.ifc_len / sizeof(struct ifreq); --i >= 0; ++interfaceList)
    {
        struct ifreq interface;

        strcpy(interface.ifr_name, interfaceList->ifr_name);
        // Get the currents interface flags
        if (!ioctl(sock, SIOCGIFFLAGS, &interface))
        {
            if (!(interface.ifr_flags & IFF_LOOPBACK))
            {
                // Get the hardware address
                if (!ioctl(sock, SIOCGIFHWADDR, &interface))
                {
                    // We found our network interface
                    ret = 0;
                    std::memcpy(interface.ifr_hwaddr.sa_data, result.data(), sizeof(result));
                    break;
                }
            }
        }
    }

    // Shutdown the connection
    shutdown(sock, SHUT_RDWR);
    return ret;
}
#endif