#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/adapter/cpp_netlib.hpp>
#include <webmock/api.hpp>
#include <sstream>

namespace webmock { namespace adapter { namespace cpp_netlib {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::adapter::cpp_netlib", []{
        using namespace api::directive;
        namespace network = boost::network;
        namespace http = network::http;
        
        webmock::response const webmock_response{
            "200",
            "test",
            {{"Content-Type","text/plane"}}
        };
        
        after_each([&]{
            api::reset();
        });
        
        it("should swap the implementation", [&]{
            a_stub{"GET","http://www.boost.org"}
                .conditions(with_header("Connection","close"))
                .returns(a_response(webmock_response))
            ;
            cpp_netlib::client::request request("http://www.boost.org");
            request << network::header("Connection","close");
            cpp_netlib::client client;
            cpp_netlib::client::response response = client.get(request);
            
            AssertThat(
                static_cast<int>(http::status(response)),
                Equals(static_cast<int>(webmock_response.status))
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
        
        describe("when the stub satisfying the request not found", [&]{
            it("should throw an exception", [&]{
                webmock::request const webmock_request{
                    "GET", "http://www.hogefuga.jp/path/to/bar.json", {
                        {"Content-Type", "application/json"}
                    },
                    "{\"hoge\": 1}"
                };
                
                cpp_netlib::client::request request(webmock_request.url);
                for (auto && header: webmock_request.headers) {
                    request << network::header(header.first, header.second);
                }
                request << network::body(webmock_request.body);
                
                cpp_netlib::client client;
                AssertThrows(std::string, client.get(request));
                AssertThat(LastException<std::string>(), Equals((std::ostringstream()
                    << "A stub satisfying the request not found!\n"
                    << webmock_request
                ).str()));
            });
        });
    });
});
}}}
