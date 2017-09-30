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

  virtual void getters() = 0;
  virtual std::size_t send() = 0;
  virtual std::size_t receive() = 0;
};
}
}
