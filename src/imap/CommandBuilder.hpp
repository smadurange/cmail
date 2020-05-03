#pragma once

#include <atomic>
#include <string>

namespace cmail::imap
{
    struct Command;

    class CommandBuilder
    {
    public:
        CommandBuilder(CommandBuilder const&) = delete;
        CommandBuilder &operator=(CommandBuilder const&) = delete;
        ~CommandBuilder() = default;

        static CommandBuilder &getInstance();
        const Command command(const std::string &command);
    private:
        std::atomic<int> id;

        CommandBuilder();
    };
}
