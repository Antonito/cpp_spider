#pragma once

#include <cstdint>
#include <array>

#include "Packet.h"

class MacAddress
{
public:
  static void get(spider::network::tcp::MacAddrArray &addr);

private:
  static std::int32_t getAddress(std::array<std::uint8_t, 6> &result);
#if defined           _WIN32
  static std::int32_t getAddressWindows(std::array<std::uint8_t, 6> &result);
#elif defined __APPLE__
  static std::int32_t getAddressMacOS(std::array<std::uint8_t, 6> &result);
#elif defined __linux__
  static std::int32_t getAddressLinux(std::array<std::uint8_t, 6> &result);
#else
#error "Plateform not supported"
#endif
};
