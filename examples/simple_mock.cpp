#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include <boost/network/mock.hpp>

int main() {
    namespace network = boost::network;
    namespace http = network::http;
#if 1
    using tag = http::tags::http_mock_8bit_tcp_resolve;
    
    network::mock::simple_mock<tag>("GET", "http://www.boost.org", [](auto && request){
        std::cout << "#request" << std::endl;
        for (auto && h: network::headers(request)) {
            std::cout << h.first << ": " << h.second << std::endl;
        }
        
        http::basic_response<tag> result;
        result
            << http::status(200)
            << network::header("Content-Type","text/plane")
            << network::header("Content-Length","4")
            << network::body("test");
        return result;
    });
#else
    using tag = http::tags::http_async_8bit_tcp_resolve;
#endif
    using client_type = http::basic_client<tag,1,1>;
    
    client_type::request request("http://www.boost.org");
    request << network::header("Connection","close");
    client_type client;
    client_type::response response = client.get(request);
    
    std::cout << "#response" << std::endl;
    std::cout << http::status(response) << std::endl;
    std::cout << http::body(response) << std::endl;
    
    for (auto && h: http::headers(response)) {
        std::cout << h.first << ": " << h.second << std::endl;
    }
    
    return 0;
}
