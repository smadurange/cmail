#include <mutex>
#include <spdlog/spdlog.h>

#include "connection.hpp"

using std::mutex;

using boost::asio::io_context;
using boost::asio::ssl::context;

cmail::imap::connection::connection()
    : ctx(new io_context()),
      ssl(new context(context::sslv23)),
      mtx_soc(mutex()),
      soc(*ctx, *ssl)
{
    spdlog::warn("SSL verify mode is set to verify_none.");
    ssl->set_verify_mode(boost::asio::ssl::verify_none);
}

cmail::imap::connection::~connection()
{
    const std::lock_guard<mutex> lock(mtx_soc);
    soc.lowest_layer().cancel();
    soc.shutdown();
    spdlog::info("IMAP connection closed.");
}
