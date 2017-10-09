#include <cstddef>
#include <string>

namespace spider
{
  namespace server
  {
    class Response
    {
    public:
      Response();
      ~Response();

      // Response(Response const &) = delete;
      // Response(Response &&) = delete;
      // Response &operator=(Response const &) = delete;
      // Response &operator=(Response &&) = delete;

      std::uint32_t const getNbClient() const;
      void                setNbClient(std::uint32_t);
      std::string const & getResponse() const;
      void                setResponse(std::string);

    private:
      std::string   m_response;
      std::uint32_t m_nbClient;
    };
  }
}
