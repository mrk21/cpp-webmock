#ifndef WEBMOCK_CORE_RESPONSE_HPP
#define WEBMOCK_CORE_RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>
#include <tuple>
#include <webmock/core/http.hpp>

namespace webmock { namespace core {
    struct response {
        using header_type = std::multimap<std::string, std::string>;
        
        http::status status;
        std::string body;
        header_type headers;
    };
    
    inline bool operator ==(response const & lop, response const & rop) {
        return std::tie(lop.status, lop.body, lop.headers)
            == std::tie(rop.status, rop.body, rop.headers);
    }
    
    inline bool operator !=(response const & lop, response const & rop) {
        return !(lop == rop);
    }
    
    inline std::ostream & operator <<(std::ostream & lop, response const & rop) {
        lop << rop.status << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}}

#endif
