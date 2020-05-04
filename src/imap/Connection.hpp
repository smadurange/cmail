#pragma once

#include <memory>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace cmail::imap
{
    struct Command;

    class Connection
    {
    public:
        explicit Connection();
        ~Connection();
        Connection(Connection const&) = delete;
        Connection& operator=(Connection const&) = delete;

        bool open(const std::string &host, int port);

    private:
        std::unique_ptr<boost::asio::io_context> const ctx;
        std::unique_ptr<boost::asio::ssl::context> const ssl;
        std::mutex mtx;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
        bool connected;
        std::string host;
        int port;
        
        void send(const Command &command);
        void receive();
    };
}
