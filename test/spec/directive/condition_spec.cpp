#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/directive/condition.hpp>

namespace webmock { namespace directive {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::directive::condition", []{
        request const req{
            "PUT",
            "http://www.foobar.com/path/to/resource",
            {{"Content-Type","application/json"}},
            "{\"hoge\": 1}"
        };
        
        describe("method(value)", [&]{
            it("should be true", [&]{
                AssertThat(method("PUT")(req), Equals(true));
                AssertThat(method(std::regex("PUT|GET"))(req), Equals(true));
            });
        });
        
        describe("url(value)", [&]{
            it("should be true", [&]{
                AssertThat(url("http://www.foobar.com/path/to/resource")(req), Equals(true));
                AssertThat(url(std::regex("http://www.foobar.com/.*"))(req), Equals(true));
            });
        });
        
        describe("body(value)", [&]{
            it("should be true", [&]{
                AssertThat(body("{\"hoge\": 1}")(req), Equals(true));
                AssertThat(body(std::regex("\\{\"hoge\": \\d\\}"))(req), Equals(true));
            });
        });
        
        
        describe("header(key, value)", [&]{
            it("should be true", [&]{
                AssertThat(header("Content-Type","application/json")(req), Equals(true));
                AssertThat(header("Content-Type", std::regex("application/(json|xml)"))(req), Equals(true));
            });
        });
    });
});
}}
