#pragma once

#include <cstdint>

namespace spider
{
namespace client
{
namespace library
{
enum class ProcArchitecture : std::uint16_t
{
    AMD64,
    ARM,
    IntelItanium,
    x86,
    Unknown
};

enum class Architecture : std::uint8_t
{
    BITS_64,
    BITS_32,
    BITS_UNKNOWN
};

enum class OperatingSystem : std::uint8_t
{
    Windows,
    MacOS,
    Linux,
    Other
};

struct SystemInfos
{
    SystemInfos() : pArch(ProcArchitecture::Unknown),
                    arch(Architecture::BITS_UNKNOWN),
                    os(OperatingSystem::Other),
                    pageSize(0),
                    nbProc(0), ram(0) {}

    // Processor architecture
    ProcArchitecture pArch;
    // 32 or 64 bits ?
    Architecture arch;
    // Operating system
    OperatingSystem os;
    // Kernel page size
    std::uint32_t pageSize;
    // Number of physical CPUs
    std::uint16_t nbProc;
    // Installed memory
    std::uint64_t ram;
};
}
}
}