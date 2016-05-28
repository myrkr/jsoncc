/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

namespace Json {

class utf8validator {
public:
	utf8validator();
	bool validate(int);

private:
	int state_;
};

}
