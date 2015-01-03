#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/http.hpp>

namespace webmock { namespace core { namespace http {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::http", [&]{
        describe("::method", [&]{
            it("should convert std::string that is all uppercase", [&]{
                AssertThat(static_cast<std::string>(method("get")), Equals("GET"));
            });
        });
        
        describe("::header_name", [&]{
            it("should convert std::string that is camelcase", [&]{
                AssertThat(static_cast<std::string>(header_name("TRANSFER-ENCODING")), Equals("Transfer-Encoding"));
                AssertThat(static_cast<std::string>(header_name("transfer-encoding")), Equals("Transfer-Encoding"));
            });
        });
        
        describe("::status", [&]{
            it("should be able to construct from an integer", [&]{
                AssertThat(status(200), Equals("200"));
            });
            
            it("should convert an integer", [&]{
                AssertThat(static_cast<unsigned int>(status("200")), Equals(200));
            });
            
            describe("when the value not specified", [&]{
                it("should be 200", [&]{
                    AssertThat(status(), Equals("200"));
                });
            });
        });
        
        describe("::url", [&]{
            it("should consider to equal an URL that is a different form and is same meaning", [&]{
                AssertThat(
                    url("HTTP://WWW.BOOST.ORG?a=1&a=2&b=3"),
                    Equals("http://www.boost.org:80/?b=3&a=2#")
                );
            });
            
            it("should convert `std::string` and normalize", [&]{
                AssertThat(
                    static_cast<std::string>(url("HTTP://WWW.BOOST.ORG:8080?a=1&a=2&b=3")),
                    Equals("http://www.boost.org:8080/?a=2&b=3")
                );
            });
        });
    });
});
}}}
