#include <istream>
#include <stdexcept>
#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/verify_mode.hpp>

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
    if (error) throw std::runtime_error("Failed to resolve hostname: " + error.message());

    ip::tcp::resolver::iterator it = boost::asio::connect(socket.lowest_layer(), endpoints, error);
    if(error) throw std::runtime_error("Failed to connect to IMAP server: " + error.message());
    ip::tcp::endpoint endpoint = *it;
    spdlog::debug("Connected to IMAP server: " + endpoint.address().to_string());

    socket.set_verify_mode(ssl::verify_none);
    spdlog::warn("SSL certificate validation is set to verify_none.");
    socket.handshake(ssl::stream<ip::tcp::socket>::client, error);
    if (error) throw std::runtime_error("SSL handshake failed: " + error.message());

    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, '\n', error);
    std::istream is(&buf);
    std::string response;
    std::getline(is, response);
    buf.consume(buf.size());
    spdlog::trace(response);
}
