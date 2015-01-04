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
                
                AssertThat(
                    url("http://www.boost.org/a/b//c"),
                    Equals("http://www.boost.org/a/b/c")
                );
                
                AssertThat(
                    url("http://www.boost.org/a/b/./c"),
                    Equals("http://www.boost.org/a/b/c")
                );
                
                AssertThat(
                    url("http://www.boost.org/a/b/../c"),
                    Equals("http://www.boost.org/a/c")
                );
                
                AssertThat(
                    url("http://www.boost.org/a/b/../../../c"),
                    Equals("http://www.boost.org/c")
                );
            });
            
            describe("when the scheme was none", [&]{
                it("should match the any scheme", [&]{
                    AssertThat(
                        url("//www.boost.org:8080"),
                        Equals("http://www.boost.org:8080")
                    );
                    
                    AssertThat(
                        url("//www.boost.org:8080"),
                        Equals("//www.boost.org:8080")
                    );
                    
                    AssertThat(
                        url("//www.boost.org:8080"),
                        not Equals("http://www.boost.org")
                    );
                });
                
                describe("when the port was none", [&]{
                    it("should match the any port", [&]{
                        AssertThat(
                            url("//www.boost.org"),
                            Equals("https://www.boost.org:8080")
                        );
                        
                        AssertThat(
                            url("//www.boost.org"),
                            Equals("//www.boost.org:8080")
                        );
                    });
                });
            });
            
            it("should convert `std::string` and normalize", [&]{
                url target("HTTP://WWW.BOOST.ORG:8080/../../ a%20あ?a=1&a=2&b=3");
                AssertThat(
                    static_cast<std::string>(target),
                    Equals("http://www.boost.org:8080/ a あ?a=2&b=3")
                );
                AssertThat(
                    boost::lexical_cast<std::string>(target),
                    Equals("http://www.boost.org:8080/%20a%20%E3%81%82?a=2&b=3")
                );
            });
        });
    });
});
}}}
