#ifndef WEBMOCK_CONDITION_LIST_HPP
#define WEBMOCK_CONDITION_LIST_HPP

#include <webmock/request.hpp>
#include <vector>
#include <functional>

namespace webmock {
    class condition_list {
        using condition_type = std::function<bool(request const &)>;
        std::vector<condition_type> conditions;
        
    public:
        void add(condition_type condition);
        bool match(webmock::request const & request) const;
    };
}

#endif
