#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/ci_value_base.hpp>
#include <sstream>

namespace webmock { namespace core {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::ci_value_base", []{
        describe("operators", [&]{
            it("should pass", [&]{
                struct val: public ci_value_base<val> {
                    using ci_value_base<val>::ci_value_base;
                };
                AssertThat(val("get") == "GET", Equals(true));
                AssertThat(val("get") < "GET2", Equals(true));
                AssertThat(static_cast<std::string>(val("get")), Equals("get"));
            });
        });
    });
});
}}
