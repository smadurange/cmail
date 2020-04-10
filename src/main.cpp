#include <boost/asio/buffer.hpp>
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
    
    ssl::context ctx(ssl::context::sslv23);
    ssl::stream<ip::tcp::socket> socket(ioService, ctx);
    socket.set_verify_mode(ssl::verify_none);

    ip::tcp::resolver resolver(ioService);
    ip::tcp::resolver::query query(hostname, port);
    ip::tcp::resolver::iterator endpointIter = resolver.resolve(query);
    ip::tcp::resolver::iterator end;
    
    while(endpointIter != end)
    {
        ip::tcp::endpoint endpoint = *endpointIter++;
        std::cout << "Trying to connect to " << endpoint << std::endl;
        boost::system::error_code connErr;
        socket.lowest_layer().connect(endpoint, connErr);
        
        if(!connErr)
        {
            std::cout << "Connected to " << endpoint << std::endl;
            socket.handshake(ssl::stream<ip::tcp::socket>::client);
            std::cout << "SSL handshake success." << std::endl;
            break;
        };
    }

    // Login to mailbox.
    std::string username = "test.20200410@outlook.com";
    std::string password = "MN3SbbTVYviMi55F";
    std::stringstream ss;
    ss << "$ LOGIN " << username << " " << password;
    const char *req = ss.str().c_str();
    char data[500];
    strcpy(data, req);
    boost::asio::write(socket, boost::asio::buffer(data, sizeof(data)));
    std::cout << "Finished writing to socket." << std::endl;

    char buffer[10000];
    boost::asio::read(socket, boost::asio::buffer(buffer));
    std::string response = std::string(buffer);
    std::cout << "Received response: " << response << std::endl;
    
    return 0;
}
