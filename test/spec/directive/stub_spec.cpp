#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/directive/stub.hpp>

namespace webmock { namespace directive {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::directive::stub", []{
        describe("stub(method, url)", [&]{
            it("should build", [&]{
                auto && s = stub("POST", "http://www.hogebar.com/")
                    << with_header("Content-Type", "application/json")
                    << with_body(std::regex("^response.*$"))
                    << response({"200"}) * 2
                    << error<std::exception>().times(2)
                    << response({"404"});
                
                AssertThat(s.match({
                    "POST",
                    "http://www.hogebar.com/",
                    {{"Content-Type","application/json"}},
                    "response: hoge"
                }), Equals(true));
                
                AssertThat(s.match({
                    "POST",
                    "http://www.hogebar.com/",
                    {{"Content-Type","application/json"}},
                }), Equals(false));
                
                AssertThat(s.get_response({}).status, Equals("200"));
                AssertThat(s.get_response({}).status, Equals("200"));
                AssertThrows(std::exception, s.get_response({}));
                AssertThrows(std::exception, s.get_response({}));
                AssertThat(s.get_response({}).status, Equals("404"));
                AssertThat(s.get_response({}).status, Equals("404"));
            });
        });
    });
});
}}
