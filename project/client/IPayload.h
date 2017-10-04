#pragma once

#include "SystemInfo.h"

namespace spider
{
namespace client
{
namespace library
{
class IPayload
{
public:
  virtual ~IPayload() = default;

  // Should initialize the payload, if needed
  virtual bool init() = 0;

  // Should de-initialize the payload, if needed
  virtual bool deinit() = 0;

  // Collect informations about the system
  virtual SystemInfos getInfos() const = 0;
};
}
}
}