// src/LexicalAnalyzer.cpp
#include "LexicalAnalyzer.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// 静态成员初始化
unordered_map<string, int> LexicalAnalysis::Keywords = {
    {"auto", 1}, {"break", 2}, {"case", 3}, {"char", 4},
    {"const", 5}, {"continue", 6}, {"default", 7}, {"do", 8},
    {"double", 9}, {"else", 10}, {"enum", 11}, {"extern", 12},
    {"float", 13}, {"for", 14}, {"goto", 15}, {"if", 16},
    {"int", 17}, {"long", 18}, {"register", 19}, {"return", 20},
    {"short", 21}, {"signed", 22}, {"sizeof", 23}, {"static", 24},
    {"struct", 25}, {"switch", 26}, {"typedef", 27}, {"union", 28},
    {"unsigned", 29}, {"void", 30}, {"volatile", 31}, {"while", 32}
};

unordered_map<string, int> LexicalAnalysis::Operators = {
    {"-", 33}, {"--", 34}, {"-=", 35}, {"->", 36},
    {"!", 37}, {"!=", 38}, {"%", 39}, {"%=", 40},
    {"&", 41}, {"&&", 42}, {"&=", 43}, {"(", 44},
    {")", 45}, {"*", 46}, {"*=", 47}, {",", 48},
    {".", 49}, {"/", 50}, {"/=", 51}, {":", 52},
    {";", 53}, {"?", 54}, {"[", 55}, {"]", 56},
    {"^", 57}, {"^=", 58}, {"{", 59}, {"|", 60},
    {"||", 61}, {"|=", 62}, {"}", 63}, {"~", 64},
    {"+", 65}, {"++", 66}, {"+=", 67}, {"<", 68},
    {"<<", 69}, {"<<=", 70}, {"<=", 71}, {"=", 72},
    {"==", 73}, {">", 74}, {">=", 75}, {">>", 76},
    {">>=", 77}, {"\"", 78}
};

vector<char> LexicalAnalysis::equal_label = {'=','!','<','>','+','-','*','/','%'};
vector<char> LexicalAnalysis::double_label = {'&','|','+','-','<','>','/','='};
vector<char> LexicalAnalysis::single_label = {
    '-', '!', '%', '&', '(', ')', '*', ',', '.', '/',
    ':', ';', '?', '[', ']', '^', '{', '|', '}', '~',
    '+', '<', '>', '=', '"'
};
vector<char> LexicalAnalysis::digits = {'1','2','3','4','5','6','7','8','9','0'};
vector<char> LexicalAnalysis::invalid_chars = {' ','\t','\n'};
vector<char> LexicalAnalysis::format_specifiers = {
    'd','i','o','u','x','X','f','F','e','E','g','G','a','A','c','s','p','n','%'
};

// 成员函数实现
template<typename T>
bool LexicalAnalysis::contains(const vector<T>& vec, const T& value) {
    return find(vec.begin(), vec.end(), value) != vec.end();
}

int LexicalAnalysis::analyze(istringstream& iss) {
    lexical_analysis.clear();
    string current;
    char c;
    int result = -1;
    int count = 0;

    while (iss.get(c)) {
        //consider Comment
        if (contains(equal_label, c)) { //detect multi-bit label
            if (!current.empty()) {
                result = identify(current);
                store_answer(current, result, count, lexical_analysis);
            }
            char next_c;
            char rest_c = '\0';
            current = c;
            iss.get(next_c);
            if (next_c == '=') {    //double_label with equal sign
                current += next_c;
            }
            else if (c=='/'&&next_c=='/') {
                current += next_c;
                while (iss.get(c)) {
                    if (c == '\n' || c == EOF) {
                        break;
                    }
                    current += c;
                }
            }
            else if (c == '/' && next_c == '*') {
                current += next_c;
                char first_end, second_end;
                iss.get(first_end);
                iss.get(second_end);
                while (first_end != '*' && second_end != '/') {
                    current += first_end;
                    first_end = second_end;
                    iss.get(second_end);
                }
                current += first_end;
                current += second_end;
            }
            // format specifier
            else if (c == '%' && contains(format_specifiers, next_c)) {
                current += next_c;
            }
            else if (contains(double_label, c) && next_c == c) {  //double_label without same sign
                current += next_c;
                if (c == '<' || c == '>') {    //triple_label with equal sign
                    char next_next_c;
                    iss.get(next_next_c);
                    if (next_next_c == '=')
                        current += next_next_c;
                    else if (!contains(invalid_chars, next_next_c))
                        rest_c = next_next_c;
                }
            }
            else {
                if(!contains(invalid_chars, next_c))
                    rest_c = next_c;
            }
            //it needn't to consider single label here
            store_answer(current, identify(current), count, lexical_analysis);
            if(rest_c != '\0' && !contains(invalid_chars, rest_c))
                current = rest_c;
        }
        else if (contains(invalid_chars, c)) {
            if (current.empty())
                continue;

            result = identify(current);
            store_answer(current, result, count, lexical_analysis);
        }
        else if (contains(single_label, c)) {  //it does not contains multi-bits label conditions
            if (!current.empty()) {
                result = identify(current);
                store_answer(current, result, count, lexical_analysis);
            }

            current += c;
            result = identify(current);
            store_answer(current, result, count, lexical_analysis);
        }
        else {
            current += c;
        }
    }
    return count;
}

int LexicalAnalysis::is_identifiers(const string& lexical) {
    auto keywordIt = Keywords.find(lexical);
    if (keywordIt != Keywords.end()) {
        return keywordIt->second;
    }
    return -1;
}

int LexicalAnalysis::is_operators(const string& lexical) {
    auto operatorIt = Operators.find(lexical);
    if (operatorIt != Operators.end()) {
        return operatorIt->second;
    }
    return -1;
}

bool LexicalAnalysis::is_constant(const string& lexical) {
    bool flag = false;
    int l = lexical.length();
    for (int i = 0; i < l; ++i) {
        if (!contains(digits, lexical[i])) {
            if (!flag && lexical[i] == '.') 
                flag = true;
            return false;
        }
    }
    return true;
}

int LexicalAnalysis::identify(const string& current) {
    if (current.substr(0, 2) == "//" || current.substr(0, 2) == "/*")
        return 79;
    if (current[0] == '%' && contains(format_specifiers, current[1]))
        return 81;
    
    int identifiers = is_identifiers(current);
    if (identifiers > 0)
        return identifiers;
    
    int operators = is_operators(current);
    if (operators > 0)
        return operators;
    
    if (is_constant(current))
        return 80;
    
    return 81;
}

std::vector<Token> LexicalAnalysis::getTokens() const {
    return lexical_analysis;
}

void LexicalAnalysis::clear() {
    lexical_analysis.clear();
}

// 辅助函数实现
void store_answer(string& lexical, int result, int& count, vector<Token>& analysis) {
    analysis.emplace_back(lexical, result);
    count++;
    lexical.clear();
}

void read_prog(string& prog) {
    char c;
    while (scanf("%c", &c) != EOF) {
        prog += c;
    }
}