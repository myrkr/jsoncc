#include "parser-impl.h"

#include "token-stream.h"
#include "utf8stream.h"

namespace jsonp {

void ParserImpl::parse(char const * data, size_t size)
{
	Utf8Stream utf8stream(data, size);
	TokenStream tokens(utf8stream);
}

}
