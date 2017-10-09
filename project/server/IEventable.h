#pragma once

#include "Event.h"

namespace spider
{
  namespace server
  {
    class IEventable
    {
    public:
      virtual ~IEventable() = default;
      virtual void sendEvent(Event &ev) = 0;
    };
  }
}
