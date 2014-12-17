#include <webmock/stub.hpp>

namespace webmock {
    stub::stub(condition_list const & conditions, std::initializer_list<response_sequence> sequences)
        : conditions(conditions), sequences(sequences) {}
    
    bool stub::match(webmock::request const & request) const {
        return this->conditions.match(request);
    }
    
    response stub::get_response(webmock::request const & request) {
        if (this->sequences.size() == 0) return {};
        if (this->sequences.size() > 1 && this->sequences.front().is_end()) {
            this->sequences.pop();
        }
        return this->sequences.front().get_response(request);
    }
    
    stub & stub::add_condition(condition_list::condition_type condition) {
        this->conditions.add(condition);
        return *this;
    }
    
    stub & stub::add_sequence(response_sequence const & sequence) {
        this->sequences.push(sequence);
        return *this;
    }
}
