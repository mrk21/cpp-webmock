#ifndef BOOST_NETWORK_MOCK_CLIENT_HPP
#define BOOST_NETWORK_MOCK_CLIENT_HPP

#include <boost/network/protocol/http/client/pimpl.hpp>
#include <boost/network/mock/support.hpp>
#include <boost/network/mock/request_manager.hpp>

namespace boost { namespace network { namespace http { namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct mock_client {
        using response_type = basic_response<Tag>;
        using resolver_type = typename resolver<Tag>::type;
        using string_type = typename string<Tag>::type;
        using body_callback_function_type = function<
            void (iterator_range<char const *> const &, system::error_code const &)>;
        using body_generator_function_type = function<bool(string_type &)>;
        
        mock_client(
            bool cache_resolved,
            bool follow_redirect,
            bool always_verify_peer,
            int timeout,
            boost::shared_ptr<boost::asio::io_service> service,
            optional<string_type> const & certificate_filename,
            optional<string_type> const & verify_path,
            optional<string_type> const & certificate_file,
            optional<string_type> const & private_key_file
        ) {}
        
        void wait_complete() {}
        
        response_type const request_skeleton(
            basic_request<Tag> const & request,
            string_type const & method,
            bool get_body,
            body_callback_function_type callback,
            body_generator_function_type generator
        ) {
            return mock::get_request_manager<Tag>()[
                {method, request.uri().string()}
            ]->request(method, request);
        }
    };
    
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct client_base<Tag, version_major, version_minor,
        typename enable_if<is_mock<Tag>>::type>
    {
        using type = mock_client<Tag, version_major, version_minor>;
    };
}}}}

#endif
