#ifndef WEBMOCK_CORE_STUB_REGISTRY_HPP
#define WEBMOCK_CORE_STUB_REGISTRY_HPP

#include <webmock/core/request.hpp>
#include <webmock/core/response.hpp>
#include <webmock/core/stub.hpp>
#include <webmock/core/condition_list.hpp>
#include <vector>
#include <deque>
#include <thread>
#include <boost/optional.hpp>

namespace webmock { namespace core {
    class stub_registry {
        std::deque<stub> stubs;
        std::vector<request> request_history;
        std::mutex access_mutex;
        
    public:
        stub & add(core::stub const & stub);
        void reset();
        boost::optional<response> access(core::request const & request);
        std::size_t count_requests(condition_list const & conditions) const;
    };
}}

#ifndef WEBMOCK_BUILD_LIBRARY
#include <webmock/core/stub_registry.ipp>
#endif

#endif
