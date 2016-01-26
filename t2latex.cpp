/*
author: Plinio Vargas
email: pvargas@cs.odu.edu
date: October 13, 2014

Program Name: t2latex.cpp
Title: Converter from text to Latex code
Version: 1.0

specification:
Given a series of symbols such as,
<, >, {, }, _, \t {tab}, \n {new line}, etc.
create a file converting those symbols to Latex
equivelant such as,
$<$, $>$, \{ ....

*/
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

#include "text2Latex.h"
int main ( int argc, char *argv[] )

{
    ifstream datafile;
    ofstream outfile;
    string entryname = "data.txt", out="out.txt", str;
    stack<char> A;
    stack<string> openStack;
    stack<string> unbalance;

    if ( argc > 3)  // verifies if input file was specified
    {
        cout << "usage: t2latex <inputfile> <outputfile>";
        return -1;
    }

    /* verifies argument from command line */
    if ( argc == 2)
    {
        entryname = argv[1]; //uses argument file from command line
    }
    if ( argc == 3 )
    {
        entryname = argv[1];
        out = argv[2];
    }

    datafile.open(entryname.c_str());
    outfile.open(out.c_str());

    if ( !datafile.is_open() )
    {
        cout << "Couldn't open file:" << entryname <<
        "\nPlease, verify file is in the correct directory...\n" << endl;
        return -1;
    }
    if ( !outfile.is_open() )
    {
        cout << "Couldn't write to file:" << out <<
        "\nPlease, verify file is in the correct directory...\n" << endl;
        return -1;
    }
    text2Latex line;
    while ( getline (datafile, str)  )
    {
        line.convert2Latex(str);
        outfile << line.getLineNumber() << "\t \\hspace*{5mm}" << line.getLine() << "\\\\" << endl;
    }

    cout << "\n\nLine count:" << line.getLineNumber() << endl;
    datafile.close();
    outfile.close();
    return 0;
}
