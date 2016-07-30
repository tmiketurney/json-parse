/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		tokenize.cpp : Token class
 *
 */

#include <iostream>

#include "tokenize.hpp"

using namespace std;

tokenize::tokenize()
{
	m_current.tokenliteral = TokenLiteral::tEof;
}

tokenize::tokenize(istream *input)
{
	m_input = input;
	m_current.tokenliteral = TokenLiteral::tEof;
}

tokenize::~tokenize()
{

}

bool tokenize::handle_digits()
{
	return true;
}

bool tokenize::handle_strings()
{
	return true;
}

bool tokenize::check_name(string name)
{
	return true;
}

TokenLiteral tokenize::read()
{
	char ch = 0;
	bool success = true;

	*m_input >> ch;

	switch (ch) {

		// first group are single character reads
		case 0:
			m_current.tokenliteral = TokenLiteral::tEof;
			break;
		case '{':
			m_current.tokenliteral = TokenLiteral::tLCurly;
			break;
		case '}':
			m_current.tokenliteral = TokenLiteral::tRCurly;
			break;
		case '[':
			m_current.tokenliteral = TokenLiteral::tLBracket;
			break;
		case ']':
			m_current.tokenliteral = TokenLiteral::tRBracket;
			break;
		case ':':
			m_current.tokenliteral = TokenLiteral::tColon;
			break;

		// second group are numbers
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			m_input->putback(ch);
			success = handle_digits();
			break;

		// third group are strings
		case '"':
			m_input->putback(ch);
			success = handle_strings();
			break;

		// fourth group are name constants
		case 'f':
			m_input->putback(ch);
			success = check_name("false");
			break;
		case 't':
			m_input->putback(ch);
			success = check_name("true");
			break;
		case 'n':
			m_input->putback(ch);
			success = check_name("null");
			break;

		// last group is default/error
		default:
			cerr << "Error: bad start of token value[" << ch << "]\n";
			return TokenLiteral::tBadToken;
	}

	if (success)
		return m_current.tokenliteral;
	else
	{
		cerr << "Not Successful reading token\n";
		return TokenLiteral::tBadToken;
	}
}
