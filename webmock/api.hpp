#ifndef WEBMOCK_API_HPP
#define WEBMOCK_API_HPP

#include <webmock/api/stub.hpp>
#include <webmock/api/request.hpp>
#include <webmock/api/condition.hpp>
#include <webmock/api/response.hpp>
#include <webmock/api/application.hpp>

namespace webmock {
    using core::request;
    using core::response;
}

namespace webmock { namespace api {
    inline void reset() {
        app().registry.reset();
    }
    
    inline void stub_not_found_callback(application::stub_not_found_callback_type callback = nullptr) {
        app().config.stub_not_found.callback = callback;
    }
    
    inline void allow_connecting_to_net() {
        app().config.stub_not_found.is_connecting_to_net = true;
    }
    
    inline void disallow_connecting_to_net() {
        app().config.stub_not_found.is_connecting_to_net = false;
    }
}}

#endif
