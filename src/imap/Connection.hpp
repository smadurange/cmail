#pragma once

#include <atomic>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "Response.hpp"

namespace cmail::imap
{
    class Connection
    {
    public:
        static Connection &instance();
        Connection(Connection const&) = delete;
        Connection &operator=(Connection const&) = delete;
        ~Connection() = default;
        Response open(const std::string &host, int port);
        Response close();
        Response send(const std::string &request);

    private:
        bool connected{false};
        std::mutex mtx;
        std::atomic<int> counter{0};
        boost::asio::ip::tcp::resolver resolver;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;

        Connection(boost::asio::io_context &ctx, boost::asio::ssl::context &ssl);
    };
}
