#include <iostream>
#include <list>
#include <rapidjson/encodings.h>
#include <string>

//#include <ncurses.h>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "imap/connection.hpp"

using std::FILE;
using std::fopen;

using cmail::imap::connection;

using rapidjson::Document;
using rapidjson::FileReadStream;

Document initConfig()
{
    FILE *fp = fopen("config.json", "r");
    char buf[65536];
    FileReadStream fs(fp, buf, sizeof(buf));
    Document doc;
    doc.ParseStream(fs);
    fclose(fp);
    return doc;
}

int main(int argc, char *argv[])
{
    auto config = initConfig();

    auto console = spdlog::stdout_color_mt("console");  
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);

    // initscr();
    // raw();
    // noecho();
    // keypad(stdscr, true);
    
    auto host = config["host"].GetString();
    auto port = config["port"].GetInt();
    auto username = config["username"].GetString();
    auto password = config["password"].GetString();
    
    connection conn;
    // printw("Number of messages in mailbox: %d",stat.messages_no);

    // getch();
    // endwin();

    return 0;
}
