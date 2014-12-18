#ifndef WEBMOCK_CONDITION_LIST_HPP
#define WEBMOCK_CONDITION_LIST_HPP

#include <webmock/request.hpp>
#include <vector>
#include <functional>
#include <initializer_list>

namespace webmock {
    class condition_list {
    public:
        using condition_type = std::function<bool(request const &)>;
        
    private:
        std::vector<condition_type> conditions;
        
    public:
        condition_list() = default;
        
        condition_list(std::initializer_list<condition_type> conditions)
            : conditions(conditions) {}
        
        void add(condition_type condition) {
            this->conditions.push_back(condition);
        }
        
        bool match(webmock::request const & request) const {
            for (auto && condition: this->conditions) {
                if (!condition(request)) return false;
            }
            return true;
        }
    };
}

#endif
