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

enum class ObjectStates : char {
	osNeedString,
	osNeedColon,
	osNeedValue,
	osNeedEnd
};

class parser : public tokenize {

	private:
		bool parseObject();
		bool parseValue();
		bool parseArray();

	public:
		vector<Token> SymbolStream;

		parser();
		parser(istream *m_input);
		~parser();

		int parse_input();
		string sObjectStates(ObjectStates state);
};
 
#endif		// _PARSER_HPP
 
