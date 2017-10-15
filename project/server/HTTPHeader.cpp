#include "HTTPHeader.h"
#include "Logger.hpp"

namespace spider
{
  namespace http
  {
    HTTPHeader::HTTPHeader(
        std::map<std::string,
                 std::function<void(std::uint32_t, std::uint32_t)>> const
            &routes)
        : m_method(), m_url(), m_version(), m_headers(), m_routes(routes)
    {
    }

    HTTPHeader::~HTTPHeader()
    {
    }

    void HTTPHeader::getResponse(std::uint32_t id)
    {
      std::uint32_t     victimId = 0;
      std::stringstream res(m_url.substr(m_url.find_first_of('/', 1) + 1));
      ;
      res >> victimId;
      std::string realURL(m_url.substr(0, m_url.find("/", 1)));
      auto        search = m_routes.find(realURL);
      if (search != m_routes.end())
	{
	  nope::log::Log(Info) << "response is: " << search->first;
	  return (search->second(id, victimId));
	}
      else
	{
	  m_routes.at("/404")(id, victimId);
	}
    }

    std::uint32_t HTTPHeader::contentLength()
    {
      auto request = m_headers.find("content-length");
      if (request != m_headers.end())
	{
	  std::stringstream ss(request->second);
	  std::uint32_t     len;
	  ss >> len;
	  return (len);
	}
      return (0);
    }

    void HTTPHeader::onReadHeader(std::string line)
    {
      nope::log::Log(Debug) << "[HTTPHeader] header: " << line;

      std::stringstream ss(line);
      std::string       headerName;
      std::getline(ss, headerName, ':');
      std::string value;
      std::getline(ss, value);
      m_headers[headerName] = value;
    }

    void HTTPHeader::onReadRequestLine(std::string line)
    {
      std::stringstream ss(line);
      ss >> m_method;
      ss >> m_url;
      ss >> m_version;

      nope::log::Log(Info) << "request '" << m_method << "' on : " << m_url;
    }
  }
}
