#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ssl.hpp>

namespace cindel
{
    class ImapClient
    {
    public:
        ImapClient(
                boost::asio::io_service &ioService,
                boost::asio::ssl::context &sslContext);
        ~ImapClient() = default;
        void connect(const std::string &hostname, const std::string &port);
        void login(const std::string &username, const std::string &password);

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
        boost::asio::ip::tcp::resolver resolver;
    };
}
