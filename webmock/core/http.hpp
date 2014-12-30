#ifndef WEBMOCK_CORE_HTTP_HPP
#define WEBMOCK_CORE_HTTP_HPP

#include <webmock/core/ci_value_base.hpp>
#include <string>
#include <cctype>
#include <boost/lexical_cast.hpp>

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
    
    struct status: public ci_value_base<status> {
        using ci_value_base<status>::ci_value_base;
        
        status() : ci_value_base("200") {}
        status(unsigned int v) : ci_value_base(boost::lexical_cast<std::string>(v)) {}
        
        operator unsigned int() const {
            return boost::lexical_cast<unsigned int>(this->data.c_str());
        }
    };
}}}

#endif
