#pragma once

#include "IPlugin.h" // Provided api

class Kill : public spider::server::IPlugin
{
  public:
    Kill();
    virtual ~Kill() = default;

    virtual std::string const &getName() const;
    virtual std::string const &getDescription() const;
    virtual void command(spider::server::IClient *, void const *);

  private:
    std::string const m_name;
    std::string const m_description;
};
