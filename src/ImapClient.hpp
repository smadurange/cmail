#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>

namespace cindel
{
    class ImapClient
    {
    public:
        ImapClient(boost::asio::io_service &ioService);
        void Connect(const std::string &hostname, const std::string &port);

    private:
        boost::asio::ip::tcp::socket socket;
        std::string hostname;
        std::string port;
    };
}
