#ifndef WEBMOCK_API_MOCK_HPP
#define WEBMOCK_API_MOCK_HPP

#include <webmock/api/detail/mock.hpp>

namespace webmock { namespace api {
inline namespace directive {
    class a_request: public detail::mock_base<a_request> {
    public:
        using detail::mock_base<a_request>::mock_base;
    };
}}}

#endif
