#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/http.hpp>

namespace webmock { namespace core { namespace http {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::http", [&]{
        describe("::method", [&]{
            it("should convert std::string that is all uppercase", [&]{
                AssertThat(static_cast<std::string>(method("get")), Equals("GET"));
            });
        });
    });
});
}}}
