#pragma once

#include "Event.h"

namespace spider
{
  namespace server
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
    class IEventable
    {
    public:
      virtual ~IEventable() = default;
      virtual void sendEvent(Event &ev) = 0;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
