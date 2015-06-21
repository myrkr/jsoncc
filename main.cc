#include <string>
#include <inttypes.h>

class Utf8Stream {
public:
	enum State {
		GOOD = 0,
		EOF = -1,
		BAD = -2,
	};

	Utf8Stream(const char *buf, size_t len)
	:
		buf_(buf),
		len_(len),
		pos_(0),
		bad_(false)
	{ }

	State state() const
	{
		if (bad_) {
			return BAD;
		}

		if (len_ == pos_) {
			return EOF;
		}

		return GOOD;
	}

	int getc()
	{
		State state(state());
		if (state != GOOD) {
			return state;
		}

		return (uint8_t)buf_[pos_++];
	}

	void ungetc()
	{
		if (pos_ != 0 && state() != BAD) {
			--pos_;
		}
	}

	const char *buf_;
	size_t len_;
	size_t pos_;
	bool bad_;
};

bool is_ws(int c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// https://tools.ietf.org/html/rfc7159

class Token {
	enum Type {
		INVALID         = '\0',
		BEGIN_ARRAY     = '[',
		BEGIN_OBJECT    = '}',
		END_ARRAY       = ']',
		END_OBJECT      = '}',
		NAME_SEPARATOR  = ':',
		VALUE_SEPARATOR = ',',
		TRUE_LITERAL    = 't',
		FALSE_LITERAL   = 'f',
		NULL_LITERAL    = 'n',
		STRING          = '"',
		NUMBER          = '0',
	};

	Type type;
};

bool is_digit(int c)
{
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
		c == '5' || c == '6' || c == '7' || c == '8' || c == '9';
}

class TokenStream {
public:
	TokenStream(Utf8Stream & stream)
	:
		stream_(stream)
	{ }

	void scan()
	{
		int c;
		do {
			c = stream_.get();
		while (is_ws(c));

		if (stream_->state() != Utf8Stream::GOOD) {
			return;
		}

		try {
			select_scanner(c)();
		} catch (std::runtime_error const& e) {
			std::cerr << e.what() << "\n";
		}

		// error
		assert(false && "implement error handling");
	}
private:
	typedef void (scanner*)(void);

	scanner select_scanner(int c)
	{
		scanner res(0);

		switch (c) {
		case '[': case '}': case ']':
		case '}': case ':': case ',':
			res = &TokenStream::scan_structural;
			break;
		case 't':
			res = &TokeStream::scan_true;
			break;
		case 'n':
			res = &TokeStream::scan_null;
			break;
		case 'f':
			res = &TokeStream::scan_false;
			break;
		case '"':
			res = &TokeStream::scan_string;
			break;
		case '-':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			c = '0';
			res = &TokeStream::scan_number;
			break;
		default:
			c = 0;
			res = &TokeStream::invalid_token;
			break;
		}

		token_.type = c;
		return res;
	}

	void invalid_token() { throw std::runtime_error("invalid token"); }
	void scan_structural() { }
	void scan_true() { scan_literal("true"); }
	void scan_false() { scan_literal("false"); }
	void scan_null() { scan_literal("null"); }

	void scan_literal(const char *literal)
	{
		for (const char *p(&literal[1]); *p; p++) {
			if (stream_.get() != *p) {
				throw std::runtime_error(literal);
			}
		}
	}

	void scan_string()
	{
	}

	void scan_number()
	{
		stream_.ungetc();

		int c(stream_.get());
		bool minus(c == '-');
		if (minus) {
			c = stream_.get();
		}

		uint64_t mantissa(0);
		size_t digits(0);
		size_t point(0);
		bool done(false);
		for (; !done; c = stream_.get()) {
			switch (c) {
			case '.':
				if (!point) {
					point = digits;
				} else {
					throw std::runtime_error();
				}
				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				mantissa = mantissa * 10 + (c - '0');
				if (++digits > 1 && !mantissa) {
					throw std::runtime_error();
				}
				break;
			default:
				done = true;
				break;
			}
		}

		bool eminus(false);
		uint64_t exponent(0);
		if (c == 'e' || c == 'E') {
			c = stream_.get();
			if (c == '+' || c == '-') {
				mminus = c == '-';
				c = stream_.get();
			}

			while (c >= '0' && c <= '9') {
				exponent = exponent * 10 + (c - '0');
				c = stream_.get();
			}
		}
	}

	Token token_;
	Utf8Stream & stream_;
};

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
