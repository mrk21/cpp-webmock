#ifndef WEBMOCK_UTIL_PERCENT_ENCODING_HPP
#define WEBMOCK_UTIL_PERCENT_ENCODING_HPP

#include <string>

namespace webmock { namespace util {
    // see: RFC3986(http://www.ietf.org/rfc/rfc3986.txt), 2.1
    struct percent_encoding {
        static std::string encode(std::string const & value);
        static std::string decode(std::string const & value);
    };
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/util/percent_encoding.ipp>
#endif

#endif
