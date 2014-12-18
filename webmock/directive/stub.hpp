#ifndef WEBMOCK_DIRECTIVE_STUB_HPP
#define WEBMOCK_DIRECTIVE_STUB_HPP

#include <webmock/stub.hpp>
#include <webmock/directive/mock.hpp>
#include <webmock/directive/response.hpp>

namespace webmock { namespace directive {
    class stub: public mock_base<stub> {
    protected:
        webmock::stub & data;
        
    public:
        stub(
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) :
            mock_base(url, registry),
            data(this->init_data())
        {}
        
        stub(
            with_method const & method,
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) :
            mock_base(method, url, registry),
            data(this->init_data())
        {}
        
        void add_sequence(response_sequence const & sequence) {
            this->data.add_sequence(sequence);
        }
        
        void add_condition(condition_type condition) {
            mock_base::add_condition(condition);
            this->data.add_condition(condition);
        }
        
        friend stub & operator <<(stub & lop, response_sequence const & rop) {
            lop.add_sequence(rop);
            return lop;
        }
        
        friend stub && operator <<(stub && lop, response_sequence const & rop) {
            lop.add_sequence(rop);
            return std::move(lop);
        }
        
    private:
        webmock::stub & init_data() {
            return this->registry.add({this->conditions});
        }
    };
}}

#endif
