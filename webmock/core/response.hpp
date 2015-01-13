#ifndef WEBMOCK_CORE_RESPONSE_HPP
#define WEBMOCK_CORE_RESPONSE_HPP

#include <webmock/core/http.hpp>
#include <iostream>

namespace webmock { namespace core {
    struct response {
        http::status status;
        http::body body;
        http::headers headers;
    };
    
    bool operator ==(response const & lop, response const & rop);
    bool operator !=(response const & lop, response const & rop);
    std::ostream & operator <<(std::ostream & lop, response const & rop);
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/core/response.ipp>
#endif

#endif
