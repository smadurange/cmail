#pragma once

#include <string>

namespace cmail
{
    struct Header
    {
        std::string Subject;
        std::string From;
        std::string Date;
    };
}
