#ifndef WEBMOCK_DIRECTIVE_DETAIL_RESPONSE_HPP
#define WEBMOCK_DIRECTIVE_DETAIL_RESPONSE_HPP

#include <webmock/core/response_sequence.hpp>

namespace webmock { namespace directive { namespace detail {
    template <typename Derived>
    class response_base {
    public:
        using generator_type = core::response_sequence::response_generator_type;
        
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
        
        operator core::response_sequence() const {
            return {this->count, this->generator};
        }
    };
    
    struct static_response: public response_base<static_response> {
        static_response(core::response const & response) :
            response_base<static_response>([=](auto &&){
                return response;
            })
        {}
    };
    
    struct dynamic_response: public response_base<dynamic_response> {
        using response_base<dynamic_response>::response_base;
    };
    
    struct error_response: public response_base<error_response> {
        using response_base<error_response>::response_base;
    };
}}}

#endif
