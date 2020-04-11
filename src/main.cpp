#include <boost/asio.hpp>
#include <exception>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>

#include "ImapClient.hpp"

int main(int argc, char *argv[])
{
    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    const std::string hostname = "outlook.office365.com";
    const std::string port = "143";
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";
    
    boost::asio::io_service ioService;
    auto imapClient = cindel::ImapClient(ioService);

    try
    {
        imapClient.Connect(hostname, port);
    }
    catch (const std::exception &e)
    {
        spdlog::error("IMAP connection error: " + std::string(e.what()) );
        return -1;
    }        

    ioService.run();

    return 0;
}
