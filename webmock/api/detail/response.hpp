#ifndef WEBMOCK_API_DETAIL_RESPONSE_HPP
#define WEBMOCK_API_DETAIL_RESPONSE_HPP

#include <webmock/core/response_sequence.hpp>

namespace webmock { namespace api { namespace detail {
    template <typename Derived>
    class response_base {
    public:
        using generator_type = core::response_sequence::response_generator_type;
        
    protected:
        std::size_t count = 1;
        generator_type generator = [](auto &&){ return core::response{}; };
        
    public:
        response_base() = default;
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
}}}

#endif
