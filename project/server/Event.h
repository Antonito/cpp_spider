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
        IEventable const *dest;
        std::uint32_t destId;
    };
    IEventable const *emitter;
    std::string commandName;
};
}
}