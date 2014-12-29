#ifndef WEBMOCK_CORE_STUB_REGISTRY_HPP
#define WEBMOCK_CORE_STUB_REGISTRY_HPP

#include <webmock/core/request.hpp>
#include <webmock/core/response.hpp>
#include <webmock/core/stub.hpp>
#include <webmock/core/condition_list.hpp>
#include <vector>
#include <deque>
#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

namespace webmock { namespace core {
    class stub_registry {
        std::deque<stub> stubs;
        std::vector<request> request_history;
        
    public:
        stub & add(core::stub const & stub) {
            this->stubs.push_front(stub);
            return this->stubs.front();
        }
        
        void reset() {
            this->stubs.clear();
            this->request_history.clear();
        }
        
        boost::optional<response> access(core::request const & request) {
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
}}

#endif
