#include <sstream>

#include "Command.hpp"
#include "CommandBuilder.hpp"

using std::string;
using std::ostringstream;

using cmail::imap::Command;

cmail::imap::CommandBuilder::CommandBuilder()
    : id(0)
{
}

cmail::imap::CommandBuilder &cmail::imap::CommandBuilder::getInstance()
{
    static CommandBuilder instance;
    return instance;
}

const Command cmail::imap::CommandBuilder::command(const string &command)
{
    ++id;
    ostringstream os;
    if(id < 10)
       os << "A00" << id;
    else if(id < 100)
       os << "A0" << id;
    else
       os << "A" << id;
    
    Command cmd;
    cmd.tag = os.str();
    cmd.text = cmd.tag + " " + command + "\r\n";
    return cmd;
}
