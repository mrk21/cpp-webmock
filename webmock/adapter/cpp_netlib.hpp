#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_HPP

#include <boost/network/protocol/http/client.hpp>
#include <webmock/adapter/cpp_netlib/tags.hpp>
#include <webmock/adapter/cpp_netlib/client.hpp>
#include <utility>

namespace webmock { namespace adapter { namespace cpp_netlib {
    namespace detail {
        template <typename Tag, unsigned version_major, unsigned version_minor>
        boost::network::http::basic_client<
            typename tags::map<Tag>::type,
            version_major,
            version_minor
        >
        client_map(
            boost::network::http::basic_client<
                Tag,
                version_major,
                version_minor
            >
        );
    }
    
    template <typename OriginalClient>
    using basic_client = decltype(detail::client_map(
        std::declval<OriginalClient>()
    ));
    
    using client = basic_client<boost::network::http::client>;
    
    
    template <bool Enabled, typename OriginalClient>
    struct select_by_type {
        using type = basic_client<OriginalClient>;
    };
    
    template <typename OriginalClient>
    struct select_by_type<false, OriginalClient> {
        using type = OriginalClient;
    };
    
    
    template <bool Enabled, typename Tag, unsigned version_major, unsigned version_minor>
    struct select_by_param {
        using type = boost::network::http::basic_client<
            typename tags::map<Tag>::type,
            version_major,
            version_minor
        >;
    };
    
    template <typename Tag, unsigned version_major, unsigned version_minor>
    struct select_by_param<false, Tag, version_major, version_minor> {
        using type = boost::network::http::basic_client<
            Tag,
            version_major,
            version_minor
        >;
    };
}}}

#endif
