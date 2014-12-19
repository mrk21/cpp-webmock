#ifndef WEBMOCK_API_MOCK_HPP
#define WEBMOCK_API_MOCK_HPP

#include <webmock/api/detail/mock.hpp>

namespace webmock { namespace api {
    class mock: public detail::mock_base<mock> {
    public:
        using detail::mock_base<mock>::mock_base;
    };
}}

#endif
