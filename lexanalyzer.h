#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include "interface.h"


using namespace std;

class Lexical_Analyzer
{

    friend class Interpreter;
    friend class Interface;


public:

    enum class categoryType
    {
        KEYWORD,
        IDENTIFIER,
        STRING_LITERAL,
        NUMERIC_LITERAL,
        ASSIGNMENT_OP,
        ARITH_OP,
        LOGICAL_OP,
        RELATIONAL_OP,
        LEFT_PAREN,
        RIGHT_PAREN,
        COLON,
        COMMA,
        COMMENT,
        INDENT,
        UNKNOWN
    };

    friend ostream& operator<<(ostream& os, categoryType cat)
    {
        if (cat == categoryType::KEYWORD)
            os << "KEYWORD";
        else if (cat == categoryType::IDENTIFIER)
            os << "IDENTIFIER";
        else if (cat == categoryType::STRING_LITERAL)
            os << "STRING LITERAL";
        else if (cat == categoryType::NUMERIC_LITERAL)
            os << "NUMERIC LITERAL";
        else if (cat == categoryType::ASSIGNMENT_OP)
            os << "ASSIGNMENT_OP";
        else if (cat == categoryType::ARITH_OP)
            os << "ARITH_OP";
        else if (cat == categoryType::LOGICAL_OP)
            os << "LOGICAL_OP";
        else if (cat == categoryType::RELATIONAL_OP)
            os << "RELATIONAL_OP";
        else if (cat == categoryType::LEFT_PAREN)
            os << "LEFT_PAREN";
        else if (cat == categoryType::RIGHT_PAREN)
            os << "RIGHT_PAREN";
        else if (cat == categoryType::COLON)
            os << "COLON";
        else if (cat == categoryType::COMMA)
            os << "COMMA";
        else if (cat == categoryType::COMMENT)
            os << "COMMENT";
        else if (cat == categoryType::INDENT)
            os << "INDENT";
        else if (cat == categoryType::UNKNOWN)
            os << "UNKNOWN";

        return os;
    }


    void startLexAnalysis(string fileLine, int lineCount)
    {
        vector<pair<string, categoryType>> v2; //empty vector to add to tokenInfo
        pair<string, categoryType> dual; //pairs to be added to tokenInfo[i]

        tokenInfo.push_back(v2); //creates an empty vector of tokenLineType

        for (int i = 0; i < fileLine.length(); i++)
        {
            string temp = "";
            if (fileLine[i] == ' ' || fileLine[i] == '\t')
            {
                if (i == 0)
                {
                    while (fileLine[i] == ' ' || fileLine[i] == '\t')
                    {
                        temp = temp + fileLine[i];
                        i++;
                    }
                    dual.first = temp;
                    dual.second = categoryType::INDENT;
                    tokenInfo[lineCount].push_back(dual);
                    i--;
                }
                else
                    continue;
            }
            else if (isdigit(fileLine[i])) //if digit char
            {
                while (isdigit(fileLine[i])) //gets all digits
                {
                    temp = temp + fileLine[i];
                    i++;
                }
                dual.second = categoryType::NUMERIC_LITERAL; //adds token
                dual.first = temp;
                tokenInfo[lineCount].push_back(dual);
                i--;
            }
            else if (isalpha(fileLine[i])) //if alphabet character
            {
                while (isalpha(fileLine[i])) //takes all alphabet characters until no longer true
                {
                    temp = temp + fileLine[i];
                    i++;
                }

                if (temp == "print" || temp == "elif" || temp == "if" || temp == "else" || temp == "while" || temp == "int" || temp == "input") //if keyword
                    dual.second = categoryType::KEYWORD;
                else if (temp == "and" || temp == "or" || temp == "not") //if logical op
                    dual.second = categoryType::LOGICAL_OP;
                else //if neither of above -> identifier
                    dual.second = categoryType::IDENTIFIER;

                dual.first = temp;
                tokenInfo[lineCount].push_back(dual);
                i--;
            }
            else if (fileLine[i] == '(') //if left parenthesis
            {
                dual.first = fileLine[i];
                dual.second = categoryType::LEFT_PAREN;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == ')') //if right parenthesis
            {
                dual.first = fileLine[i];
                dual.second = categoryType::RIGHT_PAREN;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == ':') //if colon
            {
                dual.first = fileLine[i];
                dual.second = categoryType::COLON;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == ',') //if comma
            {
                dual.first = fileLine[i];
                dual.second = categoryType::COMMA;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '>' || fileLine[i] == '<') //if relational op
            {
                temp = fileLine[i];

                if (fileLine[i + 1] == '=')//if next char also equal then relational op
                {
                    temp = temp + fileLine[i + 1];
                    dual.first = temp;
                    dual.second = categoryType::RELATIONAL_OP;
                    tokenInfo[lineCount].push_back(dual);
                    i++;
                    continue;
                }
                else //if not then assignment op
                    dual.first = fileLine[i];

                dual.second = categoryType::RELATIONAL_OP;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '=') //if equal
            {
                temp = fileLine[i];

                if (fileLine[i + 1] == '=')//if next char also equal then relational op
                {
                    temp = temp + fileLine[i + 1];
                    dual.first = temp;
                    dual.second = categoryType::RELATIONAL_OP;
                    tokenInfo[lineCount].push_back(dual);
                    i++;
                    continue;
                }
                else //if not then assignment op
                    dual.first = fileLine[i];

                dual.second = categoryType::ASSIGNMENT_OP;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '!') //if not
            {
                temp = fileLine[i];

                if (fileLine[i + 1] == '=')//if next char also equal then relational op
                {
                    temp = temp + fileLine[i + 1];
                    dual.first = temp;
                    dual.second = categoryType::RELATIONAL_OP;
                    tokenInfo[lineCount].push_back(dual);
                    i++;
                    continue;
                }
                else //if not then assignment op
                    dual.first = fileLine[i];

                dual.second = categoryType::UNKNOWN;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '+' || fileLine[i] == '-' || fileLine[i] == '/' || fileLine[i] == '*' || fileLine[i] == '%')
            { //if arithmetic op
                dual.first = fileLine[i];
                dual.second = categoryType::ARITH_OP;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '#') //if comment start
            {
                for (i; i < fileLine.size(); i++)
                {
                    temp = temp + fileLine[i];
                }
                dual.first = temp;
                dual.second = categoryType::COMMENT;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '\'') //if single quote
            {
                temp = fileLine[i];
                i++;
                while (fileLine[i] != '\'' && i < fileLine.size())
                {
                    temp = temp + fileLine[i];
                    i++;
                }
                temp = temp + '\'';
                dual.first = temp;
                dual.second = categoryType::STRING_LITERAL;
                tokenInfo[lineCount].push_back(dual);
            }
            else if (fileLine[i] == '\"') //if double quote
            {
                temp = fileLine[i];
                i++;
                while (fileLine[i] != '\"' && i < fileLine.size())
                {
                    temp = temp + fileLine[i];
                    i++;
                }
                temp = temp + '\"';
                dual.first = temp;
                dual.second = categoryType::STRING_LITERAL;
                tokenInfo[lineCount].push_back(dual);
            }
            else //if none of above cases, unknown
            {
                dual.first = fileLine[i];
                dual.second = categoryType::UNKNOWN;
                tokenInfo[lineCount].push_back(dual);
            }

        }

       
       //for (int i = 0; i < tokenInfo[0].size(); i++)
         //  cout << tokenInfo[0][i].first << "\t" << tokenInfo[0][i].second << endl;
    }

