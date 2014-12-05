#include <webmock/response.hpp>
#include <tuple>

namespace webmock {
    bool operator ==(response const & lop, response const & rop) {
        return std::tie(lop.status, lop.body, lop.headers)
            == std::tie(rop.status, rop.body, rop.headers);
    }
    
    bool operator !=(response const & lop, response const & rop) {
        return !(lop == rop);
    }
    
    std::ostream & operator <<(std::ostream & lop, response const & rop) {
        lop << rop.status << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}
