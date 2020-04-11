#include <boost/asio.hpp>

#include "ImapClient.hpp"

int main(int argc, char *argv[])
{
    const std::string hostname = "outlook.office365.com";
    const std::string port = "143";
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";
    
    boost::asio::io_service ioService;

    auto imapClient = cindel::ImapClient(ioService);
    imapClient.Connect(hostname, port);

    ioService.run();

    return 0;
}
