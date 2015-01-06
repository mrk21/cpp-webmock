#ifndef WEBMOCK_API_CONDITION_HPP
#define WEBMOCK_API_CONDITION_HPP

#include <webmock/api/detail/condition.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/assign.hpp>

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
        
        bool operator ()(core::request const & request) const {
            using namespace boost::assign;
            using namespace boost::adaptors;
            return boost::algorithm::all_of(
                request.headers.equal_range(this->name) | map_values, this->match
            );
        }
    };
}
}}

#endif
