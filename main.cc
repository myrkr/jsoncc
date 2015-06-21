#include <string>
#include <inttypes.h>

// https://tools.ietf.org/html/rfc7159

class Parser {
public:
	Parser(const char *buf, size_t len)
	:
		stream_(buf, len),
		scanner_(stream_)
	{ }


	Utf8Stream stream_;
	TokenStream scanner_;
};

int main()
{
	char data[] = "{}";

	Parser p(data, sizeof(data) - 1);
}
