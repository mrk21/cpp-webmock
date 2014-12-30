#ifndef WEBMOCK_CORE_HTTP_HPP
#define WEBMOCK_CORE_HTTP_HPP

#include <webmock/core/ci_value_base.hpp>
#include <string>
#include <cctype>

namespace webmock { namespace core { namespace http {
    struct method: public ci_value_base<method> {
        using ci_value_base<method>::ci_value_base;
        
        operator std::string() const {
            std::string result;
            for (auto && c: this->data) {
                result.push_back(std::toupper(c));
            }
            return result;
        }
    };
}}}

#endif
