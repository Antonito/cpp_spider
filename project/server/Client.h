#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <array>
#include <iostream>
#include "IClient.h"
#include "IEventable.h"
#include "Logger.hpp"

namespace spider
{
namespace server
{
class Client final : public IClient, public IEventable
{
public:
  Client(boost::asio::io_service &);
  virtual size_t send();
  virtual size_t receive();
  virtual void getters();
  virtual void sendEvent(Event &ev);

  boost::asio::ip::tcp::socket &getSocket();
  void start();
  void handleRead(const boost::system::error_code& error, size_t bytesTransferred);
  void handleWrite(const boost::system::error_code& error);

  void eventManager();
  static constexpr std::size_t maxLength = 1024;

private:
  boost::asio::ip::tcp::socket m_socket;
  std::array<char, 1024> m_buffer;
// TODO: rm
#if 0
  boostAsioTCPSocket m_commandSocket;
  boostAsioTCPSocket m_shellSocket;
  boostAsioUDPSocket m_dataSocket;
#endif
};
}
}
