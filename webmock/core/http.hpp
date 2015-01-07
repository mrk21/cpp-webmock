#ifndef WEBMOCK_CORE_HTTP_HPP
#define WEBMOCK_CORE_HTTP_HPP

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <boost/optional.hpp>
#include <webmock/util/ci_value_base.hpp>

namespace webmock { namespace core { namespace http {
    struct method: public util::ci_value_base<method> {
        using util::ci_value_base<method>::ci_value_base;
        using util::ci_value_base<method>::operator =;
        operator std::string() const;
    };
    
    struct header_name: public util::ci_value_base<header_name> {
        using util::ci_value_base<header_name>::ci_value_base;
        using util::ci_value_base<header_name>::operator =;
        operator std::string() const;
    };
    using header_value = std::string;
    using headers = std::multimap<header_name, header_value>;
    
    struct status: public util::ci_value_base<status> {
        using util::ci_value_base<status>::ci_value_base;
        using util::ci_value_base<status>::operator =;
        
        status();
        status(unsigned int v);
        operator unsigned int() const;
    };
    
    class url:
        boost::equality_comparable<url, url>,
        boost::less_than_comparable<url, url>
    {
    public:
        struct ci_component: public util::ci_value_base<ci_component> {
            using util::ci_value_base<ci_component>::ci_value_base;
            using util::ci_value_base<ci_component>::operator =;
            operator std::string() const;
        };
        
        using port_type = uint16_t;
        using path_type = std::vector<std::string>;
        using query_type = std::map<std::string, boost::optional<std::string>>;
        
    private:
        struct pimpl;
        std::shared_ptr<pimpl> impl;
        
    public:
        url();
        url(char const * url_str, bool is_encoded = false);
        url(std::string const & url_str, bool is_encoded = false);
        
        url(url const & from);
        url & operator =(url const & rop);
        
        url & set_encoded(std::string const & url_str);
        operator std::string() const;
        
        friend bool operator ==(url const & lop, url const & rop);
        friend bool operator <(url const & lop, url const & rop);
        friend std::ostream & operator <<(std::ostream & lop, url const & rop);
    };
    
    using body = std::string;
}}}

#ifndef WEBMOCK_USE_LIBRARY
#include <webmock/core/http.ipp>
#endif

#endif
