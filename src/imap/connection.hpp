#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace cmail::imap
{
    class connection
    {
    public:
        explicit connection();
        ~connection() = default;

        connection(connection const&) = delete;
        connection& operator=(connection const&) = delete;
    private:
        std::unique_ptr<boost::asio::io_context> ctx;
        std::unique_ptr<boost::asio::ssl::context> ssl;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket; 
    };
}
