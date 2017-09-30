#pragma once

namespace spider
{
namespace server
{
class IServer
{
public:
  virtual ~IServer() = default;
  virtual void run() = 0;

protected:
  virtual void processEvent() = 0;
  virtual bool acceptClient() = 0;
  virtual void multiplex() = 0;
};
}
}
