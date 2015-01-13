#ifndef WEBMOCK_API_CONDITION_HPP
#define WEBMOCK_API_CONDITION_HPP

#include <webmock/api/detail/condition.hpp>

namespace webmock { namespace api {
inline namespace directive {
    using with = core::condition_list::condition_type;
    using with_method = detail::with_attribute<core::http::method, &core::request::method>;
    using with_url = detail::with_attribute<core::http::url, &core::request::url>;
    using with_body = detail::with_attribute<core::http::body, &core::request::body>;
    
    class with_header {
        core::http::header_name name;
        detail::matcher<core::http::header_value> match;
        
    public:
        template <typename ExpectedType>
        with_header(core::http::header_name const & name, ExpectedType values) :
            name(name), match(values) {}
        
        bool operator ()(core::request const & request) const;
    };
}
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/api/condition.ipp>
#endif

#endif
