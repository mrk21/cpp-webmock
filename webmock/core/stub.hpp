#ifndef WEBMOCK_CORE_STUB_HPP
#define WEBMOCK_CORE_STUB_HPP

#include <webmock/core/request.hpp>
#include <webmock/core/request.hpp>
#include <webmock/core/condition_list.hpp>
#include <webmock/core/response_sequence.hpp>
#include <queue>
#include <initializer_list>

namespace webmock { namespace core {
    class stub {
        condition_list conditions;
        std::queue<response_sequence> sequences;
        
    public:
        stub(condition_list const & conditions, std::initializer_list<response_sequence> sequences = {})
            : conditions(conditions), sequences(sequences) {}
        
        bool match(core::request const & request) const {
            return this->conditions.match(request);
        }
        
        response get_response(core::request const & request) {
            if (this->sequences.size() == 0) return {};
            if (this->sequences.size() > 1 && this->sequences.front().is_end()) {
                this->sequences.pop();
            }
            return this->sequences.front().get_response(request);
        }
        
        stub & add_condition(condition_list::condition_type condition) {
            this->conditions.add(condition);
            return *this;
        }
        
        stub & add_sequence(response_sequence const & sequence) {
            this->sequences.push(sequence);
            return *this;
        }
    };
}}

#endif
