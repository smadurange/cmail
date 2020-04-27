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
    cmail::Response response;
    if(connected)
        return response("", "", true);

    boost::system::error_code error;
    boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
    spdlog::trace("Attempting to resolve endpoint.");
    boost::asio::ip::tcp::resolver::iterator endpoints = resolver.resolve(query, error);
    if(error)
    {
        std::string err = "Failed to resolve hostname: " + error.message();
        spdlog::error(err);
        return response("", err, false);
    }
    
    spdlog::trace("Resolved endpoints for " + host);
    spdlog::trace("Sending connection request to server.");
    boost::asio::ip::tcp::resolver::iterator it = boost::asio::connect(socket.lowest_layer(), endpoints, error);
    if(error)
    {
        std::string err = "Failed to connect to IMAP server: " + error.message();
        spdlog::error(err);
        return response("", err, false);
    }

    socket.set_verify_mode(ssl::verify_none);
    spdlog::warn("SSL certificate validation is set to verify_none.");
    spdlog::trace("Initiating SSL handshake.");
    socket.handshake(ssl::stream<ip::tcp::socket>::client, error);
    if(error)
    {
        std::string err = "SSL handshake failed: " + error.message();
        spdlog::error(err);
        cmail::Connection::close();
        return response("", err, false);
    }
    
    spdlog::trace("SSL handshake completed.");
    connected = true;
    spdlog::info("Connected to IMAP server: " + it->address().to_string());
    return 
}

cmail::Response cmail::Connection::close()
{
    cmail::Response res;
    try
    {
        socket.shutdown();
        socket.close();
        spdlog::info("Connection to IMAP server closed.");
        return res("", "", true);
    }
    catch (std::exception &e)
    {
        std::string err = "Failed to close connection: " + e.what();
        spdlog::error(err);
        return res("", err, false);
    }
}
