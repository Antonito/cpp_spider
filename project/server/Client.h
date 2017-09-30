#pragma once

#include "IClient.h"
#include "IEventable.h"

namespace spider
{
namespace server
{
class Client final : public IClient, public IEventable
{
public:
  virtual size_t send();
  virtual size_t receive();

  void eventManager();

private:
// TODO: rm
#if 0
  boostAsioTCPSocket m_commandSocket;
  boostAsioTCPSocket m_shellSocket;
  boostAsioUDPSocket m_dataSocket;
#endif
};
}
}
