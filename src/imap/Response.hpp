#pragma once

#include <string>

namespace cmail::imap
{
    struct Response
    {
        std::string tag{std::string()};
        std::string content{std::string()};
        bool success{false}; 
    };
}
