#ifndef WEBMOCK_API_STUB_HPP
#define WEBMOCK_API_STUB_HPP

#include <webmock/core/stub.hpp>
#include <webmock/api/detail/mock.hpp>
#include <webmock/api/response.hpp>

namespace webmock { namespace api {
inline namespace directive {
    class a_stub: public detail::mock_base<a_stub> {
    protected:
        core::stub & data;
        
    public:
        a_stub(
            application & app = api::app()
        ) :
            mock_base(app),
            data(this->init_data())
        {}
        
        a_stub(
            with_url const & url,
            application & app = api::app()
        ) :
            mock_base(url, app),
            data(this->init_data())
        {}
        
        a_stub(
            with_method const & method,
            with_url const & url,
            application & app = api::app()
        ) :
            mock_base(method, url, app),
            data(this->init_data())
        {}
        
        template <typename... Types>
        auto returns(Types... sequences) {
            this->add_sequences({sequences...});
            return this->derived();
        }
        
        friend auto operator <<(a_stub & lop, core::response_sequence const & rop) {
            return lop.returns(rop);
        }
        
        friend auto operator <<(a_stub && lop, core::response_sequence const & rop) {
            return lop.returns(rop);
        }
        
    protected:
        virtual void add_conditions(std::initializer_list<condition_type> conditions) {
            mock_base::add_conditions(conditions);
            for (auto && c: conditions) this->data.add_condition(c);
        }
        
        virtual void add_sequences(std::initializer_list<core::response_sequence> sequences) {
            for (auto && s: sequences) this->data.add_sequence(s);
        }
        
    private:
        core::stub & init_data() {
            return this->app.registry.add({this->conditions_});
        }
    };
}
}}

#endif
