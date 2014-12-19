#ifndef WEBMOCK_API_DETAIL_CONDITION_HPP
#define WEBMOCK_API_DETAIL_CONDITION_HPP

#include <webmock/core/condition_list.hpp>
#include <webmock/core/request.hpp>
#include <functional>
#include <regex>
#include <boost/operators.hpp>

namespace webmock { namespace api { namespace detail {
    class matcher: boost::equality_comparable<matcher, std::string> {
        std::function<bool(std::string const &)> impl;
        
    public:
        matcher(std::string const & value) :
            impl([=](auto && target){
                return target == value;
            })
        {}
        
        matcher(const char * value) :
            matcher(std::string(value))
        {}
        
        matcher(std::regex const & pettern) :
            impl([=](auto && target){
                return std::regex_match(target, pettern);
            })
        {}
        
        bool operator ==(std::string const & value) const {
            return this->impl(value);
        }
        
        bool operator ()(std::string const & value) const {
            return this->impl(value);
        }
    };
    
    template <std::string core::request::*Attribute>
    class with_string_attribute {
        matcher match;
    
    public:
        template <typename ExpectedType>
        with_string_attribute(ExpectedType const & value) : match(value) {}
        
        bool operator ()(core::request const & request) const {
            return this->match(request.*Attribute);
        }
    };
}}}

#endif
