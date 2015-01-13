#ifndef WEBMOCK_CORE_REQUEST_HPP
#define WEBMOCK_CORE_REQUEST_HPP

#include <webmock/core/http.hpp>
#include <iostream>

namespace webmock { namespace core {
    struct request {
        http::method method;
        http::url url;
        http::headers headers;
        http::body body;
    };
    
    bool operator ==(request const & lop, request const & rop);
    bool operator !=(request const & lop, request const & rop);
    std::ostream & operator <<(std::ostream & lop, request const & rop);
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/core/request.ipp>
#endif

#endif
