#ifndef BOOST_NETWORK_MOCK_SUPPORT_HPP
#define BOOST_NETWORK_MOCK_SUPPORT_HPP

#include <boost/network/protocol/http/tags.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace network { namespace http {
    template <class Tag, class Enable = void>
    struct is_mock : mpl::false_ {};
    
    template <class Tag>
    struct is_mock<Tag, typename enable_if<typename Tag::is_mock>::type> : mpl::true_ {};
}}}

#endif
