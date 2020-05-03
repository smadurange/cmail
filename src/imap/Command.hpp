# pragma once

#include <string>

namespace cmail::imap
{
    struct Command
    {
        std::string tag;
        std::string text;
    };
}
