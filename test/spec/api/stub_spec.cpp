#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/api/stub.hpp>

namespace webmock { namespace api {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::api::stub", []{
        it("should build", [&]{
            core::stub_registry registry;
            
            a_stub{"http://www.hogebar.jp/", registry}
                .conditions(
                    with_header("Content-Type", "application/json"),
                    with_body(std::regex("^response.*$"))
                )
                .returns(
                    a_response({"200"}) * 2,
                    an_error<std::exception>().times(2)
                )
                << a_response({"404"});
            
            auto && s = a_stub{"POST", "http://www.hogebar.com/", registry}; s
                .conditions(with_header("Content-Type", "application/json"))
                .conditions(with_body(std::regex("^response.*$")))
                .returns(a_response({"200"}).times(2))
                << an_error<std::exception>() * 2
                << a_response({"404"});
            
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
