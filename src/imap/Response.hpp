#pragma once

#include <string>

namespace cmail
{
    struct Response
    {
        std::string tag;
        std::string content;
        bool success; 
    };
}
