#include <spdlog/spdlog.h>

#include "connection.hpp"

using boost::asio::io_context;
using boost::asio::ssl::context;

cmail::imap::connection::connection()
    : ctx(new io_context()),
      ssl(new context(context::sslv23)),
      socket(*ctx, *ssl)
{
    spdlog::warn("SSL verify mode is set to verify_none.");
    ssl->set_verify_mode(boost::asio::ssl::verify_none);
}

cmail::imap::connection::~connection()
{
    socket.lowest_layer().cancel();
    socket.shutdown();
    spdlog::info("IMAP connection closed.");
}
