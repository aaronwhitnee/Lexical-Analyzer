//////////////////////////////////////////
/*
 *
 * Aaron Robinson
 * 10/5/14
 * CS 460
 *
 * lex.hpp
 *
 * Header file for the Lexical Analyzer.
 * Included in Project1.cpp
 *
 */
//////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <iomanip>

using namespace std;

enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
        DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
        SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
        DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
        XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDA, RPAREN,
        LPAREN, SEMI, QUEST, COLON, COMMA, EOFT};

string token_names[] = {"ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT",
        "DIV", "MOD", "EXP", "NOT", "AND", "OR", "XOR", "ASSIGN", "LT", "GT", "SHIFTL",
        "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSWQ", "MULTEQ",
        "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
        "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDA", "RPAREN",
        "LPAREN", "SEMI", "QUEST", "COLON", "COMMA", "EOFT"};

int state_table[5][4] = {
//   a  0  ; other
    {1, 1, 2, 3},
    {1, 1, 2, 4},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

vector <string> lexemes;
int current_lex = -1;
void add_lexeme (string lexeme)
{
    lexemes.push_back(lexeme);
}

enum token_type get_token (ifstream &infile);

int ln = 0;
int pos = 0;
int errors = 0;
enum token_type get_token (ifstream &infile)
{
    int cs = 0;
    string valid = "a0;";
    string lexeme = "";
    string line = "";
    if (pos >= line.length())
    {
        cout << "Getting a new line..." << endl;
        getline(infile, line);
        if (infile.eof())
        {
            cout << "reached EOF" << endl;
            return EOFT;
        }
        ln++;
        pos = 0;
        cout << ln << ": " << line << endl;
    }

    while(isspace(line[pos]))
        pos++;

    while(true)//!infile.fail() && !infile.eof())
    {
        // Consume next available character
        char c = line[pos];
        lexeme += c;
        cout << "consumed char: " << c << endl;
        cout << "curr lexeme: " << lexeme << endl;

        if(isalpha(c))
            c = 'a';
        if(isdigit(c))
            c = '0';

        int col = 0;
        while(c != valid[col] && col < 3)
            col++;
        cout << "state: " << cs;
        // Update state based on character just consumed
        cs = state_table[cs][col];
        cout << "  col: " << col << endl;
        cout << "line pos: " << pos << endl;

        switch(cs)
        {
            case 0:
                pos++;
                break;
            case 1:
                pos++;
                break;
            case 2:
                add_lexeme(lexeme);
                pos++;
                cs = 0;
                return SEMI;
            case 3:
                add_lexeme(lexeme);
                cout << "Error at " << ln << "," << pos << ": ";
                cout << "Invalid character found: " << lexeme << endl;
                pos++;
                cs = 0;
                errors++;
                return ERROR;
            case 4:
                lexeme[lexeme.length()-1] = '\0';
                add_lexeme(lexeme);
                pos++;
                cs = 0;
                return IDENT;
        }
    }
    //cout << "Shouldn't get here..." << endl;
    //return EOFT;
}

string get_lexeme ()
{
    cout << "Getting lexeme..." << endl;
    current_lex++;
    return lexemes[current_lex];
}

void error_message (char * msg);

//void end_lex ();
