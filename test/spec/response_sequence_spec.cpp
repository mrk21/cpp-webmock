#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/response_sequence.hpp>

namespace webmock {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::response_sequence", []{
        describe("#is_end()", [&]{
            it("should return a true as many times as specified by this count", [&]{
                response target{"200", "test", {
                    {"Content-Type","text/plane"},
                }};
                response_sequence sequence(2, [&](auto &&){ return target; });
                
                AssertThat(sequence.is_end(), Equals(false));
                AssertThat(sequence.get_response(request{}), Equals(target));
                
                AssertThat(sequence.is_end(), Equals(false));
                AssertThat(sequence.get_response(request{}), Equals(target));
                
                AssertThat(sequence.is_end(), Equals(true));
                AssertThat(sequence.get_response(request{}), Equals(target));
            });
        });
    });
});
}
