#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>

namespace cindel
{
    class ImapClient
    {
    public:
        ImapClient(boost::asio::io_service service);
        void Connect(const std::string &hostname, const std::string &port);

    private:
        std::string hostname;
        std::string port;
    };
}
