#ifndef WEBMOCK_DIRECTIVE_STUB_HPP
#define WEBMOCK_DIRECTIVE_STUB_HPP

#include <webmock/stub.hpp>
#include <webmock/directive/registry.hpp>
#include <webmock/directive/condition.hpp>
#include <webmock/directive/response.hpp>

namespace webmock { namespace directive {
    inline webmock::stub & stub(with_url url) {
        return registry().add({{url}});
    }
    
    inline webmock::stub & stub(with_method method, with_url url) {
        return registry().add({{method, url}});
    }
    
    template <typename T>
    webmock::stub & operator <<(webmock::stub & rop, response_base<T> const & lop) {
        return rop.add_sequence(lop);
    }
    
    inline webmock::stub & operator <<(webmock::stub & rop, condition_list::condition_type lop) {
        return rop.add_condition(lop);
    }
}}

#endif
