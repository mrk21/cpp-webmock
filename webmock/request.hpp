#ifndef WEBMOCK_REQUEST_HPP
#define WEBMOCK_REQUEST_HPP

#include <string>
#include <map>
#include <iostream>

namespace webmock {
    struct request {
        std::string method;
        std::string url;
        std::multimap<std::string, std::string> headers;
        std::string body;
    };
    
    bool operator ==(request const & lop, request const & rop);
    bool operator !=(request const & lop, request const & rop);
    std::ostream & operator <<(std::ostream & lop, request const & rop);
}

#endif
