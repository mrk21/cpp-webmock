#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_TAGS_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_TAGS_HPP

#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network { namespace http { namespace tags {
    struct webmock_adapter {
        using is_webmock_adapter = mpl::true_::type;
    };
    
    using webmock_adapter_http_default_8bit_tcp_resolve_tags   = mpl::vector<http, client, simple   , webmock_adapter, tcp, default_string>;
    using webmock_adapter_http_default_8bit_udp_resolve_tags   = mpl::vector<http, client, simple   , webmock_adapter, udp, default_string>;
    using webmock_adapter_http_keepalive_8bit_tcp_resolve_tags = mpl::vector<http, client, keepalive, webmock_adapter, tcp, default_string>;
    using webmock_adapter_http_keepalive_8bit_udp_resolve_tags = mpl::vector<http, client, keepalive, webmock_adapter, udp, default_string>;
    using webmock_adapter_http_async_8bit_udp_resolve_tags     = mpl::vector<http, client, simple   , webmock_adapter, udp, default_string>;
    using webmock_adapter_http_async_8bit_tcp_resolve_tags     = mpl::vector<http, client, simple   , webmock_adapter, tcp, default_string>;
    
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_default_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_default_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_keepalive_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_keepalive_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_async_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(webmock_adapter_http_async_8bit_tcp_resolve);
}}}}

namespace webmock { namespace adapter { namespace cpp_netlib { namespace tags {
    namespace original = boost::network::http::tags;
    
    using http_default_8bit_tcp_resolve   = original::webmock_adapter_http_default_8bit_tcp_resolve;
    using http_default_8bit_udp_resolve   = original::webmock_adapter_http_default_8bit_udp_resolve;
    using http_keepalive_8bit_tcp_resolve = original::webmock_adapter_http_keepalive_8bit_tcp_resolve;
    using http_keepalive_8bit_udp_resolve = original::webmock_adapter_http_keepalive_8bit_udp_resolve;
    using http_async_8bit_udp_resolve     = original::webmock_adapter_http_async_8bit_udp_resolve;
    using http_async_8bit_tcp_resolve     = original::webmock_adapter_http_async_8bit_tcp_resolve;
    
    template <typename Tag> struct map {};
    template <> struct map<original::http_default_8bit_tcp_resolve>   { using type = http_default_8bit_tcp_resolve; };
    template <> struct map<original::http_default_8bit_udp_resolve>   { using type = http_default_8bit_udp_resolve; };
    template <> struct map<original::http_keepalive_8bit_tcp_resolve> { using type = http_keepalive_8bit_tcp_resolve; };
    template <> struct map<original::http_keepalive_8bit_udp_resolve> { using type = http_keepalive_8bit_udp_resolve; };
    template <> struct map<original::http_async_8bit_udp_resolve>     { using type = http_async_8bit_udp_resolve; };
    template <> struct map<original::http_async_8bit_tcp_resolve>     { using type = http_async_8bit_tcp_resolve; };
    
    template <typename Tag> struct rmap {};
    template <> struct rmap<http_default_8bit_tcp_resolve>   { using type = original::http_default_8bit_tcp_resolve; };
    template <> struct rmap<http_default_8bit_udp_resolve>   { using type = original::http_default_8bit_udp_resolve; };
    template <> struct rmap<http_keepalive_8bit_tcp_resolve> { using type = original::http_keepalive_8bit_tcp_resolve; };
    template <> struct rmap<http_keepalive_8bit_udp_resolve> { using type = original::http_keepalive_8bit_udp_resolve; };
    template <> struct rmap<http_async_8bit_udp_resolve>     { using type = original::http_async_8bit_udp_resolve; };
    template <> struct rmap<http_async_8bit_tcp_resolve>     { using type = original::http_async_8bit_tcp_resolve; };
}}}}

#endif
