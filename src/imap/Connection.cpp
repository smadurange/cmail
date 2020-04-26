#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/system/error_code.hpp>

#include "Connection.hpp"
#include "Response.hpp"

cmail::Connection::Connection(boost::asio::io_context &ctx, boost::asio::ssl::context &ssl)
    : socket(ctx, ssl)
{
}

cmail::Connection &cmail::Connection::instance()
{
    boost::asio::io_context ctx;
    boost::asio::ssl::context ssl(boost::asio::ssl::context::sslv23);
    static Connection conn(ctx, ssl);
    return conn;
}

void cmail::Connection::open(const std::string &host, int port)
{
    mtx.lock();
    if(connected)
        return;
    
    mtx.unlock()
}
