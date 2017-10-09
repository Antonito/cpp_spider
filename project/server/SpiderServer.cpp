#include <algorithm>
#include "SpiderServer.h"
#include "Logger.hpp"
#include "IClient.hpp"

namespace spider
{
  namespace server
  {
    SpiderServer::SpiderServer(CommandCenter &      cmdCenter,
                               volatile bool const &running,
                               std::uint16_t        port)
        : m_controllers(), m_clients(), m_cmdCenter(cmdCenter),
          m_running(running),
          m_tcpSocket(port, 64, network::ASocket::SocketType::BLOCKING),
          m_tcpDataSocket(port + 1, 64,
                          network::ASocket::SocketType::BLOCKING),
          m_commandQueue()
    {
      if (!m_tcpSocket.openConnection())
	{
	  throw std::runtime_error("Cannot initialize connection");
	}
    }

    void SpiderServer::addController(AControl &controller)
    {
      m_controllers.push_back(&controller);
    }

    void SpiderServer::run()
    {
      nope::log::Log(Info) << "Starting Spider server";

      while (m_running)
	{
	  using namespace std::chrono_literals;

	  // Get commands
	  for (auto *ctrl : m_controllers)
	    {
	      Event ev;
	      ev.response.setNbClient(
	          static_cast<std::uint32_t>(m_clients.size()));
	      while (ctrl->pollEvent(ev))
		{
		  // Link to correct client
		  if (ev.destId < m_clients.size())
		    {
		      ev.dest = m_clients[ev.destId].get();
		      ev.dest->sendEvent(ev);
		    }
		}
	    }

	  // Execute event, if needed
	  for (auto &cli : m_clients)
	    {
	      cli->execute();
	    }

	  int32_t const rc = multiplex();
	  if (rc < 0)
	    {
	      break;
	    }
	  else if (rc)
	    {
	      treatEvents();
	    }
	}

      nope::log::Log(Info) << "Stopping Spider server";
    }

    std::int32_t SpiderServer::multiplex()
    {
      std::int32_t       rc = 0;
      std::int32_t const sock = m_tcpSocket.getSocket();
      std::int32_t const sockData = m_tcpDataSocket.getSocket();

      do
	{
	  std::int32_t   maxSock = (sock > sockData) ? sock : sockData;
	  struct timeval tv;

	  FD_ZERO(&m_readfds);
	  FD_ZERO(&m_writefds);
	  tv.tv_sec = 0;
	  tv.tv_usec = 50;

	  FD_SET(sock, &m_readfds);
	  FD_SET(sockData, &m_readfds);

	  for (auto const &cli : m_clients)
	    {
	      if (cli->canWrite())
		{
		  FD_SET(cli->getSocket(), &m_writefds);
		}
	      FD_SET(cli->getSocket(), &m_readfds);
	      if (maxSock < cli->getSocket())
		{
		  maxSock = cli->getSocket();
		}
	    }
	  m_exceptfds = m_readfds;
	  rc = select(maxSock + 1, &m_readfds, &m_writefds, &m_exceptfds, &tv);
	}
      while (rc == -1 && errno == EINTR);
      return (rc);
    }

    void SpiderServer::treatEvents()
    {
      if (FD_ISSET(m_tcpSocket.getSocket(), &m_readfds))
	{
	  addClient();
	}
      if (FD_ISSET(m_tcpDataSocket.getSocket(), &m_readfds))
	{
	  // TODO
	  // Add data client
	}

      for (std::vector<std::unique_ptr<Client>>::iterator ite =
               m_clients.begin();
           ite != m_clients.end();)
	{
	  bool         deleted = false;
	  Client &     cli = **ite;
	  sock_t const sock = cli.getSocket();

	  if (FD_ISSET(sock, &m_readfds))
	    {
	      // Handle input
	      network::IClient::ClientAction action;

	      nope::log::Log(Info) << "Can read from socket #" << sock;
	      action = cli.treatIncomingData();
	      if (action != network::IClient::ClientAction::SUCCESS)
		{
		  removeClient(cli);
		  deleted = true;
		}
	    }
	  if (deleted == false && FD_ISSET(sock, &m_writefds))
	    {
	      // Handle output
	      network::IClient::ClientAction action;

	      nope::log::Log(Info) << "Can write to socket #" << sock;
	      action = cli.treatOutgoingData();
	      if (action == network::IClient::ClientAction::DISCONNECT)
		{
		  removeClient(cli);
		  deleted = true;
		}
	    }
	  if (deleted == false && FD_ISSET(sock, &m_exceptfds))
	    {
	      // Handle exception
	      removeClient(cli);
	      deleted = true;
	    }

	  // Check if we deleted anything
	  if (!deleted)
	    {
	      ++ite;
	    }
	}
    }

    bool SpiderServer::addClient()
    {
      std::int32_t  rc = 0;
      sockaddr_in_t in = {};
      sock_t const  sock = m_tcpSocket.getSocket();

      nope::log::Log(Info) << "There's client to accept on socket #" << sock;
      do
	{
	  socklen_t len = sizeof(in);
	  rc = ::accept(sock, reinterpret_cast<sockaddr_t *>(&in), &len);
	}
      while (rc == -1 && errno == EINTR);

      // Check if the socket is valid
      if (rc > 0)
	{
	  m_clients.push_back(
	      std::make_unique<Client>(rc, m_cmdCenter, m_clients.size()));
	  nope::log::Log(Info)
	      << "Added client FD #" << m_clients.back()->getSocket();
	  nope::log::Log(Info)
	      << "New player connected #" << m_clients.back()->getId();
	  ++m_curClients;
	  nope::log::Log(Info)
	      << "There are now " << m_curClients << " clients.";
	  return (true);
	}
      return (false);
    }

    void SpiderServer::removeClient(Client &cli)
    {
      Client &g = static_cast<Client &>(cli);

      nope::log::Log(Info) << "Removing Client";
      nope::log::Log(Info) << "Client disconnected #" << g.getId();
      g.disconnect();

      // Remove asked element
      m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                                     [&](std::unique_ptr<Client> const &o) {
	                               return (*o == g);
	                             }),
                      m_clients.end());
      --m_curClients;
      nope::log::Log(Info) << "There are now " << m_curClients << " clients.";
    }

    std::vector<std::unique_ptr<Client>> const &
        SpiderServer::getClients() const
    {
      return (m_clients);
    }
  }
}
