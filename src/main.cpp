#include <iostream>
#include <list>
#include <string>

#include <mailio/imap.hpp>

#include <ncurses.h>

#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/filereadstream.h>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

rapidjson::Document initConfig()
{
    std::FILE *fp = std::fopen("config.json", "r");
    char buf[65536];
    rapidjson::FileReadStream fs(fp, buf, sizeof(buf));
    rapidjson::Document doc;
    doc.ParseStream(fs);
    fclose(fp);
    return doc;
}

int main(int argc, char *argv[])
{
    rapidjson::Document config = initConfig();

    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    
    std::string host = config["host"].GetString();
    int port = config["port"].GetInt();
    std::string username = config["username"].GetString();
    std::string password = config["password"].GetString();

    mailio::imaps conn(host, port);
    conn.authenticate(username, password, mailio::imaps::auth_method_t::LOGIN);

    mailio::imaps::mailbox_stat_t stat = conn.statistics("inbox");
    printw("Number of messages in mailbox: %d",stat.messages_no);

    getch();
    endwin();

    return 0;
}
