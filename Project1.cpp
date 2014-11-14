//////////////////////////////////////////
/*
 *
 * Aaron Robinson
 * 10/5/14
 * CS 460
 *
 * Project1.cpp
 *
 */
//////////////////////////////////////////

#include "LexicalAnalyzer.hpp"
#include "SetLimits.h"

using namespace std;

int main (int argc, char * argv[])
{
    SetLimits();

    if (argc < 2)
	{
		cout << "format: Project1.cpp <filename>" << endl;
		exit (1);
	}

	LexicalAnalyzer lex( argv[1] );
	lex.print();

	return 0;
}
