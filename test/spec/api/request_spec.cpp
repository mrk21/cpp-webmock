#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/api/request.hpp>

namespace webmock { namespace api {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::api::request", []{
        describe("#count()", [&]{
            it("should be a number of an access matched by this conditions", [&]{
                application app;
                app.registry.access({"GET", "http://www.hogefuga.com/1"});
                app.registry.access({"PUT", "http://www.hogefuga.com/2"});
                app.registry.access({"GET", "http://www.hogefuga.jp/3"});
                app.registry.access({"GET", "http://www.hogefuga.com/4"});
                
                a_request req{"GET", std::regex("http://www.hogefuga.com/.*"), app};
                
                AssertThat(req.count(), Equals(2));
                app.registry.access({"GET", "http://www.hogefuga.com/5"});
                AssertThat(static_cast<int>(req), Equals(3));
            });
        });
        
        describe("#operator <<(condition_type rop)", [&]{
            it("should add the condition_type", [&]{
                application app;
                app.registry.access({"POST", "http://www.hogebar.com/"});
                app.registry.access({"POST", "http://www.hogebar.com/", {
                    {"Content-Type", "application/json"}
                }, "response3"});
                
                auto && req = a_request{"POST", "http://www.hogebar.com/", app}; req
                    .conditions(with_header("Content-Type", "application/json"))
                    << with_body(std::regex("^response.*$"));
                
                AssertThat(a_request("POST", "http://www.hogebar.com/", app)
                    .conditions(with_header("Content-Type", "application/json"))
                    .conditions(with_body(std::regex("^response.*$")))
                    .count()
                , Equals(1));
            });
        });
    });
});
}}
