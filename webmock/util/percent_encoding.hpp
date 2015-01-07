#ifndef WEBMOCK_UTIL_PERCENT_ENCODING_HPP
#define WEBMOCK_UTIL_PERCENT_ENCODING_HPP

#include <iomanip>
#include <string>
#include <sstream>
#include <boost/format.hpp>

namespace webmock { namespace util {
    // see: RFC3986(http://www.ietf.org/rfc/rfc3986.txt), 2.1
    struct percent_encoding {
        static std::string encode(std::string const & value) {
            std::ostringstream oss;
            for (auto c: value) {
                if (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~') {
                    oss << c;
                }
                else {
                    oss << boost::format("%%%02X") % static_cast<int>(c & 0xFF);
                }
            }
            return oss.str();
        }
        
        static std::string decode(std::string const & value) {
            std::istringstream iss(value);
            std::ostringstream oss;
            while (true) {
                char c = iss.get();
                if (iss.eof()) break;
                if (c == '%') {
                    std::string s;
                    iss >> std::setw(2) >> s;
                    int v;
                    std::istringstream(s) >> std::hex >> v;
                    oss << static_cast<char>(v);
                }
                else {
                    oss << c;
                }
            }
            return oss.str();
        }
    };
}}

#endif
