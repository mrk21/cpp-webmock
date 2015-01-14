cpp-webmock
=====================

The cpp-webmock is HTTP mocking library that was created by referencing to [WebMock](https://github.com/bblimke/webmock "bblimke/webmock") of Ruby.

## Introduction

In order to introduce cpp-webmock, we need basing on steps listed below:

First, if we had the following code that is using the [cpp-netlib](http://cpp-netlib.org/ "The C++ Network Library Project") that is HTTP library:

```cpp
#include <iostream>
#include <boost/network/protocol/http/client.hpp>

int main() {
    namespace network = boost::network;
    namespace http = network::http;
    using client_type = http::client;
    
    client_type::request request("http://www.boost.org/");
    request << network::header("Connection","Close");
    client_type client;
    client_type::response response = client.get(request);
    
    std::cout << "status: " << http::status(response) << std::endl;
    std::cout << "body: " << http::body(response) << std::endl;
    
    return 0;
}
```

When this code built and executed, it outputs as shown below:

```bash
status: 200
body: <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
 <meta name="generator" content=
 "HTML Tidy for Windows (vers 1st November 2003), see www.w3.org" />
 
 <title>Boost C++ Libraries</title>
...
```

Next, we will continue to introduce cpp-webmock to this code based on steps listed below:

1. Include `webmock/api.hpp` and `webmock/adapter/cpp_netlib.hpp`
1. Change the client type from `boost::network::http::client` to `webmock::adapter::cpp_netlib::client`
1. Set the behavior when the specifying URL accessed by `a_stub`
1. If you want to verify whether the client accessing the specified URL, you will use `a_request`

```cpp
#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include <webmock/api.hpp>                // 1.
#include <webmock/adapter/cpp_netlib.hpp> // 1.

int main() {
    namespace network = boost::network;
    namespace http = network::http;
    using client_type = webmock::adapter::cpp_netlib::client; // 2.
    
    using namespace webmock::api::directive;                            // 3.
    a_stub("http://www.boost.org/").returns(a_response().body("test")); // 3.
    
    client_type::request request("http://www.boost.org/");
    request << network::header("Connection","Close");
    client_type client;
    client_type::response response = client.get(request);
    
    std::cout << "status: " << http::status(response) << std::endl;
    std::cout << "body: " << http::body(response) << std::endl;
    std::cout << "access count: "                                  // 4.
        << a_request("http://www.boost.org/")                      // 4.
            .conditions(with_header("Connection","Close")).count() // 4.
        << std::endl;                                              // 4.
    
    return 0;
}
```

Finally, let's see what this code can execute:

```
status: 200
body: test
access count: 1
```

We did it! The response that is set by `a_stub` has been returned!

## Installation

The cpp-webmock is header-only library, so you do not need to build. In order to use this library, you should prepare that listed below:

* Clang 3.5 (with -std=c++14)
* Boost 1.56
* HTTP library (In the current, it corresponds cpp-netlib only)

If you use CMake to build, you might want to as following:

First, you add the `external` directory and `CMakeLists.txt` to your project:

```
project_root
├── CMakeLists.txt
├── src.cpp
└── external            # Adding
     └── CMakeLists.txt # Adding
```

Second, you write the following content to added `external/CMakeLists.txt`:

```cmake:external/CMakeLists.txt
cmake_minimum_required(VERSION 3.0.2)

include(ExternalProject)

ExternalProject_Add(webmock
  GIT_REPOSITORY https://github.com/mrk21/cpp-webmock.git
  GIT_TAG v0.3.0
  PREFIX webmock
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DBUILD_LIBRARY=ON
)
```

Finally, you append the following content to your project's `CMakeLists.txt`:

```cmake:CMakeLists.txt
cmake_minimum_required(VERSION 3.0.2)

message("Building externals...")
try_compile(external_status
  ${PROJECT_BINARY_DIR}/external
  ${PROJECT_SOURCE_DIR}/external
  external
  OUTPUT_VARIABLE external_result
)
if(NOT external_status)
  message("${external_result}")
  return()
endif()
message("Built externals")
include(${PROJECT_BINARY_DIR}/external/webmock/lib/webmock.cmake)

add_compile_options(-std=gnu++14)
add_executable(bin src.cpp)
target_link_libraries(bin webmock::webmock)
```

## Reference

* [webmock::request](#webmockrequest)
* [webmock::response](#webmockresponse)
* [webmock::api::a\_stub](#webmockapia_stub)
    * [constructor](#constructor)
    * [returns()](#returns)
    * [conditions()](#conditions)
    * [operator &lt;&lt;](#operator-)
    * [count()](#count)
    * [operator std::size\_t](#operator-stdsize_t)
* [webmock::api::a\_request](#webmockapia_request)
* [webmock::api::a\_response](#webmockapia_response)
* [webmock::api::an\_error](#webmockapian_error)
* [webmock::api::with\_method](#webmockapiwith_method)
* [webmock::api::with\_url](#webmockapiwith_url)
* [webmock::api::with\_body](#webmockapiwith_body)
* [webmock::api::with\_header](#webmockapiwith_header)
* [webmock::api::with](#webmockapiwith)
* [webmock::api::directive](#webmockapidirective)
* [webmock::api::reset()](#webmockapireset)
* [webmock::api::allow\_connecting\_to\_net()](#webmockapiallow_connecting_to_net)
* [webmock::api::disallow\_connecting\_to\_net()](#webmockapidisallow_connecting_to_net)
* [webmock::api::stub\_not\_found\_callback()](#webmockapistub_not_found_callback)
* [webmock::adapter::cpp\_netlib::basic\_client](#webmockadaptercpp_netlibbasic_client)
* [webmock::adapter::cpp\_netlib::select\_by\_type](#webmockadaptercpp_netlibselect_by_type)
* [webmock::adapter::cpp\_netlib::select\_by\_param](#webmockadaptercpp_netlibselect_by_param)

### webmock::request

The `webmock::request` is a structure to store a content of the request from the client.

```cpp
struct request {
    core::http::method method;
    core::http::url url;
    core::http::headers headers;
    core::http::body body;
};
```

This structure is defined the stream operator and comparative operator.

### webmock::response

The `webmock::response` is a structure to store a response returning to the client.

```cpp
struct response {
    core::http::status status;
    core::http::body body;
    core::http::headers headers;
};
```

This structure is defined the stream operator and comparative operator.

### webmock::api::a\_stub

The `a_stub` class sets a behavior of when specifying URL was accessed. This class has methods and operators listed below:

#### constructor

```cpp
a_stub::a_stub();
a_stub::a_stub(with_url const & url);
a_stub::a_stub(with_method const & method, with_url const & url);
```

The `method` argument specifies a HTTP method, and the `url` argument specifies an URL.

A stub set by `a_stub` will be inserted to a head of the stub list. When the client accessed, a stub matching the request will be found by linear search since a head in the stub list. Therefore, if it is found multiple, selects a stub added after.

```cpp
a_stub("http://www.boost.org/").returns(a_response().body("response 1"));
a_stub("http://www.boost.org/").returns(a_response().body("response 2"));

client_type::request request("http://www.boost.org/");
client_type client;
client_type::response response = client.get(request);

std::cout << http::body(response) << std::endl; // response 2
```

#### returns()

```cpp
a_stub & a_stub::returns(core::response_sequence const & response_sequence[, ...]);
```

The `returns()` method sets the behavior returning a response. The behavior can be set multiple, and it returns the response on specified order when the client accessed every time. If the number of access was greater than the number of the specified behaviors, it keeps to return a response of the behavior last specified.

```cpp
a_stub(url)
    .returns(a_response().body("response 1"))
    .returns(a_response().body("response 2"))
    .returns(a_response().body("response 3"));
```

For example, if it specified as above, when the client accessed the `url`, it will return responses shown below:

```
response 1
response 2
response 3
response 3
...
```

In addition, it can specify multiple as shown below:

```cpp
a_stub(url).returns(
    a_response().body("response 1"),
    a_response().body("response 2"),
    a_response().body("response 3")
);
```

#### conditions()

```cpp
a_stub & a_stub::conditions(core::condition_list::condition_type condition[, ...]);
```

The `condition()` method, such as whether to exist HTTP header and a condition of a request body, adds conditions of a request. This conditions are such as `with_method` and `with_body`. For example, if HTTP method was `GET` and the request body was `test`, it returns `400`:

```cpp
a_stub()
    .conditions(with_method("GET"))
    .conditions(with_body("test"))
    .returns(a_response().status(400));
```

The `condition()` method can also specify as multiple as `returns()` method:

```cpp
a_stub().conditions(
    with_method("GET"),
    with_body("test")
).returns(a_response().status(400));
```

#### operator &lt;&lt;

```cpp
a_stub & operator <<(a_stub & lop, core::response_sequence const & rop);
a_stub & operator <<(a_stub & lop, core::condition_list::condition_type rop);
```

In the above, I said to use the `returns()` method if you want to specify responses, and said to use the `conditions()` method if want to add conditions, but you can also realize these functions by using the `<<` operator.

```cpp
a_stub()
    << with_method("GET")
    << with_body("test")
    << a_response().status(400);
```

#### count()

```cpp
std::size_t a_stub::count() const;
```

This method returns the number of responses returned–the number of requested.

```cpp
auto && stub = a_stub("http://www.hogebar.com/").returns(a_response());

client_type client;
client.get(client_type::request{"http://www.hogebar.com/"});
client.get(client_type::request{"http://www.hogebar.com/"});

std::cout << stub.count() << std::endl; // 2
```

#### operator std::size\_t

```cpp
std::size_t a_stub::operator std::size_t() const;
```

This is conversion operator to convert to an integer type. The returned value equals to the value getting from `count()` method:

```cpp
std::cout << std::boolalpha << (stub < 3) << std::endl; // true
```

### webmock::api::a\_request

The `a_request` class is for verifying how the client requested, and it is removed from the `a_stub` the function creating responses and the function registering to the stub list. This will use to verify requests to multiple stubs:

```cpp
a_stub("http://www.hogebar.com/a").returns(a_response());
a_stub("http://www.hogebar.com/b").returns(a_response());

client_type client;
client.get(client_type::request{"http://www.hogebar.com/a"});
client.get(client_type::request{"http://www.hogebar.com/b"});

std::cout << a_request(std::regex("http://www.hogebar.com/.*")).count() << std::endl; // 2
```

### webmock::api::a\_response

```cpp
static_response a_response(core::response const & response = {});
dynamic_response a_response(dynamic_response::generator_type generator);
```

The `a_response` function generates a response to specify to the `stub::returns()` method.

The first form generates a static response not depended to the request. The `response` specifies `webmock::response` that is a content of a response:

```cpp
a_response({200, "test", {
    {"Content-Type", "text/plane"},
    {"Set-Cookie", "a=1"},
    {"Set-Cookie", "b=2"}
}});
```

The following methods can also specify the content of the response as above:

```
static_response & static_response::status(core::http::status const & status);
static_response & static_response::body(core::http::body const & body);
static_response & static_response::header(
    core::http::header_name const & name,
    core::http::header_value const & value
);
```

```cpp
a_response().status(200).body("test")
    .header("Content-Type", "text/plane")
    .header("Set-Cookie", "a=1")
    .header("Set-Cookie", "b=2");
```

The second form generates a response by the request. The `generator` specifies an unary function that receives a request from the client:

```cpp
a_response([](webmock::request const & request){
    webmock::response response;
    response.status = 200;
    response.body = request.body;
    return response;
});
```

This form cannot use above methods such as `status()`.

In addition, any form can return same response a number of times specified by `times()` method or operator `*`:

```cpp
a_stub(url)
    << a_response().body("response 1").times(2)
    << a_response().body("response 2") * 2
    << a_response().body("response 3");
```

```
response 1
response 1
response 2
response 2
response 3
response 3
...
```

### webmock::api::an\_error

```cpp
template <typename T>
error_response an_error(T value);

template <typename Exception, typename... Types>
error_response an_error(Types... args);
```

This function also specifies to the `a_stub::returns()` method in the same way as `a_response()` function, but it is not returning a response but throwing an exception. This function will use to reproduce such as name resolution failure(The cpp-netlib throws an exception if name resolution failed).

The first form throws the value `value`, but the second form throws an instance that is generated by a constructor of the exception class `Exception` with arguments `args`.

In addition, any form can throw multiple exceptions a number of times specified by `times()` method or operator `*` in the same way as `a_response`:

```cpp
a_stub(url)
    << an_error("Host not found (authoritative)") * 2
    << an_error<std::logic_error>("Host not found (authoritative)");
```

### webmock::api::with\_method

```cpp
with_method(core::http::method const & value);
with_method(std::regex const & regex);
```

The `with_method` class instance is for specifying to `a_stub::conditions()` method, and its constructor specifies a request HTTP method.

The first form determines whether to equal to the specified value `value`, and the second form determines whether to match to the specified regular expression `regex`.

### webmock::api::with\_url

```cpp
with_url(core::http::url const & value);
with_url(std::regex const & regex);
```

The `with_url` class instance is for specifying to `a_stub::conditions()` method, and its constructor specifies a request URL.

The first form determines whether to equal to the specified value `value`, and the second form determines whether to match to the specified regular expression `regex`.

### webmock::api::with\_body

```cpp
with_body(core::http::body const & value);
with_body(std::regex const & regex);
```

The `with_body` class instance is for specifying to `a_stub::conditions()` method, and its constructor specifies a request body.

The first form determines whether to equal to the specified value `value`, and the second form determines whether to match to the specified regular expression `regex`.

### webmock::api::with\_header

```cpp
with_header(core::http::header_name const & name, core::http::header_value const & value);
with_header(core::http::header_name const & name, std::regex const & regex);
```

The `with_header` class instance is for specifying to `a_stub::conditions()` method, and its constructor specifies a request header.

The first form determines whether the value that corresponds to specified header name `name` to equal to the value `value`, and the second form determines whether to match to the regular expression `regex`.

### webmock::api::with

```cpp
with(core::condition_list::condition_type condition);
```

The `with` class instance is for specifying to `a_stub::conditions()` method, and its constructor specifies an unary function that receives the request from the client. This class will use to check the request particularly:

```cpp
a_stub(url).conditions(with([](webmock::request const & request){
    using namespace boost::property_tree;
    ptree json;
    std::istringstream iss(request.body);
    json_parser::read_json(iss, json);
    if (auto && id = json.get_optional<int>("user.id")) {
        if (*id >= 100) return true;
    }
    return false;
}));
```

In addition, the `with` class does not use, and instead the unary function can specify also directly.

```cpp
a_stub(url).conditions([](webmock::request const & request){
    ...
});
```

### webmock::api::directive

This namespace is inline namespace, and it defines the directives such as `a_stub`, `a_response` and `with_method`. You will become not needing to write the namespace of the directives by using "using directive" to this namespace. The symbol names defined in this namespace are not general such as `a_stub`, `a_response` and `with_method`, so it will not conflict even if you used "using directive".

```cpp
using namespace webmock::api::directive;

a_stub("http://www.boost.org")
    .conditions(with_method("GET"))
    .returns(a_response().body("response1"));

a_request("http://www.boost.org").count();
```

### webmock::api::reset()

```cpp
void reset();
```

This function clears the stub list and access histories. It will use on before and after each test.

### webmock::api::allow\_connecting\_to\_net()

```cpp
void allow_connecting_to_net();
```

This function enables `is_connecting_to_net` option. The client usually throws an exception if the stub satisfying the request not found, but if this option enabled, instead the corresponding original client accesses to the network really.

### webmock::api::disallow\_connecting\_to\_net()

```cpp
void disallow_connecting_to_net();
```

This function disables `is_connecting_to_net` option.

### webmock::api::stub\_not\_found\_callback()

```cpp
void stub_not_found_callback(
    application::stub_not_found_callback_type callback = nullptr
);
```

This function specifies the callback `callback` called when a stub satisfying the request not found. This callback is an unary function that receives the request, and it will use to throw an exception for assertion of testing framework. If the callback not specified, the callback will be set the default callback. In addition, if `is_connecting_to_net` option enabled, this callback is never called.

```cpp
webmock::api::stub_not_found_callback([](webmock::request const & request){
    throw my_exception((std::ostringstream()
        << "A stub satisfying the request not found!\n"
        << request
    ).str());
});
```

### webmock::adapter::cpp\_netlib::basic\_client

This type is for substituting `boost::network::http::basic_client`. It is template type, and its template parameter specifies the original client type.

```cpp
using namespace boost::network;
using namespace webmock::adapter;

using original_client_type = http::basic_client<http::tags::http_default_8bit_tcp_resolve,1,1>;
using client_type = cpp_netlib::basic_client<original_client_type>;
```

In addition, it defined alias `adapter::cpp_netlib::client` of the client type that specified the `boost::network::http::client` to the template parameter.

### webmock::adapter::cpp\_netlib::select\_by\_type

You will usually use the macro shown below to switch the original client type and the client type.

```cpp
#include <boost/network/protocol/http/client.hpp>

#define CPP_NETLIB_CLIENT boost::network::http::client
#ifndef USE_WEBMOCK
using client_type = CPP_NETLIB_CLIENT;
#else
#include <webmock/adapter/cpp_netlib.hpp>
using client_type = webmock::adapter::cpp_netlib::basic_client<CPP_NETLIB_CLIENT>;
#endif
#undef CPP_NETLIB_CLIENT

int main() {
    client_type client;
    ...
    return 0;
}
```

However, if you want to switch these for little use, to implement this macro all of the time is bother. At times like this, when you use `select_by_type` meta function, you can switch easy.

```cpp
#include <boost/network/protocol/http/client.hpp>
#include <webmock/adapter/cpp_netlib.hpp>

int main() {
    namespace http = boost::network::http;
    namespace webmock_adapter = webmock::adapter::cpp_netlib;
    using client_type = typename webmock_adapter::select_by_type<USE_WEBMOCK, http::client>::type;
    
    client_type client;
    ...
    return 0;
}
```

The first argument specifies a boolean value of whether to switch these. If specified the value that is evaluated to true, selects the adapter client type, and if specified the value that is evaluated to false, selects the original client type. The second argument specifies the original client type that will be base.

### webmock::adapter::cpp\_netlib::select\_by\_param

This meta function can switch these as in the same way as `select_by_type` meta function, but instead it specifies the template parameters.

```cpp
using client_type = typename webmock_adapter::select_by_param<USE_WEBMOCK,
    http::tags::http_default_8bit_tcp_resolve, 1, 1
>::type;
```
