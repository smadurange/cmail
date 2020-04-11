#include "ImapClient.hpp"
#include <boost/asio/io_service.hpp>

cindel::ImapClient::ImapClient(boost::asio::io_service &ioService)
    : socket(ioService)
{
}
