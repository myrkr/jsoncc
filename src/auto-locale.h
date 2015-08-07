/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#ifndef RAD_AUTO_LOCALE_H
#define RAD_AUTO_LOCALE_H

#include <locale.h>

namespace jsonp {
// POSIX thread local locale setting.
class AutoLocale {
public:
	AutoLocale(const char *);
	~AutoLocale();

private:
	locale_t locale_;
	locale_t saved_;
};

}
#endif
