#pragma once

#include <string>

namespace cmail
{
    struct Response
    {
        std::string tag{std::string()};
        std::string content{std::string()};
        bool success{false}; 
    };
}
