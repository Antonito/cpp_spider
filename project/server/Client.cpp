/**
 * Project Untitled
 */

#include "Client.h"

/**
 * Client implementation
 */

/**
 * @return size_t
 */
namespace spider
{
namespace server
{

  constexpr std::size_t Client::maxLength;

  Client::Client(boost::asio::io_service &io_service) : m_socket(io_service)
  {
  }

  boost::asio::ip::tcp::socket& Client::getSocket()
  {
    return m_socket;
  }

  void Client::start()
  {
    m_socket.async_read_some(boost::asio::buffer(m_buffer, maxLength),
        boost::bind(&Client::handleRead, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  void Client::handleRead(const boost::system::error_code& error,
      size_t len)
  {
    m_buffer[len] = 0;
    nope::log::Log(Info) << "received: " << m_buffer.data();
    if (!error)
    {
#if 0
      boost::asio::async_write(m_socket,
          boost::asio::buffer(m_buffer, bytes_transferred),
          boost::bind(&Client::handleWrite, this,
            boost::asio::placeholders::error));
#endif
    }
    else
    {
      delete this;
    }
  }

  void Client::handleWrite(const boost::system::error_code& error)
  {
    nope::log::Log(Info) << "write something...";
    if (!error)
    {
      m_socket.async_read_some(boost::asio::buffer(m_buffer, maxLength),
          boost::bind(&Client::handleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
  }
size_t Client::send()
{
    return 0;
}

/**
 * @return size_t
 */
size_t Client::receive()
{
    return 0;
}

void Client::sendEvent(Event &e)
{
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
