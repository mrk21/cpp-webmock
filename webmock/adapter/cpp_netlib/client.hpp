#ifndef WEBMOCK_ADAPTER_CPP_NETLIB_CLIENT_HPP
#define WEBMOCK_ADAPTER_CPP_NETLIB_CLIENT_HPP

#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/network/protocol/http/client/pimpl.hpp>

#include <webmock/adapter/cpp_netlib/support.hpp>
#include <webmock/core/request.hpp>
#include <webmock/core/response.hpp>
#include <webmock/api/application.hpp>

namespace boost { namespace network { namespace http { namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct webmock_adapter_client {
        using response_type = basic_response<Tag>;
        using resolver_type = typename resolver<Tag>::type;
        using string_type = typename string<Tag>::type;
        using body_callback_function_type = function<
            void (iterator_range<char const *> const &, system::error_code const &)>;
        using body_generator_function_type = function<bool(string_type &)>;
        
    private:
        bool cache_resolved;
        bool follow_redirect;
        bool always_verify_peer;
        int timeout;
        boost::shared_ptr<boost::asio::io_service> service;
        optional<string_type> certificate_filename;
        optional<string_type> verify_path;
        optional<string_type> certificate_file;
        optional<string_type> private_key_file;
        
    public:
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
        ) :
            cache_resolved(cache_resolved),
            follow_redirect(follow_redirect),
            always_verify_peer(always_verify_peer),
            timeout(timeout),
            service(service),
            certificate_filename(certificate_filename),
            verify_path(verify_path),
            certificate_file(certificate_file),
            private_key_file(private_key_file)
        {}
        
        void wait_complete() {}
        
        response_type const request_skeleton(
            basic_request<Tag> const & request,
            string_type const & method,
            bool get_body,
            body_callback_function_type callback,
            body_generator_function_type generator
        ) {
            auto && app = webmock::api::app();
            
            webmock::core::request webmock_request;
            webmock_request.method = method;
            webmock_request.url.set_encoded(request.uri().string());
            for (auto && header: http::headers(request)) {
                webmock_request.headers.insert(header);
            }
            webmock_request.body = http::body(request);
            
            if (auto && webmock_response = app.registry.access(webmock_request)) {
                basic_response<Tag> response;
                response
                    << http::status(webmock_response->status)
                    << network::body(webmock_response->body);
                
                for (auto && header: webmock_response->headers) {
                    response << network::header(header.first, header.second);
                }
                return response;
            }
            
            if (app.config.stub_not_found.is_connecting_to_net) {
                return this->connect_net(
                    request,
                    method,
                    get_body,
                    callback,
                    generator
                );
            }
            
            if (app.config.stub_not_found.callback) {
                app.config.stub_not_found.callback(webmock_request);
            }
            
            throw (std::ostringstream()
                << "A stub satisfying the request not found!\n"
                << webmock_request
            ).str();
        }
        
    private:
        response_type const connect_net(
            basic_request<Tag> const & request,
            string_type const & method,
            bool get_body,
            body_callback_function_type callback,
            body_generator_function_type generator
        ) {
            using original_tag = typename webmock::adapter::cpp_netlib::tags::rmap<Tag>::type;
            using original_string_type = typename string<original_tag>::type;
            using original_client_type = basic_client<
                original_tag,
                version_major,
                version_minor
            >;
            
            typename original_client_type::request original_request(request.uri());
            for (auto && header: http::headers(request)) {
                original_request << network::header(header.first, header.second);
            }
            std::string body = http::body(request);
            original_request << network::body(body);
            
            client_options<original_tag> original_option;
            original_option
                .cache_resolved(this->cache_resolved)
                .follow_redirects(this->follow_redirect)
                .io_service(this->service)
                .timeout(this->timeout)
            ;
            if (this->certificate_filename) {
                original_option.openssl_certificate(
                    lexical_cast<original_string_type>(*this->certificate_filename)
                );
            }
            if (this->verify_path) {
                original_option.openssl_verify_path(
                    lexical_cast<original_string_type>(*this->verify_path)
                );
            }
            if (this->certificate_file) {
                original_option.openssl_certificate_file(
                    lexical_cast<original_string_type>(*this->certificate_file)
                );
            }
            if (this->private_key_file) {
                original_option.openssl_private_key_file(
                    lexical_cast<original_string_type>(*this->private_key_file)
                );
            }
            
            original_client_type  original_client(original_option);
            typename original_client_type::response original_response;
            
            if (method == "HEAD") {
                original_response = original_client.head(
                    original_request
                );
            }
            else if (method == "GET") {
                original_response = original_client.get(
                    original_request,
                    callback
                );
            }
            else if (method == "POST") {
                original_response = original_client.post(
                    original_request,
                    body,
                    callback,
                    generator
                );
            }
            else if (method == "PUT") {
                original_response = original_client.put(
                    original_request,
                    body,
                    callback,
                    generator
                );
            }
            else if (method == "DELETE") {
                original_response = original_client.delete_(
                    original_request,
                    callback
                );
            }
            
            basic_response<Tag> response;
            response
                << http::status(static_cast<int>(http::status(original_response)))
                << network::body(static_cast<std::string>(http::body(original_response)));
            
            for (auto && header: http::headers(original_response)) {
                response << network::header(header.first, header.second);
            }
            return response;
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
