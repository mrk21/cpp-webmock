#include <tuple>
#include <cctype>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <webmock/config.hpp>
#include <webmock/core/http.hpp>
#include <webmock/util/uri_parser.hpp>
#include <webmock/util/percent_encoding.hpp>

namespace webmock { namespace core { namespace http {
// ## method
    WEBMOCK_INLINE method::operator std::string() const {
        std::string result;
        for (auto && c: this->data) {
            result.push_back(std::toupper(c));
        }
        return result;
    }
    
// ## header_name
    WEBMOCK_INLINE header_name::operator std::string() const {
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
    
// ## status
    WEBMOCK_INLINE status::status() : ci_value_base("200") {}
    WEBMOCK_INLINE status::status(unsigned int v) :
        ci_value_base(boost::lexical_cast<std::string>(v)) {}
    
    WEBMOCK_INLINE status::operator unsigned int() const {
        return boost::lexical_cast<unsigned int>(this->data.c_str());
    }
    
// ## url::pimpl
    struct url::pimpl {
        struct non_encoding_policy {
            template <typename T>
            std::string operator ()(T const & value) {
                return boost::lexical_cast<std::string>(value);
            }
        };
        
        struct encode_policy {
            template <typename T>
            std::string operator ()(T const & value) {
                return util::percent_encoding::encode(
                    boost::lexical_cast<std::string>(value)
                );
            }
        };
        
        struct decode_policy {
            template <typename T>
            std::string operator ()(T const & value) {
                return util::percent_encoding::decode(
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
        
        boost::optional<port_type> default_port() const {
            if (this->scheme == ci_component("http")) return 80;
            if (this->scheme == ci_component("https")) return 443;
            return boost::none;
        }
        
        void reset() {
            this->scheme = boost::none;
            this->userinfo = boost::none;
            this->host = boost::none;
            this->port = boost::none;
            this->path.clear();
            this->query = boost::none;
        }
        
        template <typename EncodingPolicy>
        void form_string(std::string const & url_str) {
            EncodingPolicy encoding;
            util::uri_parser uri(url_str);
            this->reset();
            
            if (!uri.scheme.empty()) this->scheme = encoding(uri.scheme);
            if (!uri.userinfo.empty()) this->userinfo = encoding(uri.userinfo);
            if (!uri.host.empty()) this->host = encoding(uri.host);
            if (!uri.port.empty()) this->port = boost::lexical_cast<port_type>(encoding(uri.port));
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
                        this->path.push_back(encoding(component));
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
                    auto && key = encoding(param_pair[0]);
                    if (param_pair.size() == 1) (*this->query)[key] = boost::none;
                    else                        (*this->query)[key] = encoding(param_pair[1]);
                }
            }
        }
        
        template <typename EncodingPolicy>
        std::string to_string() const {
            EncodingPolicy encoding;
            std::ostringstream oss;
            
            if (this->scheme) oss << encoding(*this->scheme) << ":";
            if (this->host) {
                oss << "//";
                if (this->userinfo) oss << encoding(*this->userinfo) << "@";
                oss << encoding(*this->host);
                if (this->port != this->default_port()) oss << ":" << encoding(*this->port);
            }
            {
                path_type processed_path(this->path.size());
                boost::transform(this->path, processed_path.begin(), encoding);
                oss << "/" << boost::join(processed_path, "/");
            }
            if (this->query) {
                std::vector<std::string> params;
                for (auto && param_pair: *this->query) {
                    if (param_pair.second) {
                        params.push_back(encoding(param_pair.first) +"="+ encoding(*param_pair.second));
                    }
                    else {
                        params.push_back(encoding(param_pair.first));
                    }
                }
                oss << "?" << boost::join(params, "&");
            }
            return oss.str();
        }
    };
    
// ## url::ci_component
    WEBMOCK_INLINE url::ci_component::operator std::string() const {
        std::string result;
        for (auto && c: this->data) {
            result.push_back(std::tolower(c));
        }
        return result;
    }
    
// ## url
    WEBMOCK_INLINE url::url() :
        impl(std::make_shared<pimpl>())
    {}
    
    WEBMOCK_INLINE url::url(char const * url_str, bool is_encoded) :
        url(std::string(url_str), is_encoded)
    {}
    
    WEBMOCK_INLINE url::url(std::string const & url_str, bool is_encoded) :
        impl(std::make_shared<pimpl>())
    {
        if (is_encoded) this->impl->form_string<pimpl::decode_policy>(url_str);
        else this->impl->form_string<pimpl::non_encoding_policy>(url_str);
    }
    
    WEBMOCK_INLINE url::url(url const & from) :
        impl(std::make_shared<pimpl>(*from.impl))
    {}
    
    WEBMOCK_INLINE url & url::operator =(url const & rop) {
        this->impl = std::make_shared<pimpl>(*rop.impl);
        return *this;
    }
    
    WEBMOCK_INLINE url & url::set_encoded(std::string const & url_str) {
        this->impl->form_string<pimpl::decode_policy>(url_str);
        return *this;
    }
    
    WEBMOCK_INLINE url::operator std::string() const {
        return this->impl->to_string<pimpl::non_encoding_policy>();
        return *this;
    }
    
    WEBMOCK_INLINE bool operator ==(url const & lop, url const & rop) {
        bool result =
            std::tie(lop.impl->userinfo, lop.impl->host, lop.impl->path, lop.impl->query) ==
            std::tie(rop.impl->userinfo, rop.impl->host, rop.impl->path, rop.impl->query);
        
        if (!lop.impl->scheme || !rop.impl->scheme) {
            if (!lop.impl->port || !rop.impl->port) return result;
            return result && (lop.impl->port == rop.impl->port);
        }
        return result && (
            std::tie(lop.impl->scheme, lop.impl->port) ==
            std::tie(rop.impl->scheme, rop.impl->port)
        );
    }
    
    WEBMOCK_INLINE bool operator <(url const & lop, url const & rop) {
        return
            lop.impl->to_string<url::pimpl::non_encoding_policy>() <
            rop.impl->to_string<url::pimpl::non_encoding_policy>();
    }
    
    WEBMOCK_INLINE std::ostream & operator <<(std::ostream & lop, url const & rop) {
        return lop << rop.impl->to_string<url::pimpl::encode_policy>();
    }
}}}
