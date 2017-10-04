#include <chrono>
#include <thread>
#include "WebServer.h"
#include "Logger.hpp"
#include "WebServer.h"

namespace spider
{
namespace http
{

WebServer::WebServer(server::CommandCenter const &cmdCenter, volatile bool const &running, std::uint32_t port) : AControl(cmdCenter, running), m_port(port), m_routes(), m_io_service(),
  m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port)), m_cmdCenter(cmdCenter), m_running(running)
{
  nope::log::Log(Info) << "Creating WebServer";
  nope::log::Log(Debug) << "creating custom routes for WebServer";
  for (auto const &cur : m_commands)
  {
    nope::log::Log(Info) << "route: " << cur.name;
    m_routes["/" + cur.name] = [&]() -> std::string
    {

      nope::log::Log(Info) << "route for: " << cur.name << " created";
      server::Event ev;
      //TODO: get id from route
      ev.destId = 0;
      ev.emitter = this;
      ev.commandName = cur.name;

      sendEvent(ev);

      sleep(10);
      std::stringstream res;
      std::string sHTML = "GETINFO HERE";
      res << "HTTP/1.1 200 OK" << std::endl;
      res << "content-type: text/html" << std::endl;
      res << "content-length: " << sHTML.length() << std::endl;
      res << std::endl;
      res << sHTML;
      return (res.str());
    };
  }
}

WebServer::~WebServer()
{
}

bool WebServer::pollEvent(server::Event &ev)
{
    return (AControl::pollEvent(ev));
}

void WebServer::sendResponse(server::Event const &ev)
{
    AControl::sendResponse(ev);
}

void WebServer::sendEvent(server::Event &ev)
{
    AControl::sendEvent(ev);
}

void WebServer::run()
{
  m_acceptor.listen();
  acceptClient(m_acceptor, m_io_service, m_routes);
  nope::log::Log(Info) << "WebServer started on port: " << m_port << "." << std::endl;
  //TODO: running gestion
  m_io_service.run();
}

void WebServer::acceptClient(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::io_service &io_service,
    std::map<std::string, std::function<std::string()>> &routes)
{
  std::shared_ptr<HTTPUserSession> client = std::make_shared<HTTPUserSession>(io_service, routes);
  acceptor.async_accept(client->getSocket(), [client, &acceptor, &io_service, &routes](const boost::system::error_code &e)
      {
        WebServer::acceptClient(acceptor, io_service, routes);
        if (!e)
        {
          client->sessionStart(client);
        }
        else
        {
          nope::log::Log(Error) << "cannot accept client on WebServer :/";
        }
      });
}

void WebServer::addRoute(std::string url, std::function<std::string()> x)
{
  m_routes[url] = x;
}
}
}
