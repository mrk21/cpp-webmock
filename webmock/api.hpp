#ifndef WEBMOCK_API_HPP
#define WEBMOCK_API_HPP

#include <webmock/api/stub.hpp>
#include <webmock/api/mock.hpp>
#include <webmock/api/condition.hpp>
#include <webmock/api/response.hpp>

namespace webmock { namespace api { namespace directive {
    using namespace condition_directive;
    using namespace response_directive;
}}}

#endif
