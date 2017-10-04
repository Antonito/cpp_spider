#pragma once

#include "Event.h"

// Useful for dynamic libraries (public API)

namespace spider
{
namespace server
{
class IControl
{
public:
  virtual ~IControl() = default;
  virtual void sendResponse(Event &ev) = 0;
};
}
}
