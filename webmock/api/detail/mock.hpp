#ifndef WEBMOCK_API_DETAIL_MOCK_HPP
#define WEBMOCK_API_DETAIL_MOCK_HPP

#include <webmock/api/application.hpp>
#include <webmock/api/condition.hpp>
#include <initializer_list>

namespace webmock { namespace api { namespace detail {
    template <typename Derived>
    class mock_base {
    protected:
        using condition_type = core::condition_list::condition_type;
        
        core::condition_list conditions_;
        application & app;
        
    public:
        mock_base(
            application & app = api::app()
        ) :
            app(app)
        {}
        
        mock_base(
            with_url const & url,
            application & app = api::app()
        ) :
            conditions_({url}), app(app)
        {}
        
        mock_base(
            with_method const & method,
            with_url const & url,
            application & app = api::app()
        ) :
            conditions_({method, url}), app(app)
        {}
        
        std::size_t count() const {
            return this->app.registry.count_requests(this->conditions_);
        }
        
        operator std::size_t() const {
            return this->count();
        }
        
        template <typename... Types>
        auto conditions(Types... conditions) {
            this->add_conditions({conditions...});
            return this->derived();
        }
        
        auto operator <<(condition_type rop) {
            return this->conditions(rop);
        }
        
    protected:
        Derived & derived() & {
            return *static_cast<Derived *>(this);
        }
        
        Derived && derived() && {
            return std::move(*static_cast<Derived *>(this));
        }
        
        virtual void add_conditions(std::initializer_list<condition_type> conditions) {
            for (auto && c: conditions) this->conditions_.add(c);
        }
    };
}}}

#endif
