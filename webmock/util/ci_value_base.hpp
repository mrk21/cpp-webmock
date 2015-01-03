#ifndef WEBMOCK_UTIL_CI_VALUE_BASE_HPP
#define WEBMOCK_UTIL_CI_VALUE_BASE_HPP

#include <string>
#include <cctype>
#include <boost/operators.hpp>

namespace webmock { namespace util {
    template <typename Derived>
    class ci_value_base:
        boost::equality_comparable<Derived, Derived>,
        boost::less_than_comparable<Derived, Derived>
    {
    protected:
        struct ci_char_traits : public std::char_traits<char> {
            static bool eq(char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
            static bool ne(char c1, char c2) { return std::toupper(c1) != std::toupper(c2); }
            static bool lt(char c1, char c2) { return std::toupper(c1) <  std::toupper(c2); }
            static int compare(char const * s1, char const * s2, std::size_t n) {
                while (n-- != 0) {
                    if (std::toupper(*s1) < std::toupper(*s2)) return -1;
                    if (std::toupper(*s1) > std::toupper(*s2)) return 1;
                    ++s1; ++s2;
                }
                return 0;
            }
            static char const * find(char const * s, int n, char a) {
                while (n-- > 0 && std::toupper(*s) != std::toupper(a)) {
                    ++s;
                }
                return s;
            }
        };
        using ci_string = std::basic_string<char, ci_char_traits>;
        
        ci_string data;
        
    public:
        ci_value_base() = default;
        ci_value_base(std::string const & v) : data(v.c_str()) {}
        ci_value_base(char const * v) : data(v) {}
        
        ci_value_base & operator =(char const * rop) {
            this->data = rop;
            return *this;
        }
        
        ci_value_base & operator =(std::string const & rop) {
            this->data = rop.c_str();
            return *this;
        }
        
        operator std::string() const {
            return this->data.c_str();
        }
        
        friend bool operator ==(Derived const & lop, Derived const & rop) {
            return lop.data == rop.data;
        }
        
        friend bool operator <(Derived const & lop, Derived const & rop) {
            return lop.data < rop.data;
        }
        
        friend std::ostream & operator <<(std::ostream & lop, Derived const & rop) {
            return lop << static_cast<std::string>(rop);
        }
    };
}}

#endif
