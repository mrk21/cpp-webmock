#ifndef WEBMOCK_REQUEST_HPP
#define WEBMOCK_REQUEST_HPP

#include <string>
#include <map>

namespace webmock {
    struct request {
        std::string method;
        std::string uri;
        std::multimap<std::string, std::string> headers;
        std::string body;
    };
}

#endif
