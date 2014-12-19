#ifndef WEBMOCK_API_DETAIL_CONDITION_HPP
#define WEBMOCK_API_DETAIL_CONDITION_HPP

#include <webmock/core/condition_list.hpp>
#include <functional>
#include <regex>

namespace webmock { namespace api { namespace detail {
    class matcher {
        std::function<bool(std::string const &)> impl;
        
    public:
        matcher(std::string const & value) :
            impl([=](std::string const & target){
                return target == value;
            })
        {}
        
        matcher(std::regex const & pettern) :
            impl([=](std::string const & target){
                return std::regex_match(target, pettern);
            })
        {}
        
        bool operator ()(std::string const & target) const {
            return this->impl(target);
        }
    };
    
    template <std::string core::request::*Attribute>
    class with_string_attribute {
        matcher match;
        
    public:
        template <typename T>
        with_string_attribute(T const & value) : match(value) {}
        
        bool operator ()(core::request const & req) const {
            return this->match(req.*Attribute);
        }
    };
}}}

#endif
