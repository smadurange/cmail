#pragma once
#include <atomic>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/error_code.hpp>

namespace cindel
{
    class ImapClient
    {
    public:
        ImapClient(boost::asio::io_service &ioService, boost::asio::ssl::context &sslContext);
        ~ImapClient() = default;
        void connect(const std::string &hostname, const std::string &port);
        bool login(const std::string &username, const std::string &password);
        std::vector<std::string>::iterator fetch(const int days);

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
        boost::asio::ip::tcp::resolver resolver;
        std::atomic<int> commandCounter{0}; 

        std::string execute(const std::string &command);
    };
}
