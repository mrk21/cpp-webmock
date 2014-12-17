#ifndef WEBMOCK_DIRECTIVE_CONDITION_HPP
#define WEBMOCK_DIRECTIVE_CONDITION_HPP

#include <webmock/condition_list.hpp>
#include <functional>
#include <regex>

namespace webmock { namespace directive {
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
    
    template <std::string request::*Attribute>
    class with_string_attribute {
        matcher match;
        
    public:
        template <typename T>
        with_string_attribute(T const & value) : match(value) {}
        
        bool operator ()(request const & req) const {
            return this->match(req.*Attribute);
        }
    };
    
    using with = condition_list::condition_type;
    using with_method = with_string_attribute<&request::method>;
    using with_url = with_string_attribute<&request::url>;
    using with_body = with_string_attribute<&request::body>;
    
    class with_header {
        std::string key;
        matcher match;
        
    public:
        template <typename T>
        with_header(std::string const & key, T const & value) :
            key(key), match(value) {}
        
        bool operator ()(request const & req) const {
            if (req.headers.count(key) != 1) return false;
            return this->match(req.headers.find(key)->second);
        }
    };
}}

#endif
