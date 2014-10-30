#include <iostream>
#include <boost/network/protocol/http/client.hpp>

namespace boost { namespace network {
    template <class Tag, class Enable = void>
    struct is_dummy : mpl::false_ {};
    
    template <class Tag>
    struct is_dummy<Tag, typename enable_if<typename Tag::is_dummy>::type> : mpl::true_ {};
    
namespace tags {
    struct dummy {
        using is_dummy = mpl::true_::type;
    };
}
namespace http { namespace tags {
    using http_dummy_8bit_tcp_resolve_tags =
        mpl::vector<http, client, simple, dummy, tcp, default_string>;
    
    BOOST_NETWORK_DEFINE_TAG(http_dummy_8bit_tcp_resolve);
}}
namespace http { namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct dummy_client {
        using resolver_type = typename resolver<Tag>::type;
        using string_type = typename string<Tag>::type;
        using body_callback_function_type = function<
            void (boost::iterator_range<char const *> const &, system::error_code const &)>;
        using body_generator_function_type = function<bool(string_type &)>;
        
        dummy_client(
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
        
        basic_response<Tag> const request_skeleton(
            basic_request<Tag> const & request,
            string_type const & method,
            bool get_body,
            body_callback_function_type callback,
            body_generator_function_type generator
        ) {
            std::cout << "#request" << std::endl;
            std::cout << method << std::endl;
            std::cout << request.uri().string() << std::endl;
            for (auto & h: network::headers(request)) {
                std::cout << h.first << ": " << h.second << std::endl;
            }
            
            basic_response<Tag> result;
            result
                << http::status(200)
                << network::header("Content-Type","text/plane")
                << network::header("Content-Length","4")
                << network::body("test");
            return result;
        }
    };
    
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct client_base<Tag, version_major, version_minor,
        typename enable_if<is_dummy<Tag>>::type>
    {
        using type = dummy_client<Tag, version_major, version_minor>;
    };
}}
}}

int main() {
    namespace network = boost::network;
    namespace http = network::http;
    using client_type = http::basic_client<http::tags::http_dummy_8bit_tcp_resolve, 1,1>;
    
    client_type::request request("http://www.hoge.jp");
    request << network::header("Connection","close");
    client_type client;
    client_type::response response = client.get(request);
    
    std::cout << "#response" << std::endl;
    std::cout << http::status(response) << std::endl;
    std::cout << http::body(response) << std::endl;
    
    for (auto & h: network::headers(response)) {
        std::cout << h.first << ": " << h.second << std::endl;
    }
    
    return 0;
}
