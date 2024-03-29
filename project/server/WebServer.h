#pragma once

#include <boost/asio.hpp>
#include <map>
#include <cstddef>
#include <functional>
#include "HTTPUserSession.h"
#include "IServer.h"
#include "AControl.h"
#include "CommandCenter.h"
#include "Client.h"

namespace spider
{
  namespace http
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    class WebServer final : public server::IServer, public server::AControl
    {
    public:
      explicit WebServer(
          server::CommandCenter const &, volatile bool const &, std::uint16_t,
          std::vector<std::unique_ptr<::spider::server::Client>> const &);
      virtual ~WebServer();
      void addRoute(std::string,
                    std::function<void(std::uint32_t, std::uint32_t)> x);
      static void acceptClient(
          boost::asio::ip::tcp::acceptor &acceptor,
          boost::asio::io_service &       io_service,
          std::map<std::string,
                   std::function<void(std::uint32_t, std::uint32_t)>> &routes,
          std::uint32_t &                                clientCount,
          std::vector<std::shared_ptr<HTTPUserSession>> &clients);

      WebServer(WebServer const &) = delete;
      WebServer(WebServer &&) = delete;
      WebServer &operator=(WebServer const &) = delete;
      WebServer &operator=(WebServer &&) = delete;

      virtual bool pollEvent(server::Event &ev);
      virtual void sendToSpider(server::Event &ev);
      virtual void sendEvent(server::Event &ev);
      virtual void run();

      void checkResponse();

    private:
      std::uint16_t m_port;
      std::map<std::string, std::function<void(std::uint32_t, std::uint32_t)>>
                                                    m_routes;
      boost::asio::io_service                       m_io_service;
      boost::asio::ip::tcp::acceptor                m_acceptor;
      std::vector<std::shared_ptr<HTTPUserSession>> m_clients;
      volatile bool const &                         m_running;
      std::uint32_t                                 m_clientCount;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
