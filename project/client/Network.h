#pragma once

#include <memory>

#ifndef _WIN32
#include <sys/select.h>
#endif

#include "RingBuffer.h"
#include "SystemMsg.h"
#include "IPayload.h"
#include "Queue.h"
#include "TCPSocket.hpp"

namespace spider
{
  namespace client
  {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    class Network
    {
    public:
      explicit Network(
          mt::Queue<SystemMsg> &               sendToNetwork,
          mt::Queue<library::IPayload::Order> &receivedFromNetwork,
          mt::Queue<std::string> &responseQueue, std::string const &sslPath);
      ~Network();

      Network(Network const &) = delete;
      Network(Network &&) = delete;
      Network &operator=(Network const &) = delete;
      Network &operator=(Network &&) = delete;

      void run(std::uint16_t const port, std::uint16_t const portData,
               std::string const &addr, bool const isIP);

    private:
      std::int32_t multiplex(fd_set &readfds, fd_set &writefds,
                             fd_set &exceptfds);
      std::int32_t treatEvents(fd_set const &readfds, fd_set const &writefds,
                               fd_set const &exceptfds);

      std::int32_t receivedCommand();
      std::int32_t writeCommandResponse();

      std::int32_t sendMsg(SystemMsg const &msg) const;

      mt::Queue<SystemMsg> &                m_sendToNetwork;
      mt::Queue<library::IPayload::Order> & m_receivedFromNetwork;
      mt::Queue<std::string> &              m_cmdResponse;
      bool                                  m_isConnected;
      std::unique_ptr<::network::TCPSocket> m_sock;
      std::unique_ptr<::network::TCPSocket> m_sockData;
      RingBuffer<4096>                      m_cmdReceived;
      std::string const                     m_sslPath;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
