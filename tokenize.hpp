/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		tokenize.hpp : Tokenize class
 *
 */
 
#ifndef _TOKENIZE_HPP
#define _TOKENIZE_HPP

#include <istream>

using namespace std;

enum class TokenLiteral : char {
	tEof,
	tString,
	tNumber,
	tTrue,
	tFalse,
	tNull,
	tBadToken,
	tLCurly='{',
	tRCurly='}',
	tLBracket='[',
	tRBracket=']',
	tColon=':',
	tComma=','
};

struct Token {
	TokenLiteral    tokenliteral;
	string          string_value;
};

class tokenize {

	private:
		istream *m_input;
		Token m_current;
		char  m_digit;

		bool handle_strings();
		bool check_name(string name);

	public:
		tokenize();
		tokenize(istream *input);
		~tokenize();

		TokenLiteral read();
		bool handle_digits();
		Token get_current() { return m_current; }
		char  get_digit()   { return m_digit; }
};
 
#endif		// _TOKENIZE_HPP
 
