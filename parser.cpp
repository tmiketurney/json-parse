/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		parser.cpp : Parser class
 *
 */
 
#include <iostream>

#include "main.hpp"
#include "tokenize.hpp"
#include "parser.hpp"

using namespace std;

parser::parser() : tokenize()
{

}

parser::parser(istream *input) : tokenize(input)
{

}

parser::~parser()
{
	for (std::vector<Token>::iterator iter = SymbolStream.begin();
         iter != SymbolStream.end(); ++iter)
	{
		delete &iter;
	}
}

int parser::parse_input()
{
	Token token;

	return 0;
}

