#ifndef WEBMOCK_RESPONSE_HPP
#define WEBMOCK_RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>

namespace webmock {
    struct response {
        std::string status;
        std::string body;
        std::multimap<std::string, std::string> headers;
    };
    
    bool operator ==(response const & lop, response const & rop);
    bool operator !=(response const & lop, response const & rop);
    std::ostream & operator <<(std::ostream & lop, response const & rop);
}

#endif
