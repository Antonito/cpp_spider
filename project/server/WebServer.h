#pragma once

#include <boost/asio.hpp>
#include <map>
#include <cstddef>
#include <functional>
#include "HTTPUserSession.h"
#include "IServer.h"
#include "AControl.h"
#include "CommandCenter.h"

namespace spider
{
namespace http
{
class WebServer final : public server::IServer, public server::AControl
{
  public:
    explicit WebServer(server::CommandCenter const &, volatile bool const &, std::uint32_t);
    virtual ~WebServer();
    void addRoute(std::string, std::function<std::string()> x);
    static void acceptClient(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::io_service &io_service,
        std::map<std::string, std::function<std::string()>> &routes);

  WebServer(WebServer const &) = delete;
  WebServer(WebServer &&) = delete;
  WebServer &operator=(WebServer const &) = delete;
  WebServer &operator=(WebServer &&) = delete;

  virtual bool pollEvent(server::Event &ev);
  virtual void sendResponse(server::Event const &ev);
  virtual void sendEvent(server::Event &ev);
  virtual void run();

  private:
    std::uint32_t m_port;
    std::map<std::string, std::function<std::string()>> m_routes;
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    server::CommandCenter const &m_cmdCenter;
    volatile bool const &m_running;
};
}
}
