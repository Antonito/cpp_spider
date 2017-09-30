#include "SpiderServer.h"
#include "Logger.hpp"

namespace spider
{
namespace server
{
SpiderServer::SpiderServer(CommandCenter &cmdCenter, volatile bool const &running, std::uint32_t port) : m_controllers(), m_clients(), m_cmdCenter(cmdCenter), m_running(running), m_io_service(), m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

void SpiderServer::addController(AControl &controller)
{
    m_controllers.push_back(&controller);
}

void SpiderServer::run()
{
    nope::log::Log(Info) << "Starting Spider server";


    startAccept();
    m_io_service.run();

    nope::log::Log(Info) << "Stopping Spider server";
}

void SpiderServer::startAccept()
{
    nope::log::Log(Info) << "new client connected !";
    Client *client = new Client(m_io_service);
    m_acceptor.async_accept(client->getSocket(),
                           boost::bind(&SpiderServer::handleAccept,
                             this, client,
                             boost::asio::placeholders::error));
}

void SpiderServer::handleAccept(Client *client,
                                 boost::system::error_code const &error)
{
    if (!error)
    {
        client->start();
        m_clients.push_back(std::move(client));
    }
    else
    {
      delete client;
    }
    startAccept();
}
}
}
