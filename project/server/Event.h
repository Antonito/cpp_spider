#pragma once

#include <string>
#include <cstdint>

namespace spider
{
namespace server
{

class IEventable;

struct Event final
{
    union {
        IEventable *dest;
        std::uint32_t destId;
    };
    IEventable *emitter;
    std::string commandName;
};
}
}