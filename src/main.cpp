#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <mailio/imap.hpp>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char *argv[])
{
    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    const std::string hostname = "outlook.office365.com";
    const std::string port = "993";
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";
    
    return 0;
}
