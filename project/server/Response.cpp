#include "Response.h"

namespace spider
{
namespace server
{

  Response::Response()
  {
  }

  Response::~Response()
  {
  }


  std::uint32_t const Response::getNbClient() const
  {
    return (m_nbClient);
  }

  void Response::setNbClient(std::uint32_t nb)
  {
    m_nbClient = nb;
  }

  std::string const &Response::getResponse() const
  {
    return (m_response);
  }

  void Response::setResponse(std::string res)
  {
    m_response = res;
  }
}
}
