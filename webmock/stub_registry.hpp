#ifndef WEBMOCK_STUB_REGISTRY_HPP
#define WEBMOCK_STUB_REGISTRY_HPP

#include <webmock/request.hpp>
#include <webmock/response.hpp>
#include <webmock/stub.hpp>
#include <webmock/condition_list.hpp>
#include <vector>
#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

namespace webmock {
    class stub_registry {
        std::vector<stub> stubs;
        std::vector<request> request_history;
        
    public:
        stub & add(webmock::stub const & stub) {
            this->stubs.push_back(stub);
            return this->stubs.back();
        }
        
        boost::optional<response> access(webmock::request const & request) {
            this->request_history.push_back(request);
            for (auto && stub: this->stubs) {
                if (stub.match(request)) return stub.get_response(request);
            }
            return boost::none;
        }
        
        std::size_t count_requests(condition_list const & conditions) const {
            return boost::count_if(this->request_history, [&conditions](auto && req){
                return conditions.match(req);
            });
        }
    };
}

#endif
