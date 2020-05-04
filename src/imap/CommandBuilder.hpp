#pragma once

#include <atomic>
#include <string>

namespace cmail::imap
{
    struct Command;

    class CommandBuilder
    {
    public:
        CommandBuilder();
        ~CommandBuilder() = default;

        const Command command(const std::string &command);
    private:
        std::atomic<int> id;
    };
}
