#include <bandit_with_gmock/bandit_with_gmock.hpp>

namespace webmock {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::condition_list", []{
        it("should be true", [&]{
            AssertThat(true, Equals(true));
        });
    });
});
}
