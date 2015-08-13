/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include "auto-locale.h"

namespace Json {

AutoLocale::AutoLocale(const char *name)
:
	locale_(newlocale(LC_ALL_MASK, name, 0)),
	saved_(uselocale(locale_))
{ }

AutoLocale::~AutoLocale()
{
	uselocale(saved_);
	freelocale(locale_);
}

}
