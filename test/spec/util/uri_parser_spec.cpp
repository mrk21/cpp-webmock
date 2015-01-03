#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/util/uri_parser.hpp>

namespace webmock { namespace util {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::util::uri_parser", []{
        it("should parse", [&]{
            AssertThat(
                uri_parser("http://www.boost.org/path/to/url?a=1&b=2#aa"),
                Equals(uri_parser({
                    {"scheme", "http"},
                    {"host", "www.boost.org"},
                    {"path", "/path/to/url"},
                    {"query", "a=1&b=2"},
                    {"fragment", "aa"},
                }))
            );
            AssertThat(
                uri_parser("http://user:pass@www.boost.org:80/path/to/url?a=1&b=2#aa"),
                Equals(uri_parser({
                    {"scheme", "http"},
                    {"userinfo", "user:pass"},
                    {"host", "www.boost.org"},
                    {"port", "80"},
                    {"path", "/path/to/url"},
                    {"query", "a=1&b=2"},
                    {"fragment", "aa"},
                }))
            );
            AssertThat(
                uri_parser("http://192.168.1.2/path/to/url?a=1&b=2#aa"),
                Equals(uri_parser({
                    {"scheme", "http"},
                    {"host", "192.168.1.2"},
                    {"path", "/path/to/url"},
                    {"query", "a=1&b=2"},
                    {"fragment", "aa"},
                }))
            );
            AssertThat(
                uri_parser("http://192.168.1.2:80/path/to/url?a=1&b=2#aa"),
                Equals(uri_parser({
                    {"scheme", "http"},
                    {"host", "192.168.1.2"},
                    {"port", "80"},
                    {"path", "/path/to/url"},
                    {"query", "a=1&b=2"},
                    {"fragment", "aa"},
                }))
            );
        });
    });
});
}}
