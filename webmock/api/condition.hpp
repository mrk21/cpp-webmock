#ifndef WEBMOCK_API_CONDITION_HPP
#define WEBMOCK_API_CONDITION_HPP

#include <webmock/api/detail/condition.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/assign.hpp>

namespace webmock { namespace api {
    using with = core::condition_list::condition_type;
    using with_method = detail::with_string_attribute<&core::request::method>;
    using with_url = detail::with_string_attribute<&core::request::url>;
    using with_body = detail::with_string_attribute<&core::request::body>;
    
    class with_header {
        std::string key;
        detail::matcher match;
        
    public:
        template <typename ExpectedType>
        with_header(std::string const & key, ExpectedType values) :
            key(key), match(values) {}
        
        bool operator ()(core::request const & request) const {
            using namespace boost::assign;
            using namespace boost::adaptors;
            return boost::algorithm::all_of(
                request.headers.equal_range(this->key) | map_values, this->match
            );
        }
    };
}}

#endif