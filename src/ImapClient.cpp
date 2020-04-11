#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <istream>
#include <stdexcept>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <string>

#include "ImapClient.hpp"a

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
        // Log trace.
        boost::asio::ip::tcp::endpoint endpoint = *iter;
        std::cout << "Connected to host: " << endpoint << std::endl;
    }

    boost::asio::streambuf buf;

    boost::asio::read_until(socket, buf, '\n', error);
    std::istream is(&buf);
    std::string response;
    std::getline(is, response);
    buf.consume(buf.size());

    // Log trace
    std::cout << response << std::endl;
}
