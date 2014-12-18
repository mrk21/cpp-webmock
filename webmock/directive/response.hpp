#ifndef WEBMOCK_DIRECTIVE_RESPONSE_HPP
#define WEBMOCK_DIRECTIVE_RESPONSE_HPP

#include <webmock/directive/detail/response.hpp>

namespace webmock { namespace directive {
    inline detail::static_response response(core::response const & response) {
        return {response};
    }
    
    inline detail::dynamic_response response(detail::dynamic_response::generator_type generator) {
        return {generator};
    }
    
    template <typename Exception, typename... Types>
    detail::error_response error(Types... args) {
        return {[=](auto &&) -> core::response {
            throw Exception(args...);
        }};
    }
    
    template <typename T>
    detail::error_response error(T value) {
        return {[=](auto &&) -> core::response {
            throw value;
        }};
    }
}}

#endif
