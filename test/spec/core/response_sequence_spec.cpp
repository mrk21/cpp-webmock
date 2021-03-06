#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/response_sequence.hpp>

namespace webmock { namespace core {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::response_sequence", []{
        describe("#is_end()", [&]{
            it("should return a true as many times as specified by this count", [&]{
                response target{"200","test"};
                response_sequence sequence(2, [&](auto &&){ return target; });
                
                AssertThat(sequence.is_end(), Equals(false));
                AssertThat(sequence.get_response({}), Equals(target));
                
                AssertThat(sequence.is_end(), Equals(false));
                AssertThat(sequence.get_response({}), Equals(target));
                
                AssertThat(sequence.is_end(), Equals(true));
                AssertThat(sequence.get_response({}), Equals(target));
            });
        });
    });
});
}}
