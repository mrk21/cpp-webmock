#ifndef WEBMOCK_DIRECTIVE_REGISTRY_HPP
#define WEBMOCK_DIRECTIVE_REGISTRY_HPP

#include <webmock/stub_registry.hpp>

namespace webmock { namespace directive {
    inline stub_registry & registry() {
        static stub_registry instance;
        return instance;
    }
}}

#endif
