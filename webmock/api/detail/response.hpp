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
    
    struct static_response: public response_base<static_response> {
        static_response(core::response const & response = {}) :
            response_base<static_response>([=](auto &&){
                return response;
            })
        {}
        
        auto status(std::string const & value) {
            this->set([=](auto && r){ r.status = value; });
            return this->ref();
        }
        
        auto body(std::string const & value) {
            this->set([=](auto && r){ r.body = value; });
            return this->ref();
        }
        
        auto header(std::string const & key, std::string const & value) {
            this->set([=](auto && r){ r.headers.insert({key,value}); });
            return this->ref();
        }
        
        auto header(core::response::header_type const & values) {
            this->set([=](auto && r){
                for (auto && v: values) {
                    r.headers.insert(v);
                }
            });
            return this->ref();
        }
        
    private:
        void set(std::function<void(core::response &)> block) {
            this->generator = [block, prev = this->generator](auto && req){
                auto && res = prev(req);
                if (block) block(res);
                return res;
            };
        }
        
        static_response &  ref() &  { return *this; }
        static_response && ref() && { return std::move(*this); }
    };
    
    struct dynamic_response: public response_base<dynamic_response> {
        using response_base<dynamic_response>::response_base;
    };
    
    struct error_response: public response_base<error_response> {
        using response_base<error_response>::response_base;
    };
}}}

#endif
