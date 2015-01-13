#ifndef WEBMOCK_UTIL_URI_PARSER_HPP
#define WEBMOCK_UTIL_URI_PARSER_HPP

#include <iostream>
#include <map>
#include <boost/operators.hpp>

namespace webmock { namespace util {
    // Simple URI Parser
    // see: RFC3986(http://www.ietf.org/rfc/rfc3986.txt)
    struct uri_parser: boost::equality_comparable<uri_parser, uri_parser> {
        std::string scheme;
        std::string userinfo;
        std::string host;
        std::string port;
        std::string path;
        std::string query;
        std::string fragment;
        
        uri_parser(std::map<std::string, std::string> components);
        uri_parser(std::string const & uri);
    };
    
    bool operator ==(uri_parser const & lop, uri_parser const & rop);
    std::ostream & operator <<(std::ostream & lop, uri_parser const & rop);
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/util/uri_parser.ipp>
#endif

#endif
