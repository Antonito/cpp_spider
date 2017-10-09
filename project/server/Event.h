#pragma once

#include <string>
#include <cstdint>
#include "Response.h"

namespace spider
{
  namespace server
  {

    class IEventable;

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif
    struct Event final
    {
      union
      {
	IEventable *  dest;
	std::uint32_t destId;
      };
      IEventable *  emitter;
      std::string   commandName;
      std::uint32_t askId;
      Response      response;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
