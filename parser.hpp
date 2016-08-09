/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		parser.hpp : Parser class
 *
 */
 
#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <iostream>
#include <vector>
 
#include "tokenize.hpp"

using namespace std;

// { string : value [,] }
enum class ObjectStates : char {
	osNeedString,
	osNeedColon,
	osNeedValue,
	osNeedEnd
};

// [ value [,] ]
enum class ArrayStates : char {
	asNeedValue,
	asNeedEnd
};

// [-] 0 | 1-9* [ . 0-9* ] [e|E] [+|-] [0-9*]
enum class NumberStates : char {
	ns0NeedMinusDigit,
	ns1NeedDigit,
	ns2NeedDigit,
	ns3NeedEDotDigit,
	ns4NeedDigit,
	ns5NeedPlusMinusDigit,
	ns6NeedDigit
};

class parser {

	private:
		tokenize m_token;
		bool parseObject();
		bool parseValue();
		bool parseArray();
		bool parseNumber();

	public:
		vector<Token> SymbolStream;

		parser();
		parser(istream *m_input);
		~parser();

		int parse_input();
		string sObjectStates(ObjectStates state);
		string sArrayStates(ArrayStates state);
		string sNumberStates(NumberStates state);
};
 
#endif		// _PARSER_HPP
 
