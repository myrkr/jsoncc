#include <ostream>

#include <jsonp.h>

namespace jsonp {

static inline std::ostream & operator<<(std::ostream & os, Error::Type type)
{
#define CASE_ERROR_TYPE(name) case name: os << # name; break
	switch (type) {
	CASE_ERROR_TYPE(Error::OK);
	CASE_ERROR_TYPE(Error::STREAM_ZERO);
	CASE_ERROR_TYPE(Error::UTF8_INVALID);
	CASE_ERROR_TYPE(Error::TOKEN_INVALID);
	CASE_ERROR_TYPE(Error::LITERAL_INVALID);
	CASE_ERROR_TYPE(Error::STRING_CTRL);
	CASE_ERROR_TYPE(Error::STRING_QUOTE);
	CASE_ERROR_TYPE(Error::ESCAPE_INVALID);
	CASE_ERROR_TYPE(Error::UESCAPE_INVALID);
	CASE_ERROR_TYPE(Error::UESCAPE_ZERO);
	CASE_ERROR_TYPE(Error::UESCAPE_SURROGATE);
	CASE_ERROR_TYPE(Error::NUMBER_INVALID);
	CASE_ERROR_TYPE(Error::NUMBER_OVERFLOW);
	}
#undef CASE_ERROR_TYPE
	return os;
}

}
