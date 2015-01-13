#include <webmock/config.hpp>
#include <webmock/core/request.hpp>
#include <tuple>

namespace webmock { namespace core {
    WEBMOCK_INLINE bool operator ==(request const & lop, request const & rop) {
        return std::tie(lop.method, lop.url, lop.headers, lop.body)
            == std::tie(rop.method, rop.url, rop.headers, rop.body);
    }
    
    WEBMOCK_INLINE bool operator !=(request const & lop, request const & rop) {
        return !(lop == rop);
    }
    
    WEBMOCK_INLINE std::ostream & operator <<(std::ostream & lop, request const & rop) {
        lop << rop.method << " " << rop.url << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}}
