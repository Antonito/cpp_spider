#include "SpiderServer.h"
#include "Logger.hpp"

namespace spider
{
namespace server
{
SpiderServer::SpiderServer(CommandCenter &cmdCenter, volatile bool const &running) : m_controllers(), m_clients(), m_cmdCenter(cmdCenter), m_running(running), m_io_service()
{
}

void SpiderServer::addController(AControl &controller)
{
    m_controllers.push_back(&controller);
}

void SpiderServer::run()
{
    nope::log::Log(Info) << "Starting Spider server";

    using boostTCP = boost::asio::tcp;
    boostTCP::acceptor acceptor(m_io_service, boostTCP::endpoint(boostTCP::v4(), 12345)));

    startAccept();
    m_io_service.run();

    nope::log::Log(Info) << "Stopping Spider server";
}

void SpiderServer::startAccept()
{
    session *new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

void SpiderServer::handle_accept(session *new_session,
                                 boost::system::error_code const &error)
{
    if (!error)
    {
        new_session->start();
    }
    else
    {
        delete new_session;
    }
    start_accept();
}
}
}