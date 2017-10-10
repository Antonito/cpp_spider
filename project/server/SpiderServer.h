#pragma once

#include <vector>
#include <memory>
#include "IServer.h"
#include "AControl.h"
#include "Client.h"
#include "CommandCenter.h"
#include "TCPSocket.hpp"
#include "EventStorage.h"

#ifndef _WIN32
#include <sys/select.h>
#else
#include <windows.h>
#endif

namespace spider
{
  namespace server
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    class SpiderServer final : public IServer
    {
    public:
      explicit SpiderServer(CommandCenter &cmdCenter, volatile bool const &,
                            std::uint16_t  port);
      virtual ~SpiderServer() = default;

      SpiderServer(SpiderServer &&) = delete;
      SpiderServer &operator=(SpiderServer &&) = delete;
      SpiderServer(SpiderServer const &) = delete;
      SpiderServer &operator=(SpiderServer const &) = delete;

      virtual void run();

      void addController(AControl &controller);
      std::vector<std::unique_ptr<Client>> const &getClients() const;

    private:
      std::int32_t multiplex();
      void         treatEvents();
      bool         addClientData();
      bool         addClient();
      void removeClient(Client &cli);
      void removeClientData(::network::TCPSocket const &sock);

      bool readSize(::network::TCPSocket &sock, std::uint8_t *data,
                    size_t const size);

      ::network::IClient::ClientAction readData(::network::TCPSocket &sock);

      std::vector<AControl *>                            m_controllers;
      std::vector<std::unique_ptr<Client>>               m_clients;
      std::vector<std::unique_ptr<::network::TCPSocket>> m_clientsData;
      CommandCenter &                                    m_cmdCenter;
      volatile bool const &                              m_running;
      ::network::TCPSocket                               m_tcpSocket;
      ::network::TCPSocket                               m_tcpDataSocket;
      std::queue<Event>                                  m_commandQueue;
      std::int32_t                                       m_curClients;
      Storage                                            m_storage;

      fd_set m_readfds;
      fd_set m_writefds;
      fd_set m_exceptfds;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
