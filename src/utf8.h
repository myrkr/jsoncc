/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#ifndef JSONP_UTF8_H
#define JSONP_UTF8_H

namespace jsonp {

class utf8validator {
public:
	utf8validator();
	bool validate(int);

private:
	int state_;
};

}

#endif
