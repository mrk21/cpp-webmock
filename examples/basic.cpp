#include <iostream>
#include <webmock/webmock.hpp>
#include <webmock/adapter/cpp_netlib.hpp>

template <typename Tag>
void access() {
    namespace directive = webmock::directive;
    namespace network = boost::network;
    namespace http = network::http;
    using client_type = http::basic_client<Tag,1,1>;
    
    auto && stub = directive::stub{"GET","http://www.boost.org/"}
        << directive::response({"200","test"});
    
    typename client_type::request request("http://www.boost.org/");
    request << network::header("Connection","close");
    client_type client;
    typename client_type::response response = client.get(request);
    
    std::cout << "status: " << http::status(response) << std::endl;
    std::cout << "body: " << http::body(response) << std::endl;
    std::cout << "access count: " << stub << std::endl;
}

int main() {
    namespace http = boost::network::http;
    
    std::cout << "## Normal" << std::endl;
    access<http::tags::http_async_8bit_tcp_resolve>();
    
    std::cout << "## Mock" << std::endl;
    access<http::tags::http_mock_8bit_tcp_resolve>();
    
    return 0;
}
