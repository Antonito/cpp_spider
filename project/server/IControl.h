#pragma once

#include "Event.h"

// Useful for dynamic libraries (public API)

namespace spider
{
  namespace server
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

    class IControl
    {
    public:
      virtual ~IControl() = default;
      virtual void sendEvent(Event &ev) = 0;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
