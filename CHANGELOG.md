# Changelog

## [0.3.1](https://github.com/mrk21/cpp-webmock/tree/v0.3.1) - 2015-01-15

* Bugfix [#20](https://github.com/mrk21/cpp-webmock/issues/20): Fail to build on immediately after the `git clone`

## [0.3.0](https://github.com/mrk21/cpp-webmock/tree/v0.3.0) - 2015-01-14

* Future [#18](https://github.com/mrk21/cpp-webmock/issues/18): Downloading and building of the external projects to embed to CMake script
* Future [#13](https://github.com/mrk21/cpp-webmock/issues/13): To be able to easy use on the external project that uses CMake
* Future [#12](https://github.com/mrk21/cpp-webmock/issues/12): Remedy build time

## [0.2.0](https://github.com/mrk21/cpp-webmock/tree/v0.2.0) - 2015-01-06

* Future [#10](https://github.com/mrk21/cpp-webmock/issues/10): Adds the function of resetting the request log and the stub list
* Bugfix [#8](https://github.com/mrk21/cpp-webmock/issues/8): Is not thread safe
* Future [#5](https://github.com/mrk21/cpp-webmock/issues/5): Stops returning an empty response when a stub satisfying the request not found
* Bugfix [#4](https://github.com/mrk21/cpp-webmock/issues/4): Some URLs should be interpreted to the same, but the `with_url` ends up not interpreting to that
* Bugfix [#3](https://github.com/mrk21/cpp-webmock/issues/3): A header name of the `with_header` ends up case-sensitive
* Bugfix [#2](https://github.com/mrk21/cpp-webmock/issues/2): HTTP status of a response will be empty if the status not specified on the `a_response`
* Bugfix [#1](https://github.com/mrk21/cpp-webmock/issues/1): The `with_method` ends up case-sensitive
