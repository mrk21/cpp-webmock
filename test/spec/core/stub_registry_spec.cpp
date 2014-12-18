#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/stub_registry.hpp>
#include <memory>

namespace webmock { namespace core {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::stub_registry", []{
        std::string const url = "http://www.example.com";
        response const res1{"200","test1"};
        response const res2{"200","test2"};
        
        stub const stub1{
            {[&url](auto && req){ return req.url == url; }},
            {{1, [&res1](auto &&){ return res1; }}}
        };
        stub const stub2{
            {[&url](auto && req){ return req.url == url; }},
            {{1, [&res2](auto &&){ return res2; }}}
        };
        
        std::shared_ptr<stub_registry> registry;
        
        before_each([&]{
            registry = std::make_shared<stub_registry>();
            registry->add(stub1);
            registry->add(stub2);
        });
        
        describe("#access(request)", [&]{
            it("should be a response of first matching stub to the request", [&]{
                auto && res = registry->access({"GET", url});
                AssertThat(res == boost::none, Equals(false));
                AssertThat(*res, Equals(res1));
            });
            
            describe("when matching stub to the request not found", [&]{
                it("should be a boost::none", [&]{
                    AssertThat(
                        registry->access({"GET", url+"/hoge"}) == boost::none,
                        Equals(true));
                });
            });
        });

        describe("#count_requests(conditions)", [&]{
            it("should be a number of requests matched to the conditions in this history", [&]{
                registry->access({"GET", url});
                registry->access({"GET", url+"/hoge"});
                registry->access({"POST", url});
                registry->access({"GET", url});
                
                AssertThat(registry->count_requests({
                    [&](auto && req){ return req.method == "GET"; },
                    [&](auto && req){ return req.url == url; },
                }), Equals(2u));
            });
        });
    });
});
}}
