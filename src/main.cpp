#include <boost/system/error_code.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

int main(int argc, char *argv[])
{
    const std::string hostname = "outlook.office365.com";
    const std::string port = "993";
    
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket socket(ioService);
    boost::asio::ip::tcp::resolver resolver(ioService);
    boost::asio::ip::tcp::resolver::query query(hostname, port);
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    
    while(iter != end)
    {
        boost::asio::ip::tcp::endpoint ep = *iter++;
        std::cout << "Trying to connect to " << ep << std::endl;
        boost::system::error_code connErr;
        socket.connect(ep, connErr);
        
        if(!connErr)
        {
            std::cout << "Connected to " << ep << std::endl;
            break;
        };
    }

    return 0;
}
