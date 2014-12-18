#ifndef WEBMOCK_DIRECTIVE_MOCK_HPP
#define WEBMOCK_DIRECTIVE_MOCK_HPP

#include <webmock/directive/registry.hpp>
#include <webmock/directive/condition.hpp>
#include <boost/optional.hpp>

namespace webmock { namespace directive {
    class mock {
    protected:
        using condition_type = condition_list::condition_type;
        
        condition_list conditions;
        stub_registry & registry;
        boost::optional<std::size_t> mutable cached_count;
        
    public:
        mock(
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) : conditions({url}), registry(registry) {
            this->after_initialize();
        }
        
        mock(
            with_method const & method,
            with_url const & url,
            stub_registry & registry = directive::registry()
        ) : conditions({method, url}), registry(registry) {
            this->after_initialize();
        }
        
        std::size_t count() const {
            if (!this->cached_count) {
                this->cached_count = this->registry.count_requests(this->conditions);
            }
            return *this->cached_count;
        }
        
        operator std::size_t() const {
            return this->count();
        }
        
        mock & operator <<(condition_type rop) & {
            this->conditions.add(rop);
            return *this;
        }
        
        mock operator <<(condition_type rop) && {
            this->conditions.add(rop);
            return *this;
        }
        
    protected:
        virtual void after_initialize() {}
    };
}}

#endif
