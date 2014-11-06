#ifndef BOOST_NETWORK_MOCK_REQUEST_MANAGER_HPP
#define BOOST_NETWORK_MOCK_REQUEST_MANAGER_HPP

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/mock/request_handler.hpp>

namespace boost { namespace network { namespace mock {
    template <typename Tag>
    class request_manager {
    public:
        struct id_type {
            std::string method;
            std::string uri;
            
            bool operator <(id_type const & rop) const {
                return (this->method + this->uri) < (rop.method + rop.uri);
            }
        };
        
        using response_type = http::basic_response<Tag>;
        using request_type = http::basic_request<Tag>;
        using handler_type = request_handler<Tag>;
        
        std::shared_ptr<handler_type> & operator [](id_type id) {
            return this->handlers[id];
        }
        
    protected:
        std::map<id_type, std::shared_ptr<handler_type>> handlers;
    };
    
    template <typename Tag>
    inline request_manager<Tag> & get_request_manager() {
        static request_manager<Tag> instance;
        return instance;
    }
    
    template <typename Tag>
    inline auto simple_mock(
        std::string const & method,
        std::string const & uri,
        typename simple_request_handler<Tag>::callback_type callback
    ) {
        auto result = std::make_shared<simple_request_handler<Tag>>(callback);
        mock::get_request_manager<Tag>()[{method, uri}] = result;
        return result;
    }
}}}

#endif
