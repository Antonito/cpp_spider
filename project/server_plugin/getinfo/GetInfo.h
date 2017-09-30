#pragma once

#include "IPlugin.h" // Provided api

class GetInfo : public spider::server::IPlugin
{
public:
  GetInfo();
  virtual ~GetInfo() = default;

  virtual std::string const &getName() const;
  virtual std::string const &getDescription() const;
  virtual void command(spider::server::IClient const *, void const *);

private:
  std::string const m_name;
  std::string const m_description;
};
