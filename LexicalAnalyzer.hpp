//////////////////////////////////////////
/*
 *
 * Aaron Robinson
 * 10/18/14
 * CS 460
 *
 * LexicalAnalyer.hpp
 *
 */
//////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <iomanip>
#include "table.h"

using namespace std;

#define DEBUG false
#define debug if(DEBUG) cout

enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
        DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
        SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
        DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
        XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDE, RPAREN,
        LPAREN, SEMI, QUEST, COLON, COMMA, EOFT};

string token_names[] = {"ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT",
        "DIV", "MOD", "EXP", "NOT", "AND", "OR", "XOR", "ASSIGN", "LT", "GT", "SHIFTL",
        "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ",
        "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
        "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDE", "RPAREN",
        "LPAREN", "SEMI", "QUEST", "COLON", "COMMA", "EOFT"};

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    LexicalAnalyzer( char* filename );
    enum token_type get_token( ifstream &infile );
    string get_lexeme();
    void print_error();
    void print();

private:
    ifstream input_file;
    enum token_type T;
    string valid;
    string lexeme;
    string line;
    int ln;
    int pos;
    int errors;
    int cs;
};

LexicalAnalyzer::LexicalAnalyzer()
{
}

LexicalAnalyzer::LexicalAnalyzer( char* filename )
{
    input_file.open( filename );
    if (!input_file)
    {
        cout << "Error: could not open \"" << filename << "\"" << endl;
        exit(1);
    }
    valid = "0a_.+=-/*%<>!&|^~()?;,:";
    lexeme = "";
    line = "";
    cs = 0;
    ln = 0;
    pos = 0;
    errors = 0;
}

enum token_type LexicalAnalyzer::get_token( ifstream &input_file )
{
    debug << "Inside get_token()," << endl;
    cs = 0;
    lexeme = "";
    if (pos >= line.length())
    {
        debug << "Getting a new line..." << endl;
        getline(input_file, line);
        if (input_file.eof())
        {
            return EOFT;
        }
        ln++;
        pos = 0;
        cout << "   " << ln << ": " << line << endl;
    }
    // Remove leading whitespace
    while(isspace(line[pos]))
        pos++;
    // Remove trailing whitespace
    while(line[line.size()-1] == ' ')
        line = line.substr(0, line.size()-1);

    // Loop through characters in line
    while( true )
    {
        // Consume next available character
        char c = line[pos];
        lexeme += c;
        debug << "consumed char: " << c << endl;
        debug << "curr lexeme: " << get_lexeme() << endl;

        if(isalpha(c) || c == '_')
            c = 'a';
        if(isdigit(c))
            c = '0';

        int col = 0;
        // Find the correct column in the state table
        while(c != valid[col] && col < 23)
            col++;
        debug << "state: " << cs << " -> ";
        // Update state based on character just consumed
        cs = state_table[cs][col];
        debug << cs << "  col: " << col << endl;
        debug << "line pos: " << pos << endl << endl;

        switch(cs)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                pos++;
                break;
            case 4:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                if (lexeme == "int") return INTTYPE;
                if (lexeme == "double") return DBLTYPE;
                return IDENT;
            case 5:
            case 6:
                pos++;
                break;
            case 7:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return NUMLIT;
            case 8:
                debug <<"CASE 8" << endl;
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                print_error();
                errors++;
                return ERROR;
            case 9:
                pos++;
                break;
            case 10:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return PLUS;
            case 11:
                pos++;
                cs = 0;
                return PLUSEQ;
            case 12:
                pos++;
                cs = 0;
                return PLUSPLUS;
            case 13:
                pos++;
                break;
            case 14:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return MINUS;
            case 15:
                pos++;
                cs = 0;
                return MINUSEQ;
            case 16:
                pos++;
                cs = 0;
                return MINUSMINUS;
            case 17:
                pos++;
                break;
            case 18:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return DIV;
            case 19:
                pos++;
                cs = 0;
                return DIVEQ;
            case 20:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return MULT;
            case 21:
                pos++;
                break;
            case 22:
                pos++;
                cs = 0;
                return MULTEQ;
            case 23:
                pos++;
                break;
            case 24:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return MOD;
            case 25:
                pos++;
                cs = 0;
                return MODEQ;
            case 26:
                pos++;
                break;
            case 27:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return ASSIGN;
            case 28:
                pos++;
                cs = 0;
                return EQUALTO;
            case 29:
                pos++;
                break;
            case 30:
                pos++;
                break;
            case 31:
                pos++;
                cs = 0;
                return LTE;
            case 32:
                pos++;
                cs = 0;
                return SHIFTLEQ;
            case 33:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return SHIFTL;
            case 34:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return LT;
            case 35:
                pos++;
                break;
            case 36:
                pos++;
                break;
            case 37:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return GT;
            case 38:
                pos++;
                cs = 0;
                return GTE;
            case 39:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return SHIFTR;
            case 40:
                pos++;
                cs = 0;
                return SHIFTREQ;
            case 41:
                pos++;
                break;
            case 42:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return NOT;
            case 43:
                pos++;
                cs = 0;
                return NOTEQ;
            case 44:
                pos++;
                break;
            case 45:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return AND;
            case 46:
                pos++;
                cs = 0;
                return LOGAND;
            case 47:
                pos++;
                cs = 0;
                return ANDEQ;
            case 48:
                pos++;
                break;
            case 49:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return OR;
            case 50:
                pos++;
                cs = 0;
                return LOGOR;
            case 51:
                pos++;
                cs = 0;
                return OREQ;
            case 52:
                pos++;
                break;
            case 53:
                cs = 0;
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return XOR;
            case 54:
                pos++;
                cs = 0;
                return XOREQ;
            case 55:
                pos++;
                cs = 0;
                return TILDE;
            case 56:
                pos++;
                cs = 0;
                return LPAREN;
            case 57:
                pos++;
                cs = 0;
                return RPAREN;
            case 58:
                pos++;
                cs = 0;
                return QUEST;
            case 59:
                pos++;
                cs = 0;
                return SEMI;
            case 60:
                pos++;
                cs = 0;
                return COMMA;
            case 61:
                pos++;
                cs = 0;
                return COLON;
            case 62:
                debug << "CASE 62" << endl;
                pos++;
                cs = 0;
                print_error();
                errors++;
                return ERROR;
        }
    }
}

string LexicalAnalyzer::get_lexeme()
{
    return lexeme;
}

void LexicalAnalyzer::print_error()
{
    cout << "Error at " << ln << "," << pos << ": ";
    cout << "Invalid character found: " << lexeme << endl;
}

void LexicalAnalyzer::print()
{
    while ((T = get_token(input_file)) != EOFT)
    {
        cout << "\t" << setw(16) << std::left << token_names[T] << get_lexeme() << endl;
    }
    if (errors > 0)
        cout << errors << " errors found in input file" << endl;
}
