#ifndef WEBMOCK_RESPONSE_SEQUENCE_HPP
#define WEBMOCK_RESPONSE_SEQUENCE_HPP

#include <webmock/response.hpp>
#include <webmock/request.hpp>
#include <functional>

namespace webmock {
    class response_sequence {
    public:
        using response_generator_type = std::function<response(request const &)>;
        
    private:
        std::size_t count;
        response_generator_type response_generator;
        
    public:
        response_sequence(std::size_t count, response_generator_type response_generator);
        bool is_end() const;
        response get_response(webmock::request const & request);
    };
}

#endif
