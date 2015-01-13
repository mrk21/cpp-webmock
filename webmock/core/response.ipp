#include <webmock/config.hpp>
#include <webmock/core/response.hpp>
#include <tuple>

namespace webmock { namespace core {
    WEBMOCK_INLINE bool operator ==(response const & lop, response const & rop) {
        return std::tie(lop.status, lop.body, lop.headers)
            == std::tie(rop.status, rop.body, rop.headers);
    }
    
    WEBMOCK_INLINE bool operator !=(response const & lop, response const & rop) {
        return !(lop == rop);
    }
    
    WEBMOCK_INLINE std::ostream & operator <<(std::ostream & lop, response const & rop) {
        lop << rop.status << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}}
