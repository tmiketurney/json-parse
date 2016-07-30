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
 
#include "tokenize.hpp"

using namespace std;

class parser : public tokenize {

	public:
		parser();
		parser(istream *m_input);
		~parser();

		int parse_input();
};
 
#endif		// _PARSER_HPP
 
