#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include <webmock/api.hpp>
#include <webmock/adapter/cpp_netlib.hpp>

int main() {
    namespace webmock_adapter = webmock::adapter::cpp_netlib;
    using namespace webmock::api::directive;
    constexpr bool enabled_webmock = true;
    
    namespace network = boost::network;
    namespace http = network::http;
    using client_type = typename webmock_adapter::select_by_type<enabled_webmock, http::client>::type;
    
    a_stub("http://www.boost.org/users/history/version_1_56_0.html").returns(a_response({"200"}).body("test"));
    
    client_type::request request("http://www.boost.org/users/history/version_1_56_0.html");
    request << network::header("Connection","Close");
    client_type client;
    client_type::response response = client.get(request);
    
    std::cout << "status: " << http::status(response) << std::endl; // status: 200
    std::cout << "body: " << http::body(response) << std::endl;     // body: test
    std::cout << "access count: "                                   // access count: 1
        << a_request(std::regex("http://www.boost.org/.*")).count()
        << std::endl;
    
    return 0;
}
