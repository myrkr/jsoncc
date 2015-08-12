/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <cassert>
#include <cstring>

#include "parser-impl.h"

#include "token-stream.h"
#include "utf8stream.h"

namespace {

bool must_recurse(jsonp::Token::Type token)
{
	return token == jsonp::Token::BEGIN_ARRAY ||
		token == jsonp::Token::BEGIN_OBJECT;
}

bool is_match(jsonp::Token::Type token, const char *match)
{
	return !match ||
		(!match[0] && token == jsonp::Token::END) ||
		strchr(match, token);
}

enum ParserState {
	SDOC = 0,
	SA_START,
	SA_VALUE,
	SA_NEXT,
	SA_STOP,
	SO_START,
	SO_NAME,
	SO_SEP,
	SO_VALUE,
	SO_NEXT,
	SO_STOP,
	SEND,
	SERROR,
	SMAX,
};

ParserState parser_state(jsonp::Token::Type token, ParserState state, bool rec)
{
	assert(state < SMAX);
	if (!(state < SMAX)) {
		throw jsonp::Error(jsonp::Error::INTERNAL_ERROR);
	}

	struct {
		const char *match;
		ParserState state;
	} transitions[2][SMAX][3] = {{
		/* SDOC */     {{"[{",       SDOC},     {"",  SEND},    {0, SERROR}},
		/* SA_START */ {{"[{tfn\"0", SA_VALUE}, {"]", SA_STOP}, {0, SERROR}},
		/* SA_VALUE */ {{",",        SA_NEXT},  {"]", SA_STOP}, {0, SERROR}},
		/* SA_NEXT */  {{"[{tfn\"0", SA_VALUE},                 {0, SERROR}},
		/* SA_STOP */  {                                        {0, SERROR}},
		/* SO_START */ {{"\"",       SO_NAME},  {"}", SO_STOP}, {0, SERROR}},
		/* SO_NAME */  {{":",        SO_SEP},                   {0, SERROR}},
		/* SO_SEP */   {{"[{tfn\"0", SO_VALUE},                 {0, SERROR}},
		/* SO_VALUE */ {{",",        SO_NEXT},  {"}", SO_STOP}, {0, SERROR}},
		/* SO_NEXT */  {{"\"",       SO_NAME},                  {0, SERROR}},
		/* SO_STOP */  {                                        {0, SERROR}},
		/* SEND */     {                                        {0, SERROR}},
		/* SERROR */   {                                        {0, SERROR}},
	},{
		/* SDOC */     {{"[", SA_START}, {"{", SO_START}, {0, SERROR}},
		/* SA_START */ {{"[", SA_START}, {"{", SO_START}, {0, SERROR}},
		/* SA_VALUE */ {                                  {0, SERROR}},
		/* SA_NEXT */  {{"[", SA_START}, {"{", SO_START}, {0, SERROR}},
		/* SA_STOP */  {                                  {0, SERROR}},
		/* SO_START */ {                                  {0, SERROR}},
		/* SO_NAME */  {                                  {0, SERROR}},
		/* SO_SEP */   {{"[", SA_START}, {"{", SO_START}, {0, SERROR}},
		/* SO_VALUE */ {                                  {0, SERROR}},
		/* SO_NEXT */  {                                  {0, SERROR}},
		/* SO_STOP */  {                                  {0, SERROR}},
		/* SEND */     {                                  {0, SERROR}},
		/* SERROR */   {                                  {0, SERROR}},
	}};

	for (size_t t(0); true; ++t) {
		if (is_match(token, transitions[rec][state][t].match)) {
			return transitions[rec][state][t].state;
		}
	}

	return SERROR;
}

jsonp::Error::Type parser_error(ParserState state)
{
	switch (state) {
	case SDOC:     return jsonp::Error::BAD_TOKEN_DOCUMENT;
	case SA_START: return jsonp::Error::BAD_TOKEN_ARRAY_START;
	case SA_VALUE: return jsonp::Error::BAD_TOKEN_ARRAY_VALUE;
	case SA_NEXT:  return jsonp::Error::BAD_TOKEN_ARRAY_NEXT;
	case SO_START: return jsonp::Error::BAD_TOKEN_OBJECT_START;
	case SO_NAME:  return jsonp::Error::BAD_TOKEN_OBJECT_NAME;
	case SO_SEP:   return jsonp::Error::BAD_TOKEN_OBJECT_SEP;
	case SO_VALUE: return jsonp::Error::BAD_TOKEN_OBJECT_VALUE;
	case SO_NEXT:  return jsonp::Error::BAD_TOKEN_OBJECT_NEXT;
	case SEND:     assert(false);
	case SERROR:   assert(false);
	case SO_STOP:  assert(false);
	case SA_STOP:  assert(false);
	case SMAX:     assert(false);
		break;
	}

	return jsonp::Error::INTERNAL_ERROR;
}

class Stack {
public:
	Stack()
	:
		top_(0)
	{
		data_[0] = SDOC;
	}

	ParserState & top()
	{
		return data_[top_];
	}

	void push()
	{
		if (top_ == (sizeof(data_) / sizeof(ParserState) - 1)) {
			throw jsonp::Error(jsonp::Error::PARSER_OVERFLOW);
		}
		data_[++top_] = SERROR;
	}

	void pop()
	{
		assert(top_ > 0);
		if (!(top_ > 0)) {
			throw jsonp::Error(jsonp::Error::INTERNAL_ERROR);
		}
		--top_;
	}

private:
	size_t top_;
	ParserState data_[256];
};

}

namespace jsonp {

void ParserImpl::parse(char const * data, size_t size)
{
	Utf8Stream utf8stream(data, size);
	TokenStream tokenizer(utf8stream);
	try {
		parse_document(tokenizer);
	} catch (Error & e) {
		throw;
	}
}

void ParserImpl::parse_document(TokenStream & tokenizer)
{
	Stack stack;
	for (;;) {
		tokenizer.scan();
		bool rec(false);
		ParserState state(stack.top());
recurse:
		stack.top() = parser_state(tokenizer.token.type, state, rec);
		switch (stack.top()) {
		case SA_START:
			// construct array
			break;
		case SA_VALUE:
			// append value
			break;
		case SO_START:
			// construct object
			break;
		case SO_NAME:
			// construct member
			break;
		case SO_VALUE:
			// append value
			break;
		case SA_NEXT: case SO_SEP:
		case SO_NEXT: case SDOC:
			// noop get next token
			break;
		case SA_STOP: case SO_STOP:
			stack.pop();
			break;
	        case SMAX: case SERROR:
			throw Error(parser_error(state));
		case SEND:
			return;
		}

		if (!rec && must_recurse(tokenizer.token.type)) {
			rec = true;
			stack.push();
			goto recurse;
		}
	}
}

}
