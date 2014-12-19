#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/adapter/cpp_netlib.hpp>
#include <webmock/api.hpp>

namespace webmock { namespace adapter { namespace cpp_netlib {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::adapter::cpp_netlib", []{
        it("should swap the implementation", [&]{
            using namespace api::directive;
            namespace network = boost::network;
            namespace http = network::http;
            
            core::response webmock_response{
                "200",
                "test",
                {{"Content-Type","text/plane"}}
            };
            api::stub{"GET","http://www.boost.org"}
                .conditions(with_header("Connection","close"))
                .returns(response(webmock_response))
            ;
            client::request request("http://www.boost.org");
            request << network::header("Connection","close");
            client c;
            client::response response = c.get(request);
            
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
