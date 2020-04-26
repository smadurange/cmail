#include <iostream>
#include <list>
#include <string>

#include <mailio/imap.hpp>

#include <ncurses.h>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char *argv[])
{
    const std::string host = "outlook.office365.com";
    int port = 993;
    const std::string username = "test.20200410@outlook.com";
    const std::string password = "MN3SbbTVYviMi55F";

    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    mailio::imaps conn(host, port);
    conn.authenticate(username, password, mailio::imaps::auth_method_t::LOGIN);

    mailio::imaps::mailbox_stat_t stat = conn.statistics("inbox");
    printw("Number of messages in mailbox: %d",stat.messages_no);

    getch();
    endwin();

    return 0;
}
