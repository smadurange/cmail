#pragma once
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/error_code.hpp>

#include "ImapStatusCode.hpp"

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
        cindel::ImapStatusCode login(const std::string &username, const std::string &password);
        
    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
        boost::asio::ip::tcp::resolver resolver;

        std::string getReply(boost::system::error_code &error);
    };
}
