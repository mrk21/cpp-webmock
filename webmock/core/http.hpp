#ifndef WEBMOCK_CORE_HTTP_HPP
#define WEBMOCK_CORE_HTTP_HPP

#include <iostream>
#include <vector>
#include <map>
#include <cctype>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <webmock/util/ci_value_base.hpp>
#include <webmock/util/uri_parser.hpp>

namespace webmock { namespace core { namespace http {
    struct method: public util::ci_value_base<method> {
        using util::ci_value_base<method>::ci_value_base;
        using util::ci_value_base<method>::operator =;
        
        operator std::string() const {
            std::string result;
            for (auto && c: this->data) {
                result.push_back(std::toupper(c));
            }
            return result;
        }
    };
    
    struct header_name: public util::ci_value_base<header_name> {
        using util::ci_value_base<header_name>::ci_value_base;
        using util::ci_value_base<header_name>::operator =;
        
        operator std::string() const {
            std::vector<ci_string> list;
            boost::split(list, this->data, boost::is_any_of("-"));
            
            for (auto && str: list) {
                str[0] = std::toupper(str[0]);
                for (uint32_t i=1; i < str.size(); ++i) {
                    str[i] = std::tolower(str[i]);
                }
            }
            return boost::join(list, "-").c_str();
        }
    };
    using headers = std::multimap<header_name, std::string>;
    
    struct status: public util::ci_value_base<status> {
        using util::ci_value_base<status>::ci_value_base;
        using util::ci_value_base<status>::operator =;
        
        status() : ci_value_base("200") {}
        status(unsigned int v) : ci_value_base(boost::lexical_cast<std::string>(v)) {}
        
        operator unsigned int() const {
            return boost::lexical_cast<unsigned int>(this->data.c_str());
        }
    };
    
    class url:
        boost::equality_comparable<url, url>,
        boost::less_than_comparable<url, url>
    {
        struct ci_component: public util::ci_value_base<ci_component> {
            using util::ci_value_base<ci_component>::ci_value_base;
            using util::ci_value_base<ci_component>::operator =;
            
            operator std::string() const {
                std::string result;
                for (auto && c: this->data) {
                    result.push_back(std::tolower(c));
                }
                return result;
            }
        };
        
        using query_type = std::map<std::string, boost::optional<std::string>>;
        
        ci_component host;
        std::string path;
        query_type query;
        
        boost::optional<ci_component> scheme;
        boost::optional<std::string> userinfo;
        boost::optional<uint16_t> port;
        bool is_default_port = false;
        
    public:
        url() = default;
        url(char const * value) : url(std::string(value)) {}
        url(std::string const & value) {
            util::uri_parser result(value);
            
            this->host = result.host;
            this->path = result.path.empty() ? "/" : result.path;
            
            if (!result.query.empty()) {
                std::vector<std::string> param_entries;
                boost::split(param_entries, result.query, boost::is_any_of("&"));
                for (auto && param_entry: param_entries) {
                    std::vector<std::string> param;
                    boost::split(param, param_entry, boost::is_any_of("="));
                    if (param.size() == 1) this->query[param[0]] = boost::none;
                    else                   this->query[param[0]] = param[1];
                }
            }
            
            if (!result.scheme.empty()) this->scheme = result.scheme;
            if (!result.userinfo.empty()) this->userinfo = result.userinfo;
            if (!result.port.empty()) this->port = boost::lexical_cast<uint16_t>(result.port);
            
            if (this->scheme) {
                if (!this->port) {
                    if      (*this->scheme == "http" ) this->port = 80;
                    else if (*this->scheme == "https") this->port = 443;
                }
                this->is_default_port =
                    (*this->scheme == "http"  && *this->port == 80 ) ||
                    (*this->scheme == "https" && *this->port == 443);
            }
        }
        
        operator std::string() const {
            std::ostringstream oss;
            if (this->scheme) oss << *this->scheme << ":";
            
            oss << "//";
            if (this->userinfo) oss << *this->userinfo << "@";
            oss << this->host;
            if (this->port && !this->is_default_port) oss << ":" << *this->port;
            
            oss << this->path;
            
            if (!this->query.empty()) {
                std::vector<std::string> param_entries;
                for (auto && param: this->query) {
                    if (param.second) {
                        param_entries.push_back(param.first +"="+ *param.second);
                    }
                    else {
                        param_entries.push_back(param.first);
                    }
                }
                oss << "?" << boost::join(param_entries, "&");
            }
            
            return oss.str();
        }
        
        friend bool operator ==(url const & lop, url const & rop) {
            return static_cast<std::string>(lop)
                == static_cast<std::string>(rop);
        }
        
        friend bool operator <(url const & lop, url const & rop) {
            return static_cast<std::string>(lop)
                 < static_cast<std::string>(rop);
        }
        
        friend std::ostream & operator <<(std::ostream & lop, url const & rop) {
            return lop << static_cast<std::string>(rop);
        }
    };
}}}

#endif
