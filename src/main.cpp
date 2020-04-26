#include <iostream>
#include <list>
#include <string>

#include <mailio/imap.hpp>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char *argv[])
{
    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    const std::string host = "outlook.office365.com";
    int port = 993;
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";

    mailio::imaps conn(host, port);
    conn.authenticate(username, password, mailio::imaps::auth_method_t::LOGIN);

    mailio::imaps::mailbox_stat_t stat = conn.statistics("inbox");
    std::cout << "Number of messages in mailbox: " << stat.messages_no << std::endl;

    return 0;
}
