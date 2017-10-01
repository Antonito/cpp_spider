#pragma once

#include <map>
#include <string>
#include <cstddef>
#include <sstream>

namespace spider
{
namespace http
{
class HTTPHeader
{
  public:
    HTTPHeader(std::map<std::string, std::function<std::string()>> const &routes);
    ~HTTPHeader();

    HTTPHeader(HTTPHeader const &) = delete;
    HTTPHeader(HTTPHeader &&) = delete;
    HTTPHeader &operator=(HTTPHeader const &) = delete;
    HTTPHeader &operator=(HTTPHeader &&) = delete;

    std::string getResponse();
    std::uint32_t contentLength();
    void onReadHeader(std::string);
    void onReadRequestLine(std::string);

  private:
    std::string m_method;
    std::string m_url;
    std::string m_version;
    std::map<std::string, std::string> m_headers;
    std::map<std::string, std::function<std::string()>> const &m_routes;
};
}
}
