#ifndef WEBMOCK_DIRECTIVE_RESPONSE_HPP
#define WEBMOCK_DIRECTIVE_RESPONSE_HPP

#include <webmock/response_sequence.hpp>

namespace webmock { namespace directive {
    template <typename Derived>
    class response_base {
    public:
        using generator_type = response_sequence::response_generator_type;
        
    protected:
        std::size_t count = 1;
        generator_type generator;
        
    public:
        response_base(generator_type generator) : generator(generator) {}
        
        Derived & times(std::size_t count) {
            this->count = count;
            return static_cast<Derived &>(*this);
        }
        
        Derived & operator *(std::size_t rop) {
            return this->times(rop);
        }
        
        operator response_sequence() const {
            return {this->count, this->generator};
        }
    };
    
    struct static_response: public response_base<static_response> {
        static_response(webmock::response const & response) :
            response_base<static_response>([=](auto &&){
                return response;
            })
        {}
    };
    
    inline static_response response(webmock::response const & response) {
        return {response};
    }
    
    
    struct dynamic_response: public response_base<dynamic_response> {
        using response_base<dynamic_response>::response_base;
    };

    inline dynamic_response response(dynamic_response::generator_type generator) {
        return {generator};
    }
    
    
    struct error_response: public response_base<error_response> {
        using response_base<error_response>::response_base;
    };
    
    template <typename Exception, typename... Types>
    error_response error(Types... args) {
        return {[=](auto &&) -> webmock::response {
            throw Exception(args...);
        }};
    }
    
    template <typename T>
    error_response error(T value) {
        return {[=](auto &&) -> webmock::response {
            throw value;
        }};
    }
}}

#endif
