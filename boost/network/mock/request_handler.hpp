#ifndef BOOST_NETWORK_MOCK_REQUEST_HANDLER_HPP
#define BOOST_NETWORK_MOCK_REQUEST_HANDLER_HPP

#include <string>
#include <functional>
#include <boost/network/protocol/http/response.hpp>

namespace boost { namespace network { namespace mock {
    template <typename Tag>
    struct request_handler {
        using request_type = http::basic_request<Tag>;
        using response_type = http::basic_response<Tag>;
        
        virtual response_type request(std::string const & method, request_type const & request) = 0;
    };
    
    template <typename Tag>
    struct simple_request_handler: public request_handler<Tag> {
        using base_type = request_handler<Tag>;
        using request_type = typename base_type::request_type;
        using response_type = typename base_type::response_type;
        using callback_type = std::function<response_type (std::string const &, request_type const &)>;
        
        simple_request_handler(callback_type callback) : callback(callback) {}
        
        virtual response_type request(std::string const & method, request_type const & request) {
            return this->callback(method, request);
        }
        
    protected:
        callback_type callback;
    };
}}}

#endif
