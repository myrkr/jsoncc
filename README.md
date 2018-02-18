[![Build Status](https://travis-ci.org/afett/jsoncc.svg?branch=master)](https://travis-ci.org/afett/jsoncc)
[![Coverty Scan](https://scan.coverity.com/projects/4527/badge.svg)](https://scan.coverity.com/projects/4527)

jsoncc

Simple json writer and parser for C++. This library is not optimised for speed
but for correctness and ease of use.

BSD licensed, see LICENSE.

Parser limitations:
 * no support for utf16 surrogate encoding in strings
 * unicode escape for 0 ie \u0000 is treated as an error
