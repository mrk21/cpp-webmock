#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/directive/stub.hpp>

namespace webmock { namespace directive {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::directive::stub", []{
        it("should build", [&]{
            core::stub_registry registry;
            
            directive::stub{"http://www.hogebar.jp/", registry}
                .conditions(
                    with_header("Content-Type", "application/json"),
                    with_body(std::regex("^response.*$"))
                )
                .returns(
                    response({"200"}) * 2,
                    error<std::exception>().times(2)
                )
                << response({"404"});
            
            auto && s = directive::stub{"POST", "http://www.hogebar.com/", registry}; s
                .conditions(with_header("Content-Type", "application/json"))
                .conditions(with_body(std::regex("^response.*$")))
                .returns(response({"200"}) * 2)
                .returns(error<std::exception>().times(2))
                << response({"404"});
            
            core::request const req{
                "POST",
                "http://www.hogebar.com/",
                {{"Content-Type", "application/json"}},
                "response333"
            };
            AssertThat(registry.access(req)->status, Equals("200"));
            AssertThat(registry.access(req)->status, Equals("200"));
            AssertThrows(std::exception, registry.access(req));
            AssertThrows(std::exception, registry.access(req));
            AssertThat(registry.access(req)->status, Equals("404"));
            AssertThat(registry.access(req)->status, Equals("404"));
            
            AssertThat(s.count(), Equals(6));
        });
    });
});
}}
