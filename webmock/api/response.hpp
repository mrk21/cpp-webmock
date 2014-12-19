#ifndef WEBMOCK_API_RESPONSE_HPP
#define WEBMOCK_API_RESPONSE_HPP

#include <webmock/api/detail/response.hpp>

namespace webmock { namespace api {
inline namespace directive {
    inline detail::static_response a_response(core::response const & response = {}) {
        return {response};
    }
    
    inline detail::dynamic_response a_response(detail::dynamic_response::generator_type generator) {
        return {generator};
    }
    
    template <typename Exception, typename... Types>
    detail::error_response an_error(Types... args) {
        return {[=](auto &&) -> core::response {
            throw Exception(args...);
        }};
    }
    
    template <typename T>
    detail::error_response an_error(T value) {
        return {[=](auto &&) -> core::response {
            throw value;
        }};
    }
}
}}

#endif
