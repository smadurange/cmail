#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>

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

cindel::ImapStatusCode cindel::ImapClient::login(const std::string &username, const std::string &password)
{
    const std::string command = "LOGIN " + username + " " + password + "\r\n";
    std::string response = execute(command);

    if(response.empty())
    {
        return ImapStatusCode::ConnectionError;
    }
    
    if(response.compare("A001 OK LOGIN completed."))
    {
        return ImapStatusCode::Success;
    }
    else if(response.compare("A001 NO LOGIN failed."))
    {
        return ImapStatusCode::AuthenticationError;
    }
    else
    {
        spdlog::error("Unknown server response to login request: " + response);
        return ImapStatusCode::InternalError;
    }
}

std::string cindel::ImapClient::execute(const std::string &command)
{
    std::stringstream ss;
    ss << ++commandCounter << " " << command << "\r\n";
    std::string cmd = ss.str();
    spdlog::trace("Executing command: " + cmd);
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(cmd), error);
    
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

    std::istream is(&buffer);
    std::string response;
    std::getline(is, response);
    return response;
}
