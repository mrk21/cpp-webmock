#ifndef WEBMOCK_DIRECTIVE_MOCK_HPP
#define WEBMOCK_DIRECTIVE_MOCK_HPP

#include <webmock/directive/detail/mock.hpp>

namespace webmock { namespace directive {
    class mock: public detail::mock_base<mock> {
    public:
        using detail::mock_base<mock>::mock_base;
    };
}}

#endif
