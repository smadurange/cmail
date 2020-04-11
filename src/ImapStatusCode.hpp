#pragma once

namespace cindel
{
    enum class ImapStatusCode
    { 
        Success = 0,
        InternalError,
        ConnectionError,
        AuthenticationError
    };
}
