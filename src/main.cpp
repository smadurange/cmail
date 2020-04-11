#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <boost/system/error_code.hpp>

namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

int main(int argc, char *argv[])
{
    const std::string hostname = "outlook.office365.com";
    const std::string port = "993";
    
    boost::asio::io_service ioService;

    ip::tcp::socket socket(ioService);

    ip::tcp::resolver resolver(ioService);
    ip::tcp::resolver::iterator endpoints = resolver.resolve(hostname, port);
    
    boost::system::error_code connErr;
    ip::tcp::resolver::iterator connectedEndpointIter =  boost::asio::connect(socket, endpoints);
    ip::tcp::endpoint connectedEndpoint = *connectedEndpointIter;
    
    if(connErr)
    {
        std::cout << "Failed to connect to IMAP server: " << connErr.message() << std::endl;
        return -1;
    }

    std::cout << "Connected to IMAP server: " << connectedEndpoint << std::endl;

    return 0;
}
