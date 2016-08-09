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
#include <fstream>

#include "main.hpp"
#include "tokenize.hpp"
#include "parser.hpp"

using namespace std;

parser::parser()
{
	// This should error out
}

parser::parser(istream *input) : m_token(input)
{

}

parser::~parser()
{

}


string parser::sObjectStates(ObjectStates state)
{
	string value;

	switch (state)
	{
		case ObjectStates::osNeedString:
			value = "osNeedString";
			break;

		case ObjectStates::osNeedColon:
			value = "osNeedColon";
			break;

		case ObjectStates::osNeedValue:
			value = "osNeedValue";
			break;

		case ObjectStates::osNeedEnd:
			value = "osNeedEnd";
			break;

		default:
			value = "UNKNOWN";
			break;
	}
	return value;
}


string parser::sArrayStates(ArrayStates state)
{
	string value;

	switch (state)
	{
		case ArrayStates::asNeedValue:
			value = "asNeedValue";
			break;

		case ArrayStates::asNeedEnd:
			value = "asNeedEnd";
			break;

		default:
			value = "UNKNOWN";
			break;
	}
	return value;
}


string parser::sNumberStates(NumberStates state)
{
	string value;

	switch (state)
	{
		case NumberStates::ns0NeedMinusDigit:
			value = "ns0NeedMinusDigit";
			break;

		case NumberStates::ns1NeedDigit:
			value = "ns1NeedDigit";
			break;

		case NumberStates::ns2NeedDigit:
			value = "ns2NeedDigit";
			break;

		case NumberStates::ns3NeedEDotDigit:
			value = "ns3NeedEDotDigit";
			break;

		case NumberStates::ns4NeedDigit:
			value = "ns4NeedDigit";
			break;

		case NumberStates::ns5NeedPlusMinusDigit:
			value = "ns5NeedPlusMinusDigit";
			break;

		case NumberStates::ns6NeedDigit:
			value = "ns6NeedDigit";
			break;

		default:
			value = "UNKNOWN";
			break;
	}
	return value;
}


bool parser::parseNumber()
{
	NumberStates state = NumberStates::ns0NeedMinusDigit;
	bool do_another = true;
	bool success = true;
	string numberValue;
	char single_digit;

	while (do_another)
	{
		if (debug_level >= TRACE)
		{
			cerr << "parseNumber state[" << sNumberStates(state) << "]\n";
		}

		if (!m_token.handle_digits())
			return false;

		single_digit = m_token.get_digit();
		switch (single_digit)
		{
			case '0':
				numberValue += single_digit;
				switch (state)
				{
					case NumberStates::ns0NeedMinusDigit:
						state = NumberStates::ns3NeedEDotDigit;
						break;
				}
				break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				numberValue += single_digit;
				switch (state)
				{
					case NumberStates::ns0NeedMinusDigit:
					case NumberStates::ns1NeedDigit:
						state = NumberStates::ns2NeedDigit;
						break;
					case NumberStates::ns3NeedEDotDigit:
						state = NumberStates::ns4NeedDigit;
						break;
					case NumberStates::ns5NeedPlusMinusDigit:
						state = NumberStates::ns6NeedDigit;
						break;
				}
				break;

			case '-':
				switch (state)
				{
					case NumberStates::ns0NeedMinusDigit:
						numberValue += single_digit;
						state = NumberStates::ns1NeedDigit;
						break;
					case NumberStates::ns5NeedPlusMinusDigit:
						numberValue += single_digit;
						state = NumberStates::ns6NeedDigit;
						break;
					default:
						do_another = false;
						success = false;
						break;
				}
				break;

			case '+':
				switch (state)
				{
					case NumberStates::ns5NeedPlusMinusDigit:
						numberValue += single_digit;
						state = NumberStates::ns6NeedDigit;
						break;
					default:
						do_another = false;
						success = false;
						break;
				}
				break;

			case '.':
				switch (state)
				{
					case NumberStates::ns3NeedEDotDigit:
						numberValue += single_digit;
						state = NumberStates::ns4NeedDigit;
						break;
					default:
						do_another = false;
						success = false;
						break;
				}
				break;

			case 'e':
			case 'E':
				switch (state)
				{
					case NumberStates::ns3NeedEDotDigit:
						numberValue += single_digit;
						state = NumberStates::ns5NeedPlusMinusDigit;
						break;
					default:
						do_another = false;
						success = false;
						break;
				}
				break;

			default:
				do_another = false;
				switch (state)
				{
					case NumberStates::ns0NeedMinusDigit:
					case NumberStates::ns1NeedDigit:
					case NumberStates::ns3NeedEDotDigit:
					case NumberStates::ns5NeedPlusMinusDigit:
						success = false;
						break;
				}
				break;
		}
	}

	if (success)
	{
		Token number = m_token.get_current();
		number.string_value = numberValue;
		SymbolStream.push_back(number);
	}
	return success;
}


