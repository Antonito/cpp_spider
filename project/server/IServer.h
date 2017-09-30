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
};
}
}
