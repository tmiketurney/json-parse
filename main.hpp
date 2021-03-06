/*
 *	Design a class (or collection of classes) for parsing JSON.
 *	A high level design without full implementation is ok.
 *	The Json specification can be found here:  http://www.json.org/
 *
 *	Author:		T.Michael Turney
 *	Copyright	2016. All Rights Reserved.
 *	License:	GNU, GPLv2, See LICENSE file.
 *
 *	File:		main.hpp
 *
 */

#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <string>
#include <iostream>
#include <vector>

#include "tokenize.hpp"

extern int debug_level;
// Debug levels, set in debug_level
#define NO_DEBUG 0
#define GENERIC_DEBUG 1       // additional debug statements
#define TRACE 2               // every function traces

extern ifstream infile;
extern string file_name;

#endif	// _MAIN_HPP
