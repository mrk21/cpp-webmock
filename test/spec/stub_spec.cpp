#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/stub.hpp>

namespace webmock {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::stub", []{
        describe("#get_response(request)", [&]{
            it("should return a response specified by this sequences, and if came to an end of the last sequence, then keep returning that response", [&]{
                response res1{"200", "test1", {{"Content-Type","text/plane"}}};
                response res2{"200", "test2", {{"Content-Type","text/plane"}}};
                stub target{
                    {[](auto &&){ return true; }},
                    {
                        {2, [&res1](auto &&){ return res1; }},
                        {1, [&res2](auto &&){ return res2; }},
                    }
                };
                AssertThat(target.get_response(request{}), Equals(res1));
                AssertThat(target.get_response(request{}), Equals(res1));
                AssertThat(target.get_response(request{}), Equals(res2));
                AssertThat(target.get_response(request{}), Equals(res2));
                AssertThat(target.get_response(request{}), Equals(res2));
            });
            
            describe("when this sequenses were empty", [&]{
                it("should return an empty response", [&]{
                    stub target{
                        {[](auto &&){ return true; }},
                        {}
                    };
                    AssertThat(target.get_response(request{}), Equals(response{}));
                });
            });
        });
    });
});
}
