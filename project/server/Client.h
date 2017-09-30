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

  virtual size_t send(std::string const &buffer);
  virtual size_t receive();
  virtual void sendEvent(Event &ev);

  // TODO: omg Lucas merde !
  virtual std::string const &getOS();
  virtual std::string const &getIP();
  virtual std::string const &getGeo();
  virtual std::string const &getName();

  sock_t getSocket() const;
  void start();
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
  std::string m_os;
  std::string m_ip;
  std::string m_geo;
  std::string m_pcName;
  std::queue<Event> m_commandQueue;
  CommandCenter const &m_cmdCenter;
  network::TCPSocket m_socket;
  std::uint16_t m_id;
  bool m_canWrite;
};
}
}
