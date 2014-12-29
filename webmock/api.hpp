#ifndef WEBMOCK_API_HPP
#define WEBMOCK_API_HPP

#include <webmock/api/stub.hpp>
#include <webmock/api/request.hpp>
#include <webmock/api/condition.hpp>
#include <webmock/api/response.hpp>

namespace webmock {
    using core::request;
    using core::response;
}

namespace webmock { namespace api {
    inline void reset() {
        detail::registry().reset();
    }
}}

#endif
