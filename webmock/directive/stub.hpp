#ifndef WEBMOCK_DIRECTIVE_STUB_HPP
#define WEBMOCK_DIRECTIVE_STUB_HPP

#include <webmock/core/stub.hpp>
#include <webmock/directive/detail/mock.hpp>
#include <webmock/directive/response.hpp>

namespace webmock { namespace directive {
    class stub: public detail::mock_base<stub> {
    protected:
        core::stub & data;
        
    public:
        stub(
            core::stub_registry & registry = detail::registry()
        ) :
            mock_base(registry),
            data(this->init_data())
        {}
        
        stub(
            with_url const & url,
            core::stub_registry & registry = detail::registry()
        ) :
            mock_base(url, registry),
            data(this->init_data())
        {}
        
        stub(
            with_method const & method,
            with_url const & url,
            core::stub_registry & registry = detail::registry()
        ) :
            mock_base(method, url, registry),
            data(this->init_data())
        {}
        
        template <typename... Types>
        auto returns(Types... sequences) {
            this->add_sequences({sequences...});
            return this->derived();
        }
        
        friend auto operator <<(stub & lop, core::response_sequence const & rop) {
            return lop.returns(rop);
        }
        
        friend auto operator <<(stub && lop, core::response_sequence const & rop) {
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
            return this->registry.add({this->conditions_});
        }
    };
}}

#endif
