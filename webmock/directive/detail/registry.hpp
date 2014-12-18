#ifndef WEBMOCK_DIRECTIVE_DETAIL_REGISTRY_HPP
#define WEBMOCK_DIRECTIVE_DETAIL_REGISTRY_HPP

#include <webmock/core/stub_registry.hpp>

namespace webmock { namespace directive { namespace detail {
    inline core::stub_registry & registry() {
        static core::stub_registry instance;
        return instance;
    }
}}}

#endif
