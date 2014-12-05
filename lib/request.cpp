#include <webmock/request.hpp>
#include <tuple>

namespace webmock {
    bool operator ==(request const & lop, request const & rop) {
        return std::tie(lop.method, lop.url, lop.headers, lop.body)
            == std::tie(rop.method, rop.url, rop.headers, rop.body);
    }
    
    bool operator !=(request const & lop, request const & rop) {
        return !(lop == rop);
    }
    
    std::ostream & operator <<(std::ostream & lop, request const & rop) {
        lop << rop.method << " " << rop.url << "\n";
        for (auto && h: rop.headers) {
            lop << h.first << ": " << h.second << "\n";
        }
        lop << rop.body << "\n";
        return lop;
    }
}
