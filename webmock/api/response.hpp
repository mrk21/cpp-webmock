#ifndef WEBMOCK_API_RESPONSE_HPP
#define WEBMOCK_API_RESPONSE_HPP

#include <webmock/api/detail/response.hpp>
#include <webmock/core/http.hpp>

namespace webmock { namespace api {
    struct static_response: public detail::response_base<static_response> {
        static_response(core::response const & response = {}) :
            response_base<static_response>([=](auto &&){
                return response;
            })
        {}
        
        auto status(core::http::status const & value) {
            this->set([=](auto && r){ r.status = value; });
            return this->ref();
        }
        
        auto body(core::http::body const & value) {
            this->set([=](auto && r){ r.body = value; });
            return this->ref();
        }
        
        auto header(core::http::header_name const & name, core::http::header_value const & value) {
            this->set([=](auto && r){ r.headers.insert({name, value}); });
            return this->ref();
        }
        
        auto header(core::http::headers const & values) {
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
    
    struct dynamic_response: public detail::response_base<dynamic_response> {
        using detail::response_base<dynamic_response>::response_base;
    };
    
    struct error_response: public detail::response_base<error_response> {
        using detail::response_base<error_response>::response_base;
    };
    
inline namespace directive {
    inline static_response a_response(core::response const & response = {}) {
        return {response};
    }
    
    inline dynamic_response a_response(dynamic_response::generator_type generator) {
        return {generator};
    }
    
    template <typename Exception, typename... Types>
    error_response an_error(Types... args) {
        return {[=](auto &&) -> core::response {
            throw Exception(args...);
        }};
    }
    
    template <typename T>
    error_response an_error(T value) {
        return {[=](auto &&) -> core::response {
            throw value;
        }};
    }
}
}}

#endif
