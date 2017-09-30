#pragma once

#include "IServer.h"
#include "AControl.h"
#include "CommandCenter.h"

namespace spider
{
namespace http
{
class WebServer final : public server::IServer, public server::AControl
{
public:
  explicit WebServer(server::CommandCenter const &, volatile bool const &);
  virtual ~WebServer() = default;

  WebServer(WebServer const &) = delete;
  WebServer(WebServer &&) = delete;
  WebServer &operator=(WebServer const &) = delete;
  WebServer &operator=(WebServer &&) = delete;

  virtual bool pollEvent(server::Event &ev);
  virtual void sendResponse(server::Event const &ev);
  virtual void sendEvent(server::Event &ev);
  virtual void run();

private:
  virtual void processEvent();
  virtual bool acceptClient();
  virtual void multiplex();
  // TODO: Boost asio types
};
}
}
