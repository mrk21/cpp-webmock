#ifndef WEBMOCK_CORE_RESPONSE_SEQUENCE_HPP
#define WEBMOCK_CORE_RESPONSE_SEQUENCE_HPP

#include <webmock/core/response.hpp>
#include <webmock/core/request.hpp>
#include <functional>

namespace webmock { namespace core {
    class response_sequence {
    public:
        using response_generator_type = std::function<response(request const &)>;
        
    private:
        std::size_t count;
        response_generator_type response_generator;
        
    public:
        response_sequence(std::size_t count, response_generator_type response_generator)
            : count(count), response_generator(response_generator) {}
        
        bool is_end() const {
            return this->count == 0;
        }
        
        response get_response(core::request const & request) {
            if (this->count > 0) --this->count;
            return this->response_generator(request);
        }
    };
}}

#endif
