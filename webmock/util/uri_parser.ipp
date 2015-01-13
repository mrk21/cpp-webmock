#include <webmock/config.hpp>
#include <webmock/util/uri_parser.hpp>
#include <regex>
#include <tuple>

namespace webmock { namespace util {
    WEBMOCK_INLINE uri_parser::uri_parser(std::map<std::string, std::string> components) {
        this->scheme   = components["scheme"];
        this->userinfo = components["userinfo"];
        this->host     = components["host"];
        this->port     = components["port"];
        this->path     = components["path"];
        this->query    = components["query"];
        this->fragment = components["fragment"];
    }
    
    WEBMOCK_INLINE uri_parser::uri_parser(std::string const & uri) {
        static std::regex const uri_syntax(
            "^"                      // 0:uri{
                "(?:([^:/?#]+):)?"   //   1:scheme
                "(?://"              //   authority{
                    "(?:([^@]+)@)?"  //     2:userinfo
                    "([^/@?#:]+)"    //     3:host
                    "(?::(\\d+))?"   //     4:port
                ")?"                 //   }
                "([^?#]*)"           //   5:path
                "(?:\\?([^#]*))?"    //   6:query
                "(?:#(.*))?"         //   7:fragment
            "$"                      // }
        );
        std::smatch components;
        std::regex_match(uri, components, uri_syntax);
        
        this->scheme   = components[1];
        this->userinfo = components[2];
        this->host     = components[3];
        this->port     = components[4];
        this->path     = components[5];
        this->query    = components[6];
        this->fragment = components[7];
    }
    
    WEBMOCK_INLINE bool operator ==(uri_parser const & lop, uri_parser const & rop) {
        return std::tie(lop.scheme, lop.userinfo, lop.host, lop.port, lop.path, lop.query, lop.fragment)
            == std::tie(rop.scheme, rop.userinfo, rop.host, rop.port, rop.path, rop.query, rop.fragment);
    }
    
    WEBMOCK_INLINE std::ostream & operator <<(std::ostream & lop, uri_parser const & rop) {
        return lop
            << "scheme  : " << rop.scheme   << "\n"
            << "userinfo: " << rop.userinfo << "\n"
            << "host    : " << rop.host     << "\n"
            << "port    : " << rop.port     << "\n"
            << "path    : " << rop.path     << "\n"
            << "query   : " << rop.query    << "\n"
            << "fragment: " << rop.fragment << "\n"
        ;
    }
}}
