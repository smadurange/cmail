#pragma once

#include <memory>
#include <mutex>

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
        ~connection();

        connection(connection const&) = delete;
        connection& operator=(connection const&) = delete;
    private:
        std::unique_ptr<boost::asio::io_context> const ctx;
        std::unique_ptr<boost::asio::ssl::context> const ssl;
        std::mutex mtx_soc;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> soc; 
    };
}
