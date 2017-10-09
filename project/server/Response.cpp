#include "Response.h"

namespace spider
{
  namespace server
  {

    Response::Response() : m_response(), m_nbClient()
    {
    }

    Response::~Response()
    {
    }

    Response::Response(Response const &other)
        : m_response(other.m_response), m_nbClient(other.m_nbClient)
    {
    }

    Response &Response::operator=(Response const &other)
    {
      if (this != &other)
	{
	  m_response = other.m_response;
	  m_nbClient = other.m_nbClient;
	}
      return (*this);
    }

    std::uint32_t Response::getNbClient() const
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
