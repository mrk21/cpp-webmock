#ifndef WEBMOCK_API_HPP
#define WEBMOCK_API_HPP

#include <webmock/api/stub.hpp>
#include <webmock/api/request.hpp>
#include <webmock/api/condition.hpp>
#include <webmock/api/response.hpp>
#include <webmock/api/detail/application.hpp>

namespace webmock {
    using core::request;
    using core::response;
}

namespace webmock { namespace api {
    inline void reset() {
        detail::app().registry.reset();
    }
    
    inline void stub_not_found_callback(detail::application::stub_not_found_callback_type callback = nullptr) {
        detail::app().config.stub_not_found_callback = callback;
    }
}}

#endif
