#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_SUPPORT_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_SUPPORT_HPP

#include <boost/network/protocol/http/tags.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace network { namespace http {
    template <class Tag, class Enable = void>
    struct is_webmock_adapter : mpl::false_ {};
    
    template <class Tag>
    struct is_webmock_adapter<Tag, typename enable_if<typename Tag::is_webmock_adapter>::type> : mpl::true_ {};
}}}

#endif
