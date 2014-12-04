#include <webmock/response.hpp>

namespace webmock {
    bool operator ==(response const & lop, response const & rop) {
        return lop.status == rop.status
            && lop.body == rop.body
            && lop.headers == rop.headers;
    }
    
    bool operator !=(response const & lop, response const & rop) {
        return !(lop == rop);
    }
}

