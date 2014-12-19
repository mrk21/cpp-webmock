#ifndef WEBMOCK_API_CONDITION_HPP
#define WEBMOCK_API_CONDITION_HPP

#include <webmock/api/detail/condition.hpp>

namespace webmock { namespace api {
    using with = core::condition_list::condition_type;
    using with_method = detail::with_string_attribute<&core::request::method>;
    using with_url = detail::with_string_attribute<&core::request::url>;
    using with_body = detail::with_string_attribute<&core::request::body>;
    
    class with_header {
        std::string key;
        detail::matcher match;
        
    public:
        template <typename T>
        with_header(std::string const & key, T const & value) :
            key(key), match(value) {}
        
        bool operator ()(core::request const & req) const {
            if (req.headers.count(key) != 1) return false;
            return this->match(req.headers.find(key)->second);
        }
    };
}}

#endif
