#include <webmock/stub_registry.hpp>
#include <boost/range/algorithm.hpp>

namespace webmock {
    stub & stub_registry::add(webmock::stub const & stub) {
        this->stubs.push_back(stub);
        return this->stubs.back();
    }
    
    boost::optional<response> stub_registry::access(webmock::request const & request) {
        this->request_history.push_back(request);
        for (auto && stub: this->stubs) {
            if (stub.match(request)) return stub.get_response(request);
        }
        return boost::none;
    }
    
    std::size_t stub_registry::count_requests(condition_list const & conditions) const {
        return boost::count_if(this->request_history, [&conditions](auto && req){
            return conditions.match(req);
        });
    }
}
