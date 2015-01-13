#include <webmock/config.hpp>
#include <webmock/api/condition.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/assign.hpp>

namespace webmock { namespace api {
inline namespace directive {
    WEBMOCK_INLINE bool with_header::operator ()(core::request const & request) const {
        using namespace boost::assign;
        using namespace boost::adaptors;
        return boost::algorithm::all_of(
            request.headers.equal_range(this->name) | map_values, this->match
        );
    }
}
}}
