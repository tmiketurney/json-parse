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

#include "parser.hpp"

using namespace std;

int version_major = 0;
int version_minor = 1;

int debug_level;

ifstream infile;
string file_name("stdin");


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
		cerr << "[" << i++ << "]:" << arg << "\n";

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


int main(int argc, char *argv[])
{
	vector<string> arguments = command_line(argc, argv);
	parse_cl(arguments);
	
	cerr << "debug_level[" << debug_level << "]\n";
	cerr << "file_name[" << file_name << "]\n";
	cerr << "Version[" << version_major << "." << version_minor << "]\n";

}
