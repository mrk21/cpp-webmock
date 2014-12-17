#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/directive/response.hpp>
#include <exception>

namespace webmock { namespace directive {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::directive::response", []{
        describe("response(response)", [&]{
            it("should be the response", [&]{
                response_sequence seq = response({"200"});
                AssertThat(seq.get_response({}), Equals(webmock::response{"200"}));
            });
        });
        
        describe("response(generator)", [&]{
            it("should be a response created by the generator", [&]{
                response_sequence seq = response([](auto && req){
                    return webmock::response{"200", req.method};
                });
                AssertThat(seq.get_response({"GET"}), Equals(webmock::response{"200","GET"}));
                AssertThat(seq.get_response({"PUT"}), Equals(webmock::response{"200","PUT"}));
            });
        });
        
        describe("error<Exception>(Types... args)", [&]{
            it("should throw the Exception with the args", [&]{
                struct my_exception: public std::exception {
                    int a;
                    std::string b;
                    my_exception(std::string const & b) : a(1), b(b) {}
                    my_exception(int a, std::string const & b) : a(a), b(b) {}
                };
                {
                    response_sequence seq = error<my_exception>(2,"hoge");
                    AssertThrows(my_exception, seq.get_response({}));
                    AssertThat(LastException<my_exception>().a, Equals(2));
                    AssertThat(LastException<my_exception>().b, Equals("hoge"));
                }
                {
                    response_sequence seq = error<my_exception>("fuga");
                    AssertThrows(my_exception, seq.get_response({}));
                    AssertThat(LastException<my_exception>().a, Equals(1));
                    AssertThat(LastException<my_exception>().b, Equals("fuga"));
                }
            });
        });
        
        describe("error(T value)", [&]{
            it("should throw the value", [&]{
                {
                    response_sequence seq = error(1);
                    AssertThrows(int, seq.get_response({}));
                    AssertThat(LastException<int>(), Equals(1));
                }
                {
                    response_sequence seq = error("baz");
                    using str_t = char const *;
                    AssertThrows(str_t, seq.get_response({}));
                    AssertThat(LastException<str_t>(), Equals("baz"));
                }
            });
        });
    });
});
}}
