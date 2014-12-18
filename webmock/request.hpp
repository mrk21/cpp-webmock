#ifndef WEBMOCK_REQUEST_HPP
#define WEBMOCK_REQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <tuple>

namespace webmock {
    struct request {
        std::string method;
        std::string url;
        std::multimap<std::string, std::string> headers;
        std::string body;
    };
    
    inline bool operator ==(request const & lop, request const & rop) {
        return std::tie(lop.method, lop.url, lop.headers, lop.body)
            == std::tie(rop.method, rop.url, rop.headers, rop.body);
    }
    
    inline bool operator !=(request const & lop, request const & rop) {
        return !(lop == rop);
    }
    
    inline std::ostream & operator <<(std::ostream & lop, request const & rop) {
        lop << rop.method << " " << rop.url << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}

#endif
