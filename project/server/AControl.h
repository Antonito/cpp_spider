#pragma once

#include <queue>
#include <vector>
#include "IControl.h"
#include "IEventable.h"
#include "CommandCenter.h"

#include "Queue.h"
#include "Client.h"

namespace spider
{
namespace server
{
class AControl : public IControl, public IEventable
{
public:
  explicit AControl(CommandCenter const &, volatile bool const &running, std::vector<std::unique_ptr<Client>> const &clients);
  virtual ~AControl() = 0;

  AControl(AControl const &) = delete;
  AControl(AControl &&) = delete;
  AControl &operator=(AControl const &) = delete;
  AControl &operator=(AControl &&) = delete;

  // Pull an event from the command queue
  virtual bool pollEvent(Event &ev);
  // Push an event into the command queue
  virtual void sendEvent(Event &ev);

  // Push a Event into the response queue
  virtual void sendResponse(Event &ev);

  std::size_t getNbClient() const;

protected:
  CommandCenter const &m_cmdCenter;
  mt::Queue<Event> m_responseQueue;
  mt::Queue<Event> m_commandQueue;
  std::vector<CommandInfo> const &m_commands;
  volatile bool const &m_running;
  std::vector<std::unique_ptr<Client>> const &m_clients;
};
}
}
