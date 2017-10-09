#include "AControl.h"

namespace spider
{
  namespace server
  {

    AControl::AControl(CommandCenter const &                       cmdCenter,
                       volatile bool const &                       running,
                       std::vector<std::unique_ptr<Client>> const &clients)
        : m_cmdCenter(cmdCenter), m_responseQueue(), m_commandQueue(),
          m_commands(m_cmdCenter.getCommand()), m_running(running),
          m_clients(clients)
    {
    }

    AControl::~AControl()
    {
    }

    bool AControl::pollEvent(Event &ev)
    {
      if (!m_commandQueue.empty())
	{
	  ev = m_commandQueue.front();
	  m_commandQueue.pop();
	  return (true);
	}
      return (false);
    }

    void AControl::sendEvent(Event &ev)
    {
      m_responseQueue.push(ev);
    }

    void AControl::sendToSpider(Event &ev)
    {
      m_commandQueue.push(ev);
    }

    std::size_t AControl::getNbClient() const
    {
      return (m_clients.size());
    }
  }
}
