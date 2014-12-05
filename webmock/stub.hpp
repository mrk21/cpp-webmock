#ifndef WEBMOCK_STUB_HPP
#define WEBMOCK_STUB_HPP

#include <webmock/request.hpp>
#include <webmock/request.hpp>
#include <webmock/condition_list.hpp>
#include <webmock/response_sequence.hpp>
#include <queue>
#include <initializer_list>

namespace webmock {
    class stub {
        condition_list conditions;
        std::queue<response_sequence> sequences;
        
    public:
        stub(condition_list const & conditions, std::initializer_list<response_sequence> sequences);
        bool match(webmock::request const & request) const;
        response get_response(webmock::request const & request);
    };
}

#endif
