#include "Client.h"

namespace spider
{
namespace server
{

constexpr std::size_t Client::maxLength;

Client::Client(sock_t const sock, CommandCenter const &cmdCenter, std::size_t const ndx) : m_os(""), m_ip(""), m_geo(""), m_pcName(""), m_commandQueue(), m_cmdCenter(cmdCenter), m_socket(sock), m_id(static_cast<std::uint16_t>(ndx)), m_canWrite(false), m_outputQueue(), m_inputBuffer{Client::maxLength}
{
}

bool Client::canWrite() const
{
  return m_canWrite;
}

void Client::toggleWrite()
{
  m_canWrite = !m_canWrite;
}

network::IClient::ClientAction Client::treatIncomingData()
{
  auto ret = network::IClient::ClientAction::FAILURE;

  std::string str;
  ret = readFromNetwork(str);
  nope::log::Log(Info) << "Read: [" << m_id << "] " << str;
  return ret;
}

network::IClient::ClientAction Client::treatOutgoingData()
{
  auto ret = network::IClient::ClientAction::FAILURE;

  if (!m_outputQueue.empty())
  {
    nope::log::Log(Info) << "Writing data to client";
    ret = sendNetwork(m_outputQueue.front());
    m_outputQueue.pop();
  }
  if (ret == network::IClient::ClientAction::SUCCESS)
  {
    toggleWrite();
  }

  return ret;
}

network::IClient::ClientAction Client::readFromNetwork(std::string &str)
{
  auto ret = network::IClient::ClientAction::SUCCESS;

  // - returning infos to the shell
  // - limiting shell control

  if (m_socket.recUntil(m_inputBuffer, "\r\n") == false)
  {
    nope::log::Log(Info) << "Failed to read data [Client]";
    ret = network::IClient::ClientAction::FAILURE;
  }

  // How to store datas ? getline ? Store in a queue ?

  // /!\ TODO: Check that this is not broken
  if (!m_inputBuffer.is_linearized())
  {
    m_inputBuffer.linearize();
  }
  str = m_inputBuffer.array_one().first;
  for (std::size_t i = 0; i < str.length(); ++i)
  {
    m_inputBuffer.pop_front();
  }
  return ret;
}

network::IClient::ClientAction Client::sendNetwork(std::string const &str)
{
  auto ret = network::IClient::ClientAction::SUCCESS;

  if (m_socket.send(str.c_str(), str.length()) == false)
  {
    nope::log::Log(Debug) << "Failed to write data [Client]";
    ret = network::IClient::ClientAction::FAILURE;
  }
  return (ret);
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

void Client::send(std::string const &buffer)
{
  m_outputQueue.push(buffer);
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
  m_canWrite = true;
}

void Client::eventManager()
{
}

std::string const &Client::getOS() const
{
  return (m_os);
}

std::string const &Client::getIP() const
{
  return (m_ip);
}

std::string const &Client::getGeo() const
{
  return (m_geo);
}

std::string const &Client::getName() const
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
