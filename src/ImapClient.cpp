#include <istream>
#include <stdexcept>
#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>

#include <spdlog/spdlog.h>

#include "ImapClient.hpp"

cindel::ImapClient::ImapClient(boost::asio::io_service &ioService)
    : socket(ioService), resolver(ioService)
{
}

void cindel::ImapClient::Connect(const std::string &hostname, const std::string &port)
{
    boost::system::error_code error;
    boost::asio::ip::tcp::resolver::iterator endpoints = resolver.resolve(hostname, port, error);
    
    if (error)
    {
        throw std::runtime_error("Failed to resolve hostname: " + error.message());
    }

    boost::asio::ip::tcp::resolver::iterator iter = boost::asio::connect(socket, endpoints, error);
    
    if(error)
    {
        throw std::runtime_error("Failed to connect to IMAP server: " + error.message());
    }
    else
    {
        boost::asio::ip::tcp::endpoint endpoint = *iter;
        spdlog::debug("Connected to IMAP server: " + endpoint.address().to_string());
    }

    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, '\n', error);
    std::istream is(&buf);
    std::string response;
    std::getline(is, response);
    buf.consume(buf.size());
    spdlog::trace(response);
}
