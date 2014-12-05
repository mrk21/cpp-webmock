#include <webmock/stub.hpp>

namespace webmock {
    stub::stub(condition_list const & conditions, std::initializer_list<response_sequence> sequences)
        : conditions(conditions), sequences(sequences) {}
    
    bool stub::match(webmock::request const & request) const {
        return this->conditions.match(request);
    }
    
    response stub::get_response(webmock::request const & request) {
        if (this->sequences.size() == 0) return response{};
        if (this->sequences.size() > 1 && this->sequences.front().is_end()) {
            this->sequences.pop();
        }
        return this->sequences.front().get_response(request);
    }
}
