#include "Client.h"

namespace spider
{
namespace server
{

constexpr std::size_t Client::maxLength;

Client::Client(sock_t const sock, CommandCenter const &cmdCenter, std::size_t const ndx) : m_os(""), m_ip(""), m_geo(""), m_pcName(""), m_commandQueue(), m_cmdCenter(cmdCenter), m_socket(sock), m_id(static_cast<std::uint16_t>(ndx)), m_canWrite(false)
{
}

bool Client::canWrite() const
{
  return m_canWrite;
}

network::IClient::ClientAction Client::treatIncomingData()
{
  return network::IClient::ClientAction::FAILURE;
}

network::IClient::ClientAction Client::treatOutgoingData()
{
  return network::IClient::ClientAction::FAILURE;
}

bool Client::operator==(Client const &other) const
{
  if (this != &other)
  {
    return (m_socket == other.m_socket);
  }
  return (true);
}

std::uint16_t Client::getId() const
{
  return m_id;
}

void Client::disconnect()
{
  nope::log::Log(Debug) << "Client disconnected #" << getSocket();
  m_socket.closeConnection();
}

Client::~Client()
{
  nope::log::Log(Warning) << "Disconnecting client";
}

sock_t Client::getSocket() const
{
  return m_socket.getSocket();
}

void Client::start()
{
}

size_t Client::send(std::string const &buffer)
{
  return 0;
}

size_t Client::receive()
{
  return 0;
}

void Client::execute()
{
  if (!m_commandQueue.empty())
  {
    Event ev = m_commandQueue.front();
    m_commandQueue.pop();
    m_cmdCenter.execCommand(*this, ev);
  }
}

void Client::sendEvent(Event &e)
{
  m_commandQueue.push(e);
}

void Client::eventManager()
{
}

std::string const &Client::getOS()
{
  return (m_os);
}

std::string const &Client::getIP()
{
  return (m_ip);
}

std::string const &Client::getGeo()
{
  return (m_geo);
}

std::string const &Client::getName()
{
  return (m_pcName);
}

void Client::setOS(std::string os)
{
  m_os = os;
}

void Client::setIP(std::string ip)
{
  m_ip = ip;
}

void Client::setGeo(std::string geo)
{
  m_geo = geo;
}

void Client::setName(std::string pcName)
{
  m_pcName = pcName;
}
}
}
