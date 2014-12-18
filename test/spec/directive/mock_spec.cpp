#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/directive/mock.hpp>

namespace webmock { namespace directive {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::directive::mock", []{
        describe("#count()", [&]{
            it("should be a number of an access matched by this conditions, and the number should be cached by first result", [&]{
                core::stub_registry registry;
                registry.access({"GET", "http://www.hogefuga.com/1"});
                registry.access({"PUT", "http://www.hogefuga.com/2"});
                registry.access({"GET", "http://www.hogefuga.jp/3"});
                registry.access({"GET", "http://www.hogefuga.com/4"});
                
                mock m{"GET", std::regex("http://www.hogefuga.com/.*"), registry};
                
                AssertThat(m.count(), Equals(2));
                registry.access({"GET", "http://www.hogefuga.com/5"});
                AssertThat(static_cast<int>(m), Equals(2));
            });
        });
        
        describe("#operator <<(condition_type rop)", [&]{
            it("should add the condition_type", [&]{
                core::stub_registry registry;
                registry.access({"POST", "http://www.hogebar.com/"});
                registry.access({"POST", "http://www.hogebar.com/", {
                    {"Content-Type", "application/json"}
                }, "response3"});
                
                auto && m = mock{"POST", "http://www.hogebar.com/", registry}; m
                    << with_header("Content-Type", "application/json")
                    << with_body(std::regex("^response.*$"));
                
                AssertThat(static_cast<int>(
                    mock{"POST", "http://www.hogebar.com/", registry}
                        << with_header("Content-Type", "application/json")
                        << with_body(std::regex("^response.*$"))
                ), Equals(1));
            });
        });
    });
});
}}
