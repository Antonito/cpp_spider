#include "MacAddr.h"

#include <cstring>

#if defined _WIN32
#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")
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
    // TODO
    return -1;
}
#elif defined __linux__
std::int32_t MacAddress::getAddressLinux(std::array<std::uint8_t, 6> &result)
{
    // TODO
    return -1;
}
#endif