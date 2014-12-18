#ifndef WEBMOCK_DIRECTIVE_MOCK_HPP
#define WEBMOCK_DIRECTIVE_MOCK_HPP

#include <webmock/directive/registry.hpp>
#include <webmock/directive/condition.hpp>
#include <boost/optional.hpp>

namespace webmock { namespace directive {
    template <typename Derived>
    class mock_base {
    protected:
        using condition_type = condition_list::condition_type;
        
        condition_list conditions;
        stub_registry & registry;
        boost::optional<std::size_t> mutable cached_count;
        
    public:
        mock_base(
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) :
            conditions({url}), registry(registry)
        {}
        
        mock_base(
            with_method const & method,
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) :
            conditions({method, url}), registry(registry)
        {}
        
        std::size_t count() const {
            if (!this->cached_count) {
                this->cached_count = this->registry.count_requests(this->conditions);
            }
            return *this->cached_count;
        }
        
        operator std::size_t() const {
            return this->count();
        }
        
        void add_condition(condition_type condition) {
            this->conditions.add(condition);
        }
        
        friend Derived & operator <<(Derived & lop, condition_type rop) {
            lop.add_condition(rop);
            return lop;
        }
        
        friend Derived && operator <<(Derived && lop, condition_type rop) {
            lop.add_condition(rop);
            return std::move(lop);
        }
    };
    
    class mock: public mock_base<mock> {
    public:
        using mock_base<mock>::mock_base;
    };
}}

#endif
