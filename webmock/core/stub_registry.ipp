#include <webmock/config.hpp>
#include <webmock/core/stub_registry.hpp>
#include <webmock/core/request.hpp>
#include <webmock/core/response.hpp>
#include <webmock/core/stub.hpp>
#include <webmock/core/condition_list.hpp>
#include <boost/range/algorithm.hpp>

namespace webmock { namespace core {
    WEBMOCK_INLINE stub & stub_registry::add(core::stub const & stub) {
        this->stubs.push_front(stub);
        return this->stubs.front();
    }
    
    WEBMOCK_INLINE void stub_registry::reset() {
        this->stubs.clear();
        this->request_history.clear();
    }
    
    WEBMOCK_INLINE boost::optional<response> stub_registry::access(core::request const & request) {
        std::lock_guard<std::mutex> lock(access_mutex);
        
        this->request_history.push_back(request);
        for (auto && stub: this->stubs) {
            if (stub.match(request)) return stub.get_response(request);
        }
        return boost::none;
    }
    
    WEBMOCK_INLINE std::size_t stub_registry::count_requests(condition_list const & conditions) const {
        return boost::count_if(this->request_history, [&conditions](auto && req){
            return conditions.match(req);
        });
    }
}}
