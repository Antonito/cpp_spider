#pragma once

#include <queue>
#include "IControl.h"
#include "IEventable.h"
#include "CommandCenter.h"

namespace spider
{
namespace server
{
class AControl : public IControl, public IEventable
{
public:
  explicit AControl(CommandCenter const &);
  virtual ~AControl() = default;

  AControl(AControl const &) = delete;
  AControl(AControl &&) = delete;
  AControl &operator=(AControl const &) = delete;
  AControl &operator=(AControl &&) = delete;

  // Pull an event from the command queue
  virtual bool pollEvent(Event &ev) = 0;

  // Push a Event into the response queue
  virtual void sendResponse(Event const &ev) = 0;

protected:
  CommandCenter const &m_cmdCenter;
  std::queue<Event> m_responseQueue;
  std::queue<Event> m_commandQueue;
};
}
}
