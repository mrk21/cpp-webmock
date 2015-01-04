#ifndef WEBMOCK_CORE_HTTP_HPP
#define WEBMOCK_CORE_HTTP_HPP

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <cctype>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <webmock/util/ci_value_base.hpp>
#include <webmock/util/uri_parser.hpp>
#include <webmock/util/percent_encoding.hpp>

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
    public:
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
        
        using port_type = uint16_t;
        using path_type = std::vector<std::string>;
        using query_type = std::map<std::string, boost::optional<std::string>>;
        
    private:
        struct non_encoding_policy {
            template <typename T>
            std::string operator ()(T const & value) {
                return boost::lexical_cast<std::string>(value);
            }
        };
        
        struct percent_encoding_policy {
            template <typename T>
            std::string operator ()(T const & value) {
                return util::percent_encoding::encode(
                    boost::lexical_cast<std::string>(value)
                );
            }
        };
        
        boost::optional<ci_component> scheme;
        boost::optional<std::string> userinfo;
        boost::optional<ci_component> host;
        boost::optional<port_type> port;
        path_type path;
        boost::optional<query_type> query;
        
    public:
        url() = default;
        url(char const * url_str) : url(std::string(url_str)) {}
        url(std::string const & url_str) {
            using pe = util::percent_encoding;
            util::uri_parser uri(url_str);
            
            if (!uri.scheme.empty()) this->scheme = pe::decode(uri.scheme);
            if (!uri.userinfo.empty()) this->userinfo = pe::decode(uri.userinfo);
            if (!uri.host.empty()) this->host = pe::decode(uri.host);
            if (!uri.port.empty()) this->port = boost::lexical_cast<port_type>(pe::decode(uri.port));
            else                   this->port = this->default_port();
            {
                path_type path;
                boost::split(path, uri.path, boost::is_any_of("/"));
                for (auto && component: path) {
                    if (component.empty() || component == ".") {
                        continue;
                    }
                    else if (component == "..") {
                        if (!this->path.empty()) {
                           this->path.pop_back();
                        }
                    }
                    else {
                        this->path.push_back(pe::decode(component));
                    }
                }
            }
            if (!uri.query.empty()) {
                this->query = query_type{};
                std::vector<std::string> params;
                boost::split(params, uri.query, boost::is_any_of("&"));
                for (auto && param: params) {
                    std::vector<std::string> param_pair;
                    boost::split(param_pair, param, boost::is_any_of("="));
                    auto && key = pe::decode(param_pair[0]);
                    if (param_pair.size() == 1) (*this->query)[key] = boost::none;
                    else                        (*this->query)[key] = pe::decode(param_pair[1]);
                }
            }
        }
        
        operator std::string() const {
            return this->to_string<non_encoding_policy>();
        }
        
        friend bool operator ==(url const & lop, url const & rop) {
            bool result =
                std::tie(lop.userinfo, lop.host, lop.path, lop.query) ==
                std::tie(rop.userinfo, rop.host, rop.path, rop.query);
            
            if (!lop.scheme || !rop.scheme) {
                if (!lop.port || !rop.port) return result;
                return result && (lop.port == rop.port);
            }
            return result && (
                std::tie(lop.scheme, lop.port) ==
                std::tie(rop.scheme, rop.port)
            );
        }
        
        friend bool operator <(url const & lop, url const & rop) {
            return
                lop.to_string<non_encoding_policy>() <
                rop.to_string<non_encoding_policy>();
        }
        
        friend std::ostream & operator <<(std::ostream & lop, url const & rop) {
            return lop << rop.to_string<percent_encoding_policy>();
        }
        
    private:
        boost::optional<port_type> default_port() const {
            if (this->scheme == ci_component("http")) return 80;
            if (this->scheme == ci_component("https")) return 443;
            return boost::none;
        }
        
        template <typename EncodingPolicy>
        std::string to_string() const {
            std::ostringstream oss;
            EncodingPolicy encode;
            
            if (this->scheme) oss << encode(*this->scheme) << ":";
            if (this->host) {
                oss << "//";
                if (this->userinfo) oss << encode(*this->userinfo) << "@";
                oss << encode(*this->host);
                if (this->port != this->default_port()) oss << ":" << encode(*this->port);
            }
            {
                path_type encoded(this->path.size());
                boost::transform(this->path, encoded.begin(), encode);
                oss << "/" << boost::join(encoded, "/");
            }
            if (this->query) {
                std::vector<std::string> params;
                for (auto && param_pair: *this->query) {
                    if (param_pair.second) {
                        params.push_back(encode(param_pair.first) +"="+ encode(*param_pair.second));
                    }
                    else {
                        params.push_back(encode(param_pair.first));
                    }
                }
                oss << "?" << boost::join(params, "&");
            }
            return oss.str();
        }
    };
}}}

#endif
