#include <webmock/request.hpp>

namespace webmock {
    bool operator ==(request const & lop, request const & rop) {
        return lop.method == rop.method
            && lop.uri == rop.uri
            && lop.headers == rop.headers
            && lop.body == rop.body;
    }
    
    bool operator !=(request const & lop, request const & rop) {
        return !(lop == rop);
    }
}
