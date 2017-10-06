#pragma once

#include <unordered_map>
#include "AControl.h"
#include "CommandCenter.h"
#include "Client.h"

namespace spider
{
namespace shell
{
class Shell : public server::AControl
{
  public:
    explicit Shell(server::CommandCenter const &, volatile bool const &, std::vector<std::unique_ptr<::spider::server::Client>> const &);
    virtual ~Shell() = default;

    Shell(Shell const &) = delete;
    Shell(Shell &&) = delete;
    Shell &operator=(Shell const &) = delete;
    Shell &operator=(Shell &&) = delete;

    virtual bool pollEvent(server::Event &ev);
    virtual void sendResponse(server::Event &ev);
    virtual void sendEvent(server::Event &ev);

    void run();

  private:
    void displayHelpMessage() const;
};
}
}
