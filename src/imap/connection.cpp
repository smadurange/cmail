#include <mutex>
#include <string>

#include <boost/asio/ssl/stream.hpp>
#include <boost/system/error_code.hpp>

#include <spdlog/spdlog.h>

#include "connection.hpp"

using std::lock_guard;
using std::mutex;
using std::string;
using std::to_string;

using boost::asio::io_context;
using boost::asio::ip::tcp;
using boost::asio::ssl::context;
using boost::asio::ssl::stream;
using boost::system::error_code;

cmail::imap::connection::connection()
    : ctx(new io_context()),
      ssl(new context(context::sslv23)),
      mtx_soc(mutex()),
      soc(*ctx, *ssl),
      soc_connected(false)
{
    spdlog::warn("SSL verify mode is set to verify_none.");
    ssl->set_verify_mode(boost::asio::ssl::verify_none);
}

cmail::imap::connection::~connection()
{
    const lock_guard<mutex> lock(mtx_soc);
    soc.lowest_layer().cancel();
    soc.shutdown();
    spdlog::info("IMAP connection closed.");
}

bool cmail::imap::connection::open(const string &host, int port)
{
    const lock_guard<mutex> lock(mtx_soc);
    spdlog::trace("Opening connection to host %s on post %d", host, port);
    if(soc_connected)
    {
        spdlog::trace("Connection is already open to host %s on port %d", host, port);
        return true;
    }
    
    spdlog::trace("Resolving endpoints for %s:%d", host, port);
    error_code error;
    tcp::resolver resolver(*ctx);
    tcp::resolver::query query(host, to_string(port));
    tcp::resolver::iterator endpoints = resolver.resolve(query, error);
    if(error)
    {
        spdlog::error("Failed to resolve endpoints for host %s: %s", host, error.message());
        return false;
    }

    spdlog::trace("Resolved endpoints for %s:%d", host, port);
    spdlog::trace("Sending connection request to %s on %d", host, port);
    tcp::resolver::iterator it =  connect(soc.lowest_layer(), endpoints, error);
    if (error)
    {
        spdlog::error("Failed to connect to host %s on %d: %s", host, port, error.message());
        return false;
    }

    spdlog::trace("Established connection with host %s on %d.", host, port);
    spdlog::trace("Initiating SSL handshake.");
    soc.handshake(stream<tcp::socket>::client, error);
    if(error)
    {
        spdlog::error("SSL handshake with host %s failed: %s", host, error.message());
        soc.lowest_layer().cancel();
        soc.shutdown();
        return false;
    }

    spdlog::trace("SSL handshake completed.");
    soc_connected = true;
    spdlog::info("Successfully connected to host %s on %d", host, port);
    return soc_connected;
}
