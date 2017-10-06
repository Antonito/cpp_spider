#pragma once

#include <string>
#include <cstdint>
#include "Response.h"

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
    std::uint32_t askId;
    Response response;
};
}
}
