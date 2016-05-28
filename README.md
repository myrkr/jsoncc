[![Build Status](https://travis-ci.org/afett/jsoncc.svg?branch=master)](https://travis-ci.org/afett/jsoncc)
[![Coverty Scan](https://scan.coverity.com/projects/4527/badge.svg)](https://scan.coverity.com/projects/4527)

jsoncc

simple json writer and parser for C++

BSD licensed, see LICENSE.

Parser limitations:
 * no support for utf16 surrogate encoding in strings
 * unicode escape for 0 ie \u0000 is treated as an error
