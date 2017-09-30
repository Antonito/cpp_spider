#pragma once

#include <cstddef>

namespace spider
{
namespace server
{
class IClient
{
public:
  virtual ~IClient() = default;

  IClient(IClient const &) = default;
  IClient(IClient &&) = default;
  IClient &operator=(IClient const &) = default;
  IClient &operator=(IClient &&) = default;

  virtual void getters() = 0;
  virtual std::size_t send() = 0;
  virtual std::size_t receive() = 0;
};
}
}