    vector<pair<string, categoryType>> sendData(int lineNumber)
    {
        vector<pair<string, categoryType>> tokenData;
        for (int i = 0; i < tokenInfo[lineNumber].size(); i++)
        {
            //cout << tokenInfo[lineNumber][i].first << "\t" << tokenInfo[lineNumber][i].second << endl;
            tokenData.push_back(tokenInfo[lineNumber][i]);
        }
        return tokenData;
    }

    void show()
    {
        if (!tokenInfo.size())
        {
            cout << "No token data available" << endl;
            return;
        }

        cout << "\n**********TOKEN INFORMATION**********" << endl;
        for (int i = 0; i < tokenInfo.size(); i++)
        {
            cout << "LINE #" << i << ":" << endl;
            for (int j = 0; j < tokenInfo[i].size(); j++)
            {
                cout << "TOKEN[" << j << "]:\t";
                cout << tokenInfo[i][j].first << "\t" << tokenInfo[i][j].second << endl;
            }
            cout << "-------------------------------------------------------------------" << endl;
        }
    }

    bool is_expression(string input)
    {
        tokenInfo.clear();
        startLexAnalysis(input, 0);


        for (int i = 0; i < tokenInfo[0].size(); i++)
        {
            //cout << tokenInfo[0][i].second << endl;
            if (tokenInfo[0][i].second != categoryType::INDENT)
                continue;
            if (tokenInfo[0][i].second != categoryType::RELATIONAL_OP ||
                tokenInfo[0][i].second != categoryType::LOGICAL_OP ||
                tokenInfo[0][i].second != categoryType::ARITH_OP ||
                tokenInfo[0][i].second != categoryType::NUMERIC_LITERAL ||
                tokenInfo[0][i].second != categoryType::LEFT_PAREN ||
                tokenInfo[0][i].second != categoryType::RIGHT_PAREN)
            {
                tokenInfo.clear();
                return false;
            }


        }
        tokenInfo.clear();
        return true;
    }

    void clear()
    {
        tokenInfo.clear();
    }

private:

    typedef vector<vector<pair<string, categoryType>>> tokenType;
    typedef vector<pair<string, categoryType>> tokenLineType;
    typedef pair<string, categoryType> pairType;
    tokenType tokenInfo;

};

#endif
