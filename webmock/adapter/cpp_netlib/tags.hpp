#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_TAGS_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_TAGS_HPP

#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network { namespace http { namespace tags {
    struct mock {
        using is_mock = mpl::true_::type;
    };
    
    using http_mock_8bit_tcp_resolve_tags =
        mpl::vector<http, client, simple, mock, tcp, default_string>;
    
    BOOST_NETWORK_DEFINE_TAG(http_mock_8bit_tcp_resolve);
}}}}

#endif