bool parser::parseArray()
{
	ArrayStates state = ArrayStates::asNeedValue;
	bool do_another = true;
	bool success = true;

	while (do_another)
	{
		if (debug_level >= TRACE)
		{
			cerr << "parseArray state[" << sArrayStates(state) << "]\n";
		}
		switch(state)
		{
			case ArrayStates::asNeedValue:
				if (!parseValue())
				{
					do_another = false;
					success = false;
				}
				else
				{
					state = ArrayStates::asNeedEnd;
				}
				break;

			case ArrayStates::asNeedEnd:
				switch (m_token.read())
				{
					case TokenLiteral::tComma:
						SymbolStream.push_back(m_token.get_current());
						state = ArrayStates::asNeedValue;
						break;

					case TokenLiteral::tRBracket:
						SymbolStream.push_back(m_token.get_current());
						do_another = false;
						break;

					default:
						success = false;
						do_another = false;
						break;
				}
				break;

			default:
				success = false;
				do_another = false;
				break;
		}
	}
	return success;
}


bool parser::parseValue()
{
	bool success = true;

	if (debug_level >= TRACE)
	{
		cerr << "parseValue\n";
	}

	switch (m_token.read())
	{
		case TokenLiteral::tString:
			SymbolStream.push_back(m_token.get_current());
			break;

		case TokenLiteral::tNumber:
			success = parseNumber();
			break;

		case TokenLiteral::tLCurly:
			SymbolStream.push_back(m_token.get_current());
			success = parseObject();
			break;

		case TokenLiteral::tLBracket:
			SymbolStream.push_back(m_token.get_current());
			success = parseArray();
			break;

		case TokenLiteral::tTrue:
			SymbolStream.push_back(m_token.get_current());
			break;

		case TokenLiteral::tFalse:
			SymbolStream.push_back(m_token.get_current());
			break;

		case TokenLiteral::tNull:
			SymbolStream.push_back(m_token.get_current());
			break;

		default:
			success = false;
			break;
	}
	return success;
}


bool parser::parseObject()
{
	ObjectStates state = ObjectStates::osNeedString;
	bool do_another = true;
	bool success = true;

	while (do_another)
	{
		if (debug_level >= TRACE)
		{
			cerr << "parseObject state[" << sObjectStates(state) << "]\n";
		}

		switch (m_token.read())
		{
			case TokenLiteral::tString:
				if (state == ObjectStates::osNeedString)
				{
					SymbolStream.push_back(m_token.get_current());
					state = ObjectStates::osNeedColon;
				}
				else
				{
					if (debug_level >= GENERIC_DEBUG)
					{
						cerr << "parseObject found string while looking for [" << sObjectStates(state) << "]\n";
					}
					do_another = false;
					success = false;
				}
				break;

			case TokenLiteral::tColon:
				if (state == ObjectStates::osNeedColon)
				{
					SymbolStream.push_back(m_token.get_current());
					state = ObjectStates::osNeedValue;
					if (!parseValue())
					{
						do_another = false;
						success = false;
					}
					else
					{
						state = ObjectStates::osNeedEnd;
					}
				}
				else
				{
					if (debug_level >= GENERIC_DEBUG)
					{
						cerr << "parseObject found [:] while looking for [" << sObjectStates(state) << "]\n";
					}
					do_another = false;
					success = false;
				}
				break;

			case TokenLiteral::tRCurly:
				if (state == ObjectStates::osNeedEnd)
				{
					do_another = false;
					success = true;
					SymbolStream.push_back(m_token.get_current());
				}
				else
				{
					if (debug_level >= GENERIC_DEBUG)
					{
						cerr << "parseObject found [}] while looking for [" << sObjectStates(state) << "]\n";
					}
					do_another = false;
					success = false;
				}
				break;

			case TokenLiteral::tComma:
				if (state == ObjectStates::osNeedEnd)
				{
					SymbolStream.push_back(m_token.get_current());
					state = ObjectStates::osNeedString;
				}
				else
				{
					if (debug_level >= GENERIC_DEBUG)
					{
						cerr << "parseObject found [,] while looking for [" << sObjectStates(state) << "]\n";
					}
					do_another = false;
					success = false;
				}
				break;

			default:
				do_another = false;
				success = false;
				break;
		}
	}
	return success;
}


int parser::parse_input()
{
	bool do_another = true;
	bool success = true;

	while (do_another)
	{
		if (debug_level >= TRACE)
		{
			cerr << "parse_input\n";
		}

		switch (m_token.read())
		{
			case TokenLiteral::tEof:
				// End of File
				do_another = false;
				break;

			case TokenLiteral::tLCurly:
				// start of object
				SymbolStream.push_back(m_token.get_current());
				success = parseObject();
				break;

			default:
				do_another = false;
				success = false;
				break;
		}
	}

	return success;
}

