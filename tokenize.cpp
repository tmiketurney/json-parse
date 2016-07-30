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

#include "tokenize.hpp"

using namespace std;

tokenize::tokenize()
{

}

tokenize::tokenize(istream *input)
{
	m_input = input;
}

tokenize::~tokenize()
{

}

TokenLiteral tokenize::read()
{

}
