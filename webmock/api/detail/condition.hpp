#ifndef WEBMOCK_API_DETAIL_CONDITION_HPP
#define WEBMOCK_API_DETAIL_CONDITION_HPP

#include <webmock/core/condition_list.hpp>
#include <webmock/core/request.hpp>
#include <functional>
#include <regex>
#include <boost/operators.hpp>

namespace webmock { namespace api { namespace detail {
    template <typename T>
    class matcher: boost::equality_comparable<matcher<T>, T> {
    public:
        using value_type = T;
        
        std::function<bool(value_type const &)> impl;
        
    public:
        matcher(value_type const & value) :
            impl([=](auto && target){
                return target == value;
            })
        {}
        
        matcher(std::regex const & pettern) :
            impl([=](auto && target){
                return std::regex_match(static_cast<std::string>(target), pettern);
            })
        {}
        
        bool operator ==(value_type const & value) const {
            return this->impl(value);
        }
        
        bool operator ()(value_type const & value) const {
            return this->impl(value);
        }
    };
    
    template <typename T, T core::request::*Attribute>
    class with_attribute {
        matcher<T> match;
    
    public:
        template <typename ExpectedType>
        with_attribute(ExpectedType const & value) : match(value) {}
        
        bool operator ()(core::request const & request) const {
            return this->match(request.*Attribute);
        }
    };
}}}

#endif
