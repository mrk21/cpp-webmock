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
    class string_condition_base {
        matcher match;
        
    public:
        template <typename T>
        string_condition_base(T const & value) : match(value) {}
        
        bool operator ()(request const & req) const {
            return this->match(req.*Attribute);
        }
    };
    
    using method = string_condition_base<&request::method>;
    using url = string_condition_base<&request::url>;
    using body = string_condition_base<&request::body>;
    
    class header {
        std::string key;
        matcher match;
        
    public:
        template <typename T>
        header(std::string const & key, T const & value) :
            key(key), match(value) {}
        
        bool operator ()(request const & req) const {
            if (req.headers.count(key) != 1) return false;
            return this->match(req.headers.find(key)->second);
        }
    };
}}

#endif
