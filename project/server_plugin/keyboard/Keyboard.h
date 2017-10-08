#pragma once

#include "IPlugin.h" // Provided api

class Keyboard : public spider::server::IPlugin
{
  public:
    Keyboard();
    virtual ~Keyboard() = default;

    virtual std::string const &getName() const;
    virtual std::string const &getDescription() const;
    virtual std::string const getJSON(std::string const &res) const;
    virtual void command(spider::server::IClient *, void const *);

  private:
    std::string const m_name;
    std::string const m_description;
};
