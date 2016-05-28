/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <cassert>
#include <cstring>

#include "parser-impl.h"

#include "error.h"
#include "token-stream.h"
#include "utf8stream.h"

namespace {

/*
 * Generic parser state engine
 * Check all transitions for the current State
 * until the token is found in Transition::match,
 * change to the associated state.
 *
 * Instances of this are build for Document, Array and Object
 */
template <typename State>
struct Transition {
	const char *match;
	State state;
};

template <typename T>
class StateEngine : public T {
public:
	StateEngine(Json::TokenStream & tokenizer_, size_t depth)
	: T(tokenizer_, depth) { }

	Json::Value parse()
	{
		typename T::State state(T::SSTART);
		do {
			T::tokenizer.scan();
			state = transition(T::tokenizer.token.type, state);
		} while (state != T::SEND);

		return T::result;
	}

private:
	typename T::State
	transition(Json::Token::Type token, typename T::State state)
	{
		for (size_t t(0); t < T::SMAX; ++t) {
			if (!is_match(token, T::transitions[state][t].match)) {
				continue;
			}

			typename T::State nstate(T::transitions[state][t].state);
			if (nstate == T::SERROR) {
				T::throw_error(state);
				JSONCC_THROW(INTERNAL_ERROR); // LCOV_EXCL_LINE
			} else {
				T::build(nstate);
			}

			return nstate;
		}

		return T::SERROR;
	}

	bool is_match(Json::Token::Type token, const char *match) const
	{
		return !match ||
			(!match[0] && token == Json::Token::END) ||
			strchr(match, token);
	}
};

/* Base class for state engine configurations */
class ParserState {
protected:
	ParserState(Json::TokenStream & tokenizer_, size_t depth)
	: tokenizer(tokenizer_), depth_(depth + 1)
	{
		if (depth > 255) {
			JSONCC_THROW(PARSER_OVERFLOW);
		}
	}

	Json::Value parse_value();

	Json::TokenStream & tokenizer;

private:
	size_t depth_;
};

/* State engine config for Json::Array */
class ArrayState : public ParserState {
protected:
	ArrayState(Json::TokenStream & tokenizer_, size_t depth)
	: ParserState(tokenizer_, depth) { }

	enum State {
		SERROR = 0,
		SSTART,
		SVALUE,
		SNEXT,
		SEND,
		SMAX,
	};

	void build(State state)
	{
		switch (state) {
		case SVALUE: result << parse_value(); break;
		case SNEXT:  break;
		case SEND:   break;
		case SMAX:   assert(false);           // LCOV_EXCL_LINE
		case SERROR: assert(false);           // LCOV_EXCL_LINE
		case SSTART: assert(false);           // LCOV_EXCL_LINE
			JSONCC_THROW(INTERNAL_ERROR); // LCOV_EXCL_LINE
		}
	}

	void throw_error(State state) const
	{
		switch (state) {
		case SSTART: JSONCC_THROW(BAD_TOKEN_ARRAY_START);
		case SVALUE: JSONCC_THROW(BAD_TOKEN_ARRAY_VALUE);
		case SNEXT:  JSONCC_THROW(BAD_TOKEN_ARRAY_NEXT);
		case SERROR: assert(false); // LCOV_EXCL_LINE
		case SEND:   assert(false); // LCOV_EXCL_LINE
		case SMAX:   assert(false); // LCOV_EXCL_LINE
		}
	}

	Json::Array result;
	static Transition<State> transitions[SMAX][SMAX];
};

Transition<ArrayState::State> ArrayState::transitions[SMAX][SMAX] = {
/* SERROR */ {                                   {0, SERROR}},
/* SSTART */ {{"[{tfn\"0", SVALUE}, {"]", SEND}, {0, SERROR}},
/* SVALUE */ {{",",        SNEXT},  {"]", SEND}, {0, SERROR}},
/* SNEXT  */ {{"[{tfn\"0", SVALUE}, {"]", SEND}, {0, SERROR}},
/* SEND   */ {                                   {0, SERROR}},
};

/* State engine config for Json::Object */
class ObjectState : public ParserState {
protected:
	ObjectState(Json::TokenStream & tokenizer_, size_t depth)
	: ParserState(tokenizer_, depth) { }

	enum State {
		SERROR = 0,
		SSTART,
		SNAME,
		SSEP,
		SVALUE,
		SNEXT,
		SEND,
		SMAX,
	};

	void build(State state)
	{
		switch (state) {
		case SNAME:  key = tokenizer.token.str_value; break;
		case SVALUE: result << Json::Member(key, parse_value()); break;
		case SNEXT:  break;
		case SEND:   break;
		case SSEP:   break;
		case SERROR: assert(false);           // LCOV_EXCL_LINE
		case SSTART: assert(false);           // LCOV_EXCL_LINE
		case SMAX:   assert(false);           // LCOV_EXCL_LINE
			JSONCC_THROW(INTERNAL_ERROR); // LCOV_EXCL_LINE
		}
	}

