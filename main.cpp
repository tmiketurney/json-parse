/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		main.cpp (program driver)
 *
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "main.hpp"
#include "parser.hpp"
#include "tokenize.hpp"

using namespace std;

//#define DEBUG_ARGUMENTS			// enable to dump command-line arguments

int version_major = 0;
int version_minor = 2;

int debug_level;

ifstream infile;
string file_name("cin");


void usage(vector<string> arguments)
{
	vector<string>::iterator iter = arguments.begin();
	cerr << "Usage: " << iter->c_str() << " [-d <num> | -f <file> | -v]\n";
	cerr << "\t-d sets debug level (higher is more verbose output)\n";
	cerr << "\t-f specifies file with JSON to parse, default stdin\n";
	cerr << "\t-v requests version info\n";
	cerr << "\t-? display this usage info\n";
	cerr << "\tVersion[" << version_major << "." << version_minor << "]\n";
	exit(0);
}


vector<string> command_line(int argc, char *argv[])
{
	vector<string> arguments;
	for (int i=0; i<argc; i++)
		arguments.push_back(argv[i]);
	return arguments;
}


void parse_cl(vector<string> arguments)
{
	int i=0;

	for (std::vector<string>::iterator iter = arguments.begin();
         iter != arguments.end(); ++iter)
	{
		string arg = *iter;

#ifdef DEBUG_ARGUMENTS
		cerr << "[" << i++ << "]:" << arg << "\n";
#endif

		const char *cstr = iter->c_str();
		if (cstr[0] == '-')
		{
			switch (cstr[1])
			{
				case 'd':
					if (++iter == arguments.end())
						break;
					debug_level = atoi(iter->c_str());
					break;

				case 'f':
					if (++iter == arguments.end())
						break;
					file_name.clear();
					file_name = iter->c_str();
					break;

				case 'v':
					cerr << "Version: " << version_major << "." << version_minor << "\n";
					exit(0);
					break;

				case '?':
					usage(arguments);
					break;
			}

		}
	}

}


void dumpSymbolStream(parser *pjson)
{
	for (std::vector<Token>::iterator iter = pjson->SymbolStream.begin();
         iter != pjson->SymbolStream.end(); ++iter)
	{
		Token token = *iter;
		switch (token.tokenliteral)
		{
			case TokenLiteral::tEof:
				cerr << "EOF\n";
				break;
			case TokenLiteral::tString:
				cerr << token.string_value;
				break;
			case TokenLiteral::tNumber:
				cerr << token.number_value;
				break;
			case TokenLiteral::tTrue:
				cerr << "true";
				break;
			case TokenLiteral::tFalse:
				cerr << "false";
				break;
			case TokenLiteral::tNull:
				cerr << "null";
				break;
			case TokenLiteral::tLCurly:
				cerr << "\n{ ";
				break;
			case TokenLiteral::tRCurly:
				cerr << " }\n";
				break;
			case TokenLiteral::tLBracket:
				cerr << "\n[ ";
				break;
			case TokenLiteral::tRBracket:
				cerr << " ]\n";
				break;
			case TokenLiteral::tColon:
				cerr << " : ";
				break;
			case TokenLiteral::tComma:
				cerr << " , ";
				break;
			default:
				cerr << "\nUNKNOW\n";
				break;
		}
	}
}


int main(int argc, char *argv[])
{
	// parse command-line argumets
	vector<string> arguments = command_line(argc, argv);
	parse_cl(arguments);

#ifdef DEBUG_ARGUMENTS
	cerr << "debug_level[" << debug_level << "]\n";
	cerr << "file_name[" << file_name << "]\n";
	cerr << "Version[" << version_major << "." << version_minor << "]\n";
#endif

	// Initialize parse engine with correct file
	parser *json_parser;
	if (file_name.compare("cin") == 0)
	{
		// reading input from cin
		json_parser = new parser(&cin);
	}
	else
	{
		// reading input from named file
		infile.open(file_name.c_str());
		if (!infile)
		{
			cerr << "error: unable open input file[" << file_name << "]\n";
			exit(1);
		}

		json_parser = new parser(&infile);
	}

	// Parse the input
	int success = json_parser->parse_input();

	if (success)
	{
		cerr << "Success!! Valid JSON string parsed.\n";
	}
	else
	{
		cerr << "Failure!! Invalid JSON string found during parsing.\n";
	}

	if (debug_level >= GENERIC_DEBUG)
	{
		dumpSymbolStream(json_parser);
	}

	// cleanup and exit
	delete json_parser;
	if (file_name.compare("cin") != 0)
		infile.close();
	exit(0);
}
