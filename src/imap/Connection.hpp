#pragma once

#include <atomic>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace cmail
{
    struct Response;

    class Connection
    {
    public:
        static Connection &instance();
        Connection(Connection const&) = delete;
        Connection &operator=(Connection const&) = delete;
        ~Connection() = default;
        void open(std::string host, int port);
        void close();
        Response send(const std::string &request);

    private:
        bool connected;
        std::mutex mtx;
        std::atomic<int> counter{0};
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
    };
}
