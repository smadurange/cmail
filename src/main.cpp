#include <exception>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

#include "Email.hpp"
#include "ImapClient.hpp"

int main(int argc, char *argv[])
{
    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    const std::string hostname = "outlook.office365.com";
    const std::string port = "993";
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";
    
    boost::asio::io_service ioService;
    boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
    auto client = cmail::ImapClient(ioService, sslContext);
    client.connect(hostname, port);
    client.login(username, password);
    std::vector<cmail::Email> mailbox = client.fetchMailbox(std::stoi(argv[1]));
    ioService.run();

    return 0;
}
