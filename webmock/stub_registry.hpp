#ifndef WEBMOCK_STUB_REGISTRY_HPP
#define WEBMOCK_STUB_REGISTRY_HPP

#include <webmock/request.hpp>
#include <webmock/response.hpp>
#include <webmock/stub.hpp>
#include <webmock/condition_list.hpp>
#include <vector>
#include <boost/optional.hpp>

namespace webmock {
    class stub_registry {
        std::vector<stub> stubs;
        std::vector<request> request_history;
        
    public:
        void add(webmock::stub const & stub);
        boost::optional<response> access(webmock::request const & request);
        std::size_t count_requests(condition_list const & conditions) const;
    };
}

#endif
