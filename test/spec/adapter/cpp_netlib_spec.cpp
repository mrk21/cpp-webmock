#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/adapter/cpp_netlib.hpp>
#include <webmock/directive/stub.hpp>

namespace webmock { namespace adapter { namespace cpp_netlib {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::adapter::cpp_netlib", []{
        it("should swap the implementation", [&]{
            namespace network = boost::network;
            namespace http = network::http;
            using tag = http::tags::http_mock_8bit_tcp_resolve;
            using client_type = http::basic_client<tag,1,1>;
            
            webmock::response webmock_response{
                "200",
                "test",
                {{"Content-Type","text/plane"}}
            };
            webmock::directive::stub("http://www.boost.org")
                << webmock::directive::response(webmock_response);
            
            client_type::request request("http://www.boost.org");
            request << network::header("Connection","close");
            client_type client;
            client_type::response response = client.get(request);
            
            AssertThat(
                static_cast<int>(http::status(response)),
                Equals(boost::lexical_cast<int>(webmock_response.status))
            );
            AssertThat(
                static_cast<std::string>(http::body(response)),
                Equals(webmock_response.body)
            );
            for (auto && header: http::headers(response)) {
                AssertThat(webmock_response.headers.count(header.first), Equals(1));
                AssertThat(webmock_response.headers.find(header.first)->second, Equals(header.second));
            }
        });
    });
});
}}}
