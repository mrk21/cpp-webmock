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
    std::cout << "access count: "                                                                   // 4.
        << a_request("http://www.boost.org/").conditions(with_header("Connection","Close")).count() // 4.
        << std::endl;                                                                               // 4.
    
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

## How to install

The cpp-webmock is header-only library, so you does not need to build. In order to use this library, you should prepare that listed below:

* Clang 3.5 (with -std=c++14)
* Boost 1.56
* HTTP library (In the current, it corresponds cpp-netlib only)

If you use CMake to build, you might want to as following:

```cmake
cmake_minimum_required(VERSION 3.0.2)

find_package(Boost 1.56 REQUIRED system thread)
add_compile_options(-std=gnu++14 -stdlib=libc++)
include_directories(${Boost_INCLUDE_DIRS} /path/to/cpp-webmock)
set(libs ${Boost_LIBRARIES})

# If you use cpp-webmock, you need this code: [[
 include_directories(/path/to/cpp-netlib)
 link_directories(/path/to/cpp-netlib/libs/network/src)
 list(APPEND libs
   cppnetlib-uri
   cppnetlib-client-connections
 )
# ]]

# If you want to use SSL on cpp-netlib, you need this code: [[
  find_package(OpenSSL REQUIRED)
  include_directories(${OPENSSL_INCLUDE_DIR})
  list(APPEND libs ${OPENSSL_LIBRARIES})
  add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
# ]]

add_executable(bin src.cpp)
target_link_libraries(bin ${libs})
```

## Reference