	void throw_error(State state) const
	{
		switch (state) {
		case SSTART: JSONCC_THROW(BAD_TOKEN_OBJECT_START);
		case SNAME:  JSONCC_THROW(BAD_TOKEN_OBJECT_NAME);
		case SSEP:   JSONCC_THROW(BAD_TOKEN_OBJECT_SEP);
		case SVALUE: JSONCC_THROW(BAD_TOKEN_OBJECT_VALUE);
		case SNEXT:  JSONCC_THROW(BAD_TOKEN_OBJECT_NEXT);
		case SERROR: assert(false); // LCOV_EXCL_LINE
		case SEND:   assert(false); // LCOV_EXCL_LINE
		case SMAX:   assert(false); // LCOV_EXCL_LINE
		}
	}

	std::string key;
	Json::Object result;
	static Transition<State> transitions[SMAX][SMAX];
};

Transition<ObjectState::State> ObjectState::transitions[SMAX][SMAX] = {
/* SO_ERROR */ {                                   {0, SERROR}},
/* SO_START */ {{"\"",       SNAME},  {"}", SEND}, {0, SERROR}},
/* SO_NAME  */ {{":",        SSEP},                {0, SERROR}},
/* SO_SEP   */ {{"[{tfn\"0", SVALUE},              {0, SERROR}},
/* SO_VALUE */ {{",",        SNEXT},  {"}", SEND}, {0, SERROR}},
/* SO_NEXT  */ {{"\"",       SNAME},               {0, SERROR}},
/* SO_END   */ {                                   {0, SERROR}},
};

/* State engine config for a Json document */
class DocState : public ParserState {
protected:
	DocState(Json::TokenStream & tokenizer_, size_t & depth)
	: ParserState(tokenizer_, depth) { }

	enum State {
		SERROR = 0,
		SSTART,
		SVALUE,
		SEND,
		SMAX,
	};

	void build(State state)
	{
		switch (state) {
		case SVALUE: result = parse_value(); break;
		case SEND:   break;
		case SSTART: assert(false);           // LCOV_EXCL_LINE
		case SERROR: assert(false);           // LCOV_EXCL_LINE
		case SMAX:   assert(false);           // LCOV_EXCL_LINE
			JSONCC_THROW(INTERNAL_ERROR); // LCOV_EXCL_LINE
		}
	}

	void throw_error(State state) const
	{
		switch (state) {
		case SSTART: JSONCC_THROW(BAD_TOKEN_DOCUMENT);
		case SVALUE: JSONCC_THROW(BAD_TOKEN_DOCUMENT);
		case SERROR: assert(false); // LCOV_EXCL_LINE
		case SEND:   assert(false); // LCOV_EXCL_LINE
		case SMAX:   assert(false); // LCOV_EXCL_LINE
		}
	}

	Json::Value result;
	static Transition<State> transitions[SMAX][SMAX];
};

Transition<DocState::State> DocState::transitions[SMAX][SMAX] = {
/* SERROR  */ {                            {0, SERROR}},
/* SSTART  */ {{"[{", SVALUE}, {"", SEND}, {0, SERROR}},
/* SVALUE  */ {                {"", SEND}, {0, SERROR}},
/* SEND    */ {                            {0, SERROR}},
};

/* select recursive parser for nested constructs */
Json::Value ParserState::parse_value()
{
	switch (tokenizer.token.type) {
	case Json::Token::TRUE_LITERAL:    return Json::True();
	case Json::Token::FALSE_LITERAL:   return Json::False();
	case Json::Token::NULL_LITERAL:    return Json::Null();
	case Json::Token::STRING:          return tokenizer.token.str_value;
	case Json::Token::NUMBER:
		if (tokenizer.token.number_type == Json::Token::FLOAT) {
			return Json::Number(tokenizer.token.float_value);
		} else {
			return Json::Number(tokenizer.token.int_value);
		}
		break;
	case Json::Token::BEGIN_ARRAY:
		return StateEngine<ArrayState>(tokenizer, depth_).parse();
	case Json::Token::BEGIN_OBJECT:
		return StateEngine<ObjectState>(tokenizer, depth_).parse();
	case Json::Token::END:             assert(false); // LCOV_EXCL_LINE
	case Json::Token::INVALID:         assert(false); // LCOV_EXCL_LINE
	case Json::Token::END_ARRAY:       assert(false); // LCOV_EXCL_LINE
	case Json::Token::END_OBJECT:      assert(false); // LCOV_EXCL_LINE
	case Json::Token::NAME_SEPARATOR:  assert(false); // LCOV_EXCL_LINE
	case Json::Token::VALUE_SEPARATOR: assert(false); // LCOV_EXCL_LINE
	}
	JSONCC_THROW(INTERNAL_ERROR);                     // LCOV_EXCL_LINE
	return Json::Value();
}

}

namespace Json {

/* Toplevel parser for a single document */
Value ParserImpl::parse(char const * data, size_t size)
{
	Utf8Stream utf8stream(data, size);
	TokenStream tokenizer(utf8stream);
	try {
		return StateEngine<DocState>(tokenizer, 0).parse();
	} catch (Error & e) {
		throw;
	}
}

}
