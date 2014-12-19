#include <iostream>
#include <webmock/api.hpp>
#include <webmock/adapter/cpp_netlib.hpp>

template <bool Enabled, typename Tag>
void access() {
    namespace api = webmock::api;
    using namespace api::directive;
    
    namespace network = boost::network;
    namespace http = network::http;
    namespace webmock_adapter = webmock::adapter::cpp_netlib;
    using client_type = typename webmock_adapter::select_by_param<Enabled, Tag,1,1>::type;
    
    a_stub{"GET","http://www.boost.org/"}.returns(a_response({"200"}).body("test"));
    
    typename client_type::request request("http://www.boost.org/");
    request << network::header("Connection","close");
    client_type client;
    typename client_type::response result = client.get(request);
    
    std::cout << "status: " << http::status(result) << std::endl;
    std::cout << "body: " << http::body(result) << std::endl;
    std::cout << "access count: " << a_request{}.conditions(with_method("GET")) << std::endl;
}

int main() {
    namespace http = boost::network::http;
    
    std::cout << "## Normal" << std::endl;
    access<false, http::tags::http_async_8bit_tcp_resolve>();
    
    std::cout << "## Mock" << std::endl;
    access<true, http::tags::http_async_8bit_tcp_resolve>();
    
    return 0;
}
