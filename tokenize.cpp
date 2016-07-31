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
#include <ctype.h>

#include "main.hpp"
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
	return false;
}

bool tokenize::handle_strings()
{
	// first character is " (which is already consumed)
	string word;
	char ch = 0;
	while (*m_input >> ch)
	{
		if (ch == '\\')
		{
			word += '\\';
			*m_input >> ch;
			switch (ch)
			{
				case '"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					word += ch;
					break;
				case 'u':
					word += 'u';
					char c[4];
					for (int i=0; i<4; i++)
					{
						*m_input >> c[i];
						if (!isxdigit(c[i]))
						{
							cerr << "Badly formed hex digit escape in string [" << c[i] << "]\n";
							return false;
						}
						else
							word += c[i];
					}
					break;
				default:
					cerr << "Badly formed escape in string [" << ch << "]\n";
					return false;
			}
		}
		else if (ch == '\"')
			break;
		else
			word += ch;
	}
	m_current.tokenliteral = TokenLiteral::tString;
	m_current.string_value = word;
	return true;
}

bool tokenize::check_name(string name)
{
	// first character matches start of known name
	string word;
	*m_input >> word;
	if (word.compare(name))
		return false;
	else
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
		case ',':
			m_current.tokenliteral = TokenLiteral::tComma;
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
			success = handle_strings();
			break;

		// fourth group are name constants
		case 'f':
			m_input->putback(ch);
			success = check_name("false");
			if (success)
				m_current.tokenliteral = TokenLiteral::tFalse;
			break;
		case 't':
			m_input->putback(ch);
			success = check_name("true");
			if (success)
				m_current.tokenliteral = TokenLiteral::tTrue;
			break;
		case 'n':
			m_input->putback(ch);
			success = check_name("null");
			if (success)
				m_current.tokenliteral = TokenLiteral::tNull;
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
