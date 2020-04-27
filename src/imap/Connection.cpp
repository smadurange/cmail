#include <exception>
#include <mutex>
#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/system/error_code.hpp>

#include <spdlog/spdlog.h>

#include "Connection.hpp"
#include "Response.hpp"

cmail::Connection::Connection(boost::asio::io_context &ctx, boost::asio::ssl::context &ssl)
    : resolver(ctx), socket(ctx, ssl)
{
}

cmail::Connection &cmail::Connection::instance()
{
    boost::asio::io_context ctx;
    boost::asio::ssl::context ssl(boost::asio::ssl::context::sslv23);
    static Connection conn(ctx, ssl);
    return conn;
}

cmail::Response cmail::Connection::open(const std::string &host, int port)
{
    std::lock_guard<std::mutex> lock(mtx);
    cmail::Response res;
    if(connected)
    {
        res.success = true;
        return res;
    }

    boost::system::error_code error;
    boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
    spdlog::trace("Attempting to resolve endpoint.");
    boost::asio::ip::tcp::resolver::iterator endpoints = resolver.resolve(query, error);
    if(error)
    {
        std::string err = std::string("Failed to resolve hostname: ") + error.message();
        spdlog::error(err);
        res.content = err;
        return res;
    }
    
    spdlog::trace("Resolved endpoints for " + host);
    spdlog::trace("Sending connection request to server.");
    boost::asio::ip::tcp::resolver::iterator it = boost::asio::connect(socket.lowest_layer(), endpoints, error);
    if(error)
    {
        std::string err = std::string("Failed to connect to IMAP server: ") + error.message();
        spdlog::error(err);
        res.content = err;
        return res;
    }

    socket.set_verify_mode(boost::asio::ssl::verify_none);
    spdlog::warn("SSL certificate validation is set to verify_none.");
    spdlog::trace("Initiating SSL handshake.");
    socket.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::client, error);
    if(error)
    {
        std::string err = std::string("SSL handshake failed: ") + error.message();
        spdlog::error(err);
        cmail::Connection::close();
        res.content = err;
        return res;
    }
    
    spdlog::trace("SSL handshake completed.");
    connected = true;
    boost::asio::ip::tcp::endpoint ep = *it;
    spdlog::info("Connected to IMAP server: " + ep.address().to_string());
    res.success = true;
    return res;
}

cmail::Response cmail::Connection::close()
{
    cmail::Response res;
    try
    {
        socket.lowest_layer().cancel();
        socket.shutdown();
        spdlog::info("Connection to IMAP server closed.");
        res.success = true;
        return res;
    }
    catch (std::exception &e)
    {
        std::string err = std::string("Failed to close connection: ") + e.what();
        spdlog::error(err);
        res.content = err;
        return res;
    }
}
