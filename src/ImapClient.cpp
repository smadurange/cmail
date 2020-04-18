#include <chrono>
#include <ctime>
#include <iomanip>
#include <istream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/asio/connect.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <boost/system/error_code.hpp>

#include <spdlog/spdlog.h>

#include "ImapClient.hpp"

namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

cindel::ImapClient::ImapClient(boost::asio::io_service &ioService, ssl::context &sslContext)
    : socket(ioService, sslContext), resolver(ioService)
{
}

void cindel::ImapClient::connect(const std::string &hostname, const std::string &port)
{
    boost::system::error_code error;
    ip::tcp::resolver::iterator endpoints = resolver.resolve(hostname, port, error);
    if(error) throw std::runtime_error("Failed to resolve hostname: " + error.message());
    
    spdlog::trace("Sending connection request to server.");
    ip::tcp::resolver::iterator it = boost::asio::connect(socket.lowest_layer(), endpoints, error);
    if(error) throw std::runtime_error("Failed to connect to IMAP server: " + error.message());
    ip::tcp::endpoint endpoint = *it;
    spdlog::debug("Connected to IMAP server: " + endpoint.address().to_string());

    socket.set_verify_mode(ssl::verify_none);
    spdlog::warn("SSL certificate validation is set to verify_none.");
    socket.handshake(ssl::stream<ip::tcp::socket>::client, error);
    if(error) throw std::runtime_error("SSL handshake failed: " + error.message());

    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\n', error);
    if(error) throw std::runtime_error("Failed to get a response from server: " + error.message());
    std::istream is(&buffer);
    std::string response;
    std::getline(is, response);
    spdlog::trace("Server responded to connection request with\n" + response);
}

bool cindel::ImapClient::login(const std::string &username, const std::string &password)
{
    std::string cmd;
    cmd.reserve(username.length() + password.length() + 7);
    cmd.append("LOGIN ").append(username).append(" ").append(password);
    std::string response = execute(cmd);
    std::regex rgx("OK LOGIN completed.");
    return std::regex_search(response, rgx);
}

std::vector<std::string>::iterator cindel::ImapClient::fetch(const int days)
{
    std::string cmd = "SELECT INBOX";
    std::string response = execute(cmd);
    if(response.empty()) return std::vector<std::string>::iterator();
   
    std::stringstream ss;
    auto tp = std::chrono::system_clock::now() - std::chrono::hours(days * 24);
    auto t = std::chrono::system_clock::to_time_t(tp);
    ss << "SEARCH SINCE " << std::put_time(std::localtime(&t), "%d-%b-%Y");
    response = execute(ss.str());
    
    return std::vector<std::string>::iterator();
}

std::string cindel::ImapClient::execute(const std::string &command)
{
    std::stringstream ss;
    ss << ++commandCounter << " " << command;
    std::string cmd = ss.str();
    spdlog::trace("C: " + cmd);
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(cmd.append("\r\n")), error);
    if(error)
    {
        spdlog::error("Failed to dispatch command " + cmd + " because " + error.message());
        return ""; 
    }
    
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\n', error);
    if(error)
    {
        spdlog::error("Failed to receive response for command " + cmd + " because " + error.message());
        return "";
    }
    
    ss.str(std::string());
    ss.clear();
    std::istream is(&buffer);
    std::string line;
    while(std::getline(is, line))
    {
       ss << line << std::endl; 
    }
    
    std::string response = ss.str();
    // Pop the last line feed character for better formatting.
    if(!response.empty()) response.pop_back();
    spdlog::trace("S: " + response);
    return response;
}
