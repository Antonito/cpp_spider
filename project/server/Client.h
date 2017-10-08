#pragma once

#include <array>
#include <iostream>
#include <queue>
#include "IClient.h"
#include "IEventable.h"
#include "CommandCenter.h"
#include "Logger.hpp"
#include "TCPSocket.hpp"
#include "IClient.hpp"
#include "RingBuffer.h"

namespace spider
{
namespace server
{
class Client final : public spider::server::IClient, public IEventable
{
public:
  explicit Client(sock_t const sock, CommandCenter const &cmdCenter, std::size_t const ndx);
  virtual ~Client();

  void execute();

  virtual void send(std::string const &buffer);
  virtual size_t receive();
  virtual void sendEvent(Event &ev);

  virtual std::string const &getOS() const;
  virtual std::string const &getIP() const;
  virtual std::string const &getGeo() const;
  virtual std::string const &getName() const;

  sock_t getSocket() const;
  void setOS(std::string);
  void setIP(std::string);
  void setGeo(std::string);
  void setName(std::string);

  void eventManager();
  static constexpr std::size_t maxLength = 1024;

  void disconnect();

  bool canWrite() const;
  network::IClient::ClientAction treatIncomingData();
  network::IClient::ClientAction treatOutgoingData();
  bool operator==(Client const &other) const;
  std::uint16_t getId() const;

private:
  network::IClient::ClientAction sendNetwork(std::string const &str);
  network::IClient::ClientAction readFromNetwork(std::queue<std::string> &str);

  void toggleWrite();

  std::string m_os;
  std::string m_ip;
  std::string m_geo;
  std::string m_pcName;
  std::queue<Event> m_commandQueue;
  std::queue<Event> m_responseQueue;
  CommandCenter const &m_cmdCenter;
  network::TCPSocket m_socket;
  std::uint16_t m_id;
  bool m_canWrite;
  std::queue<std::string> m_outputQueue;
  RingBuffer<0x1000> m_receiveBuffer;
};
}
}
