#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/condition_list.hpp>

namespace webmock {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::condition_list", []{
        describe("#match(request)", [&]{
            describe("when this was empty", [&]{
                it("should be true", [&]{
                    condition_list list;
                    AssertThat(list.match({}), Equals(true));
                });
            });
            
            describe("when the request matched", [&]{
                it("should be true", [&]{
                    condition_list list;
                    list.add([](auto &&){ return true; });
                    list.add([](auto &&){ return true; });
                    AssertThat(list.match({}), Equals(true));
                });
            });
            
            describe("when the request not matched", [&]{
                it("should be false", [&]{
                    condition_list list;
                    list.add([](auto &&){ return true; });
                    list.add([](auto &&){ return false; });
                    AssertThat(list.match({}), Equals(false));
                });
            });
        });
    });
});
}
