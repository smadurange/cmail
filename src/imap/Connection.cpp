#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>

#include <spdlog/spdlog.h>

#include "Command.hpp"
#include "Connection.hpp"

using std::lock_guard;
using std::mutex;
using std::ostringstream;
using std::runtime_error;
using std::string;
using std::to_string;

using boost::asio::buffer;
using boost::asio::buffers_begin;
using boost::asio::buffers_end;
using boost::asio::io_context;
using boost::asio::ip::tcp;
using boost::asio::read;
using boost::asio::ssl::context;
using boost::asio::ssl::stream;
using boost::asio::streambuf;
using boost::asio::transfer_at_least;
using boost::asio::write;
using boost::system::error_code;

using cmail::imap::Command;

cmail::imap::Connection::Connection()
    : ctx(new io_context()),
      ssl(new context(context::sslv23)),
      mtx(mutex()),
      socket(*ctx, *ssl),
      connected(false),
      host(""),
      port(0)
{
    spdlog::warn("SSL verify mode is set to verify_none.");
    ssl->set_verify_mode(boost::asio::ssl::verify_none);
}

cmail::imap::Connection::~Connection()
{
    const lock_guard<mutex> lock(mtx);
    socket.lowest_layer().cancel();
    socket.lowest_layer().close();
    spdlog::info("IMAP Connection closed.");
}

bool cmail::imap::Connection::open(const string &h, int p)
{
    const lock_guard<mutex> lock(mtx);
    spdlog::trace("Opening Connection to host %s on post %d", host, port);
    if(connected)
    {
        spdlog::trace("Connection is already open to host %s on port %d", host, port);
        return true;
    }
    
    host = h;
    port = p;
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
    spdlog::trace("Sending Connection request to %s on %d", host, port);
    tcp::resolver::iterator it =  connect(socket.lowest_layer(), endpoints, error);
    if (error)
    {
        spdlog::error("Failed to connect to host %s on %d: %s", host, port, error.message());
        return false;
    }

    spdlog::trace("Established Connection with host %s on %d.", host, port);
    spdlog::trace("Initiating SSL handshake.");
    socket.handshake(stream<tcp::socket>::client, error);
    if(error)
    {
        spdlog::error("SSL handshake with host %s failed: %s", host, error.message());
        socket.lowest_layer().cancel();
        socket.shutdown();
        return false;
    }

    spdlog::trace("SSL handshake completed.");
    connected = true;
    spdlog::info("Successfully connected to host %s on %d", host, port);
    return connected;
}

void cmail::imap::Connection::send(const Command &cmd)
{
    const lock_guard<mutex> lock(mtx);
    if(!connected)
        throw runtime_error("A connection must be opened before sending requests.");

    error_code ec;
    spdlog::trace("C: " + cmd.text); 
    write(socket, buffer(cmd.text), ec);
    if(ec)
    {
        ostringstream os;
        os << "Failed to send request " << cmd.tag << " to server: " << ec.message();
        string msg = os.str();       
        spdlog::error(msg);
        throw runtime_error(msg);
    }
}

void cmail::imap::Connection::receive()
{
    streambuf buf;
    size_t size = read(socket, buf, transfer_at_least(1));
    string data(buffers_begin(buf.data()), buffers_end(buf.data()));
    buf.consume(size);
    spdlog::debug(data);
}
