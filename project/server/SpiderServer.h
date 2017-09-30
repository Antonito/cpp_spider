#pragma once

#include <vector>
#include "IServer.h"
#include "AControl.h"
#include "Client.h"
#include "CommandCenter.h"

namespace spider
{
namespace server
{
class SpiderServer final : public IServer
{
  public:
    explicit SpiderServer(CommandCenter &cmdCenter);
    virtual ~SpiderServer() = default;

    SpiderServer(SpiderServer &&) = delete;
    SpiderServer &operator=(SpiderServer &&) = delete;
    SpiderServer(SpiderServer const &) = delete;
    SpiderServer &operator=(SpiderServer const &) = delete;

    virtual void run();

    void addController(AControl &controller);

  protected:
    virtual void processEvent();
    virtual bool acceptClient();
    virtual void multiplex();

  private:
    std::vector<AControl *> m_controllers;
    std::vector<Client> m_clients;
    CommandCenter &m_cmdCenter;
};
}
}
