#ifndef TEXT2LATEX_H_INCLUDED
#define TEXT2LATEX_H_INCLUDED

#define side2side 1
#define multiple  2
#define mixed     3
#define single    4
#define backslash 5
#define space     6

#include <map>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
using namespace std;

namespace patch
{
    template < typename T > string to_string( const T& n )
    {
        ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class text2Latex
{
    public:
        text2Latex();
        text2Latex(const string& myString);
        ~text2Latex(){}
        bool correctSymbol(const string& symbol );
        int getLineNumber();
        int hasErrors();
        int extracSymbols();
        string getLine();
        void convert2Latex( const string& myString);
    private:
        int line_number;
        int errors;
        queue<string> symbolQueue;
        string strLine;
        map<char, int> function;
};

text2Latex::text2Latex()
{
    line_number = 0;
    errors = 0;

    function[' '] = space;
    function['#'] = single;
    function['{'] = single;
    function['}'] = single;
    function['!'] = side2side;
    function['<'] = side2side;
    function['>'] = side2side;
    function['\t'] = multiple;
    function['\n'] = mixed;
    function['_'] = single;
    function['$'] = single;
    function['%'] = single;
    function['&'] = single;
    function['\\'] = backslash;
}
text2Latex::text2Latex(const string& str)
{
    strLine = str;
}

int text2Latex::getLineNumber()
{
    return line_number;
}

int text2Latex::hasErrors()
{
    return errors;
}

/* return 0 if there is not an error in the string,
otherwise returns the line number where the error occured*/
int text2Latex::extracSymbols()
{
    int k=0;
    int rep=0;
    int line_space=0;
    bool math_mode = false;

    while(k<strLine.size())
    {
        int oldKvalue = k;
        if ( math_mode && function[strLine[k]] != side2side &&
            function[strLine[k]] != backslash)
        {
            strLine.insert(k,"$");
            math_mode = false;
            k++;
        }

        /* character is a special character */
        switch ( function[strLine[k]] )
        {
        case single:
            strLine.insert(k,"\\");
            k++;
            break;
        case multiple:
            rep++;
            if ( function[strLine[k+1]] != multiple )
            {
                string horz_space = "\\hspace*{";
                horz_space.append(patch::to_string(rep*5));
                horz_space.append("mm}");
                strLine.insert(k+1,horz_space);
                k+=horz_space.size();
                rep=0;
            }
            break;
        case side2side:
            symbolQueue.push(strLine.substr(k,1));
            if ( function[strLine[k+1]] != side2side )
            {
                k = k - symbolQueue.size() + 1;
                strLine.insert(k,"$");
                math_mode = true;
                while ( !symbolQueue.empty())
                {
                    k++;
                    strLine[k] = symbolQueue.front()[0];
                    symbolQueue.pop();
                }
            }
            break;
        case backslash:
            if ( !math_mode )
            {
                strLine[k]= '$';
                math_mode = true;
                k++;
                strLine.insert(k," ");
            }
            strLine[k] = '\\';
            k++;
            strLine.insert(k,"backslash");
            k+=8;
            break;
        case mixed:
            strLine.insert(k,"\\\\");
            break;
        case space:
            line_space++;
            if ( line_space > 1 && function[strLine[k+1]] != space )
            {
                string horz_space = "\\hspace*{";
                horz_space.append(patch::to_string(line_space*1.25));
                horz_space.append("mm}");
                strLine.insert(k+1,horz_space);
                k+=horz_space.size();
                line_space=0;
            }
            else if ( line_space == 1 && function[strLine[k+1]] != space )
                line_space = 0;
            break;
        }
        k++; // inspect the next character in the string
    }
    if ( math_mode )
        strLine.append("$");
    cout << strLine << "\\\\" << endl;
    return 0;
}

string text2Latex::getLine()
{
    return strLine;
}

void text2Latex::convert2Latex(const string& str)
{
    strLine = str;
    extracSymbols();
    line_number++;
}
#endif // text2Latex_H_INCLUDED
