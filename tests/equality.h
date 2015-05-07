#ifndef JSONCC_TESTS_EQUALITY_H
#define JSONCC_TESTS_EQUALITY_H

namespace Json {

struct Null;
struct True;
struct False;
class Number;
class String;
class Array;
class Member;
class Object;
class Value;

bool operator==(Json::Null const&, Json::Null const&);
bool operator==(Json::True const&, Json::True const&);
bool operator==(Json::False const&, Json::False const&);
bool operator==(Json::Number const&, Json::Number const&);
bool operator==(Json::String const&, Json::String const&);
bool operator==(Json::Array const&, Json::Array const&);
bool operator==(Json::Member const&, Json::Member const&);
bool operator==(Json::Object const&, Json::Object const&);
bool operator==(Json::Value const&, Json::Value const&);

}

#endif
