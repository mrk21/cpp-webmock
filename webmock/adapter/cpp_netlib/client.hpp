#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_CLIENT_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_CLIENT_HPP

#include <boost/lexical_cast.hpp>
#include <boost/network/protocol/http/client/pimpl.hpp>

#include <webmock/adapter/cpp_netlib/support.hpp>
#include <webmock/core/request.hpp>
#include <webmock/core/response.hpp>
#include <webmock/api/detail/registry.hpp>

namespace boost { namespace network { namespace http { namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct webmock_adapter_client {
        using response_type = basic_response<Tag>;
        using resolver_type = typename resolver<Tag>::type;
        using string_type = typename string<Tag>::type;
        using body_callback_function_type = function<
            void (iterator_range<char const *> const &, system::error_code const &)>;
        using body_generator_function_type = function<bool(string_type &)>;
        
        webmock_adapter_client(
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
            webmock::core::request webmock_request;
            webmock_request.method = method;
            webmock_request.url = request.uri().string();
            for (auto && header: http::headers(request)) {
                webmock_request.headers.insert(header);
            }
            webmock_request.body = http::body(request);
            
            if (auto && webmock_response = webmock::api::detail::registry().access(webmock_request)) {
                basic_response<Tag> response;
                response
                    << http::status(webmock_response->status)
                    << network::body(webmock_response->body);
                
                for (auto && header: webmock_response->headers) {
                    response << network::header(header.first, header.second);
                }
                return response;
            }
            return {};
        }
    };
    
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct client_base<Tag, version_major, version_minor,
        typename enable_if<is_webmock_adapter<Tag>>::type>
    {
        using type = webmock_adapter_client<Tag, version_major, version_minor>;
    };
}}}}

#endif
