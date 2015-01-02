#ifndef WEBMOCK_API_DETAIL_REGISTRY_HPP
#define WEBMOCK_API_DETAIL_REGISTRY_HPP

#include <webmock/core/request.hpp>
#include <webmock/core/stub_registry.hpp>
#include <functional>

namespace webmock { namespace api { namespace detail {
    struct application {
        using stub_not_found_callback_type = std::function<void(core::request const &)>;
        
        core::stub_registry registry;
        
        struct {
            struct {
                stub_not_found_callback_type callback;
                bool is_connecting_to_net = false;
            } stub_not_found;
        } config;
    };
    
    inline application & app() {
        static application instance;
        return instance;
    }
}}}

#endif
