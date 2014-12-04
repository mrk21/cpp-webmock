#include <webmock/response_sequence.hpp>

namespace webmock {
    response_sequence::response_sequence(std::size_t count, response_generator_type response_generator)
        : count(count), response_generator(response_generator) {}
    
    bool response_sequence::is_end() const {
        return this->count == 0;
    }
    
    response response_sequence::get_response(webmock::request const & request) {
        if (this->count > 0) --this->count;
        return this->response_generator(request);
    }
}
