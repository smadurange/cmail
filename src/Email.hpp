#pragma once

#include <string>

namespace cmail
{
    struct Email
    {
        int Id;
        std::string Subject;
        std::string From;
        std::string Date;
    };
}
