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
        
        describe("with(lambda)", [&]{
            it("should be true", [&]{
                AssertThat(with([](auto &&){ return true; })(req), Equals(true));
            });
        });
        
        describe("with_method(value)", [&]{
            it("should be true", [&]{
                AssertThat(with_method("PUT")(req), Equals(true));
                AssertThat(with_method(std::regex("PUT|GET"))(req), Equals(true));
            });
        });
        
        describe("with_url(value)", [&]{
            it("should be true", [&]{
                AssertThat(with_url("http://www.foobar.com/path/to/resource")(req), Equals(true));
                AssertThat(with_url(std::regex("http://www.foobar.com/.*"))(req), Equals(true));
            });
        });
        
        describe("with_body(value)", [&]{
            it("should be true", [&]{
                AssertThat(with_body("{\"hoge\": 1}")(req), Equals(true));
                AssertThat(with_body(std::regex("\\{\"hoge\": \\d\\}"))(req), Equals(true));
            });
        });
        
        describe("with_header(key, value)", [&]{
            it("should be true", [&]{
                AssertThat(with_header("Content-Type","application/json")(req), Equals(true));
                AssertThat(with_header("Content-Type", std::regex("application/(json|xml)"))(req), Equals(true));
            });
        });
    });
});
}}
