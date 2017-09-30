//TODO: remove this
#include <iostream>
#include "GetInfo.h"

GetInfo::GetInfo() : m_name("GetInfo"), m_description("get Client's information like OS, IP geolocalisation, etc...") {
}

std::string const &GetInfo::getName() const {
  return m_name;
}

std::string const &GetInfo::getDescription() const {
  return m_description;
}

void GetInfo::command(spider::server::IClient const *, void const *) {
  // Implement your functionality here
  
  //TODO: remove this
  std::cout << "(REMOVE ME) GetInfo plugin here !" << std::endl;

  //send("/getinfo");
  //
  //on Client's side:
  //response("Windows 10; 165.4.67.84; FRANCE; DESKTOP-09876");
}

static GetInfo plugin;
// // The entry point called by the server
extern "C" spider::server::IPlugin *getPlugin()
{
  return static_cast<spider::server::IPlugin *>(&plugin);
}
