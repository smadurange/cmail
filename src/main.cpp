#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <boost/system/error_code.hpp>

int main(int argc, char *argv[])
{
    const std::string hostname = "outlook.office365.com";
    const std::string port = "993";
    
    boost::asio::io_service ioService;
    
    boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sslSocket(ioService, sslContext);
    sslSocket.set_verify_mode(boost::asio::ssl::verify_none);
    boost::asio::ip::tcp::socket::lowest_layer_type &socket = sslSocket.lowest_layer();

    boost::asio::ip::tcp::resolver resolver(ioService);
    boost::asio::ip::tcp::resolver::query query(hostname, port);
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    
    while(iter != end)
    {
        boost::asio::ip::tcp::endpoint endpoint = *iter++;
        std::cout << "Trying to connect to " << endpoint << std::endl;
        boost::system::error_code connErr;
        socket.connect(endpoint, connErr);
        
        if(!connErr)
        {
            std::cout << "Connected to " << endpoint << std::endl;
            sslSocket.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::client);
            std::cout << "SSL handshake success." << std::endl;
            break;
        };
    }

    return 0;
}
