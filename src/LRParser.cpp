// LRParser.cpp
#include "LRParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

// 辅助方法实现
string SLRParser::symbolToString(int symbol) {
    switch (symbol) {
    case TOK_LBRACE: return "{";
    case TOK_RBRACE: return "}";
    case TOK_IF: return "if";
    case TOK_LEFT_PAREN: return "(";
    case TOK_RIGHT_PAREN: return ")";
    case TOK_THEN: return "then";
    case TOK_ELSE: return "else";
    case TOK_WHILE: return "while";
    case TOK_ID: return "ID";
    case TOK_EQUAL: return "=";
    case TOK_SEMICOLON: return ";";
    case TOK_LT: return "<";
    case TOK_GT: return ">";
    case TOK_LE: return "<=";
    case TOK_GE: return ">=";
    case TOK_EQ: return "==";
    case TOK_PLUS: return "+";
    case TOK_MINUS: return "-";
    case TOK_MUL: return "*";
    case TOK_DIV: return "/";
    case TOK_NUM: return "NUM";
    case TOK_END: return "$";
    case NT_START: return "S'";
    case NT_PROGRAM: return "program";
    case NT_STMT: return "stmt";
    case NT_COMPOUNDSTMT: return "compoundstmt";
    case NT_STMTS: return "stmts";
    case NT_IFSTMT: return "ifstmt";
    case NT_WHILESTMT: return "whilestmt";
    case NT_ASSGSTMT: return "assgstmt";
    case NT_BOOLEXPR: return "boolexpr";
    case NT_BOOLOP: return "boolop";
    case NT_ARITHEXPR: return "arithexpr";
    case NT_ARITHEXPRPRIME: return "arithexprprime";
    case NT_MULTEXPR: return "multexpr";
    case NT_MULTEXPRPRIME: return "multexprprime";
    case NT_SIMPLEEXPR: return "simpleexpr";
    default: return "?";
    }
}

bool SLRParser::isTerminal(int symbol) {
    return symbol >= TOK_LBRACE && symbol <= TOK_END;
}

bool SLRParser::isEpsilonProduction(const Production& prod) {
    return prod.rhs.empty();
}

// 词法分析实现
vector<pair<int, int>> SLRParser::tokenize(const string& prog) {
    vector<pair<int, int>> tokens;
    istringstream iss(prog);
    string line;
    int lineNum = 1;

    while (getline(iss, line)) {
        if (line.empty()) continue;

        istringstream lineStream(line);
        string token;

        while (lineStream >> token) {
            int tokenType = getTokenType(token);
            if (tokenType != -1) {
                tokens.push_back({ tokenType, lineNum });
            }
        }

        lineNum++;
    }

    return tokens;
}

int SLRParser::getTokenType(const string& token) {
    if (token == "{") return TOK_LBRACE;
    if (token == "}") return TOK_RBRACE;
    if (token == "if") return TOK_IF;
    if (token == "(") return TOK_LEFT_PAREN;
    if (token == ")") return TOK_RIGHT_PAREN;
    if (token == "then") return TOK_THEN;
    if (token == "else") return TOK_ELSE;
    if (token == "while") return TOK_WHILE;
    if (token == "ID") return TOK_ID;
    if (token == "=") return TOK_EQUAL;
    if (token == ";") return TOK_SEMICOLON;
    if (token == "<") return TOK_LT;
    if (token == ">") return TOK_GT;
    if (token == "<=") return TOK_LE;
    if (token == ">=") return TOK_GE;
    if (token == "==") return TOK_EQ;
    if (token == "+") return TOK_PLUS;
    if (token == "-") return TOK_MINUS;
    if (token == "*") return TOK_MUL;
    if (token == "/") return TOK_DIV;
    if (token == "NUM") return TOK_NUM;
    return -1;
}

// 文法初始化实现
void SLRParser::initializeProductions() {
    productions.push_back(Production(NT_START, { NT_PROGRAM }));
    productions.push_back(Production(NT_PROGRAM, { NT_COMPOUNDSTMT }));
    productions.push_back(Production(NT_STMT, { NT_IFSTMT }));
    productions.push_back(Production(NT_STMT, { NT_WHILESTMT }));
    productions.push_back(Production(NT_STMT, { NT_ASSGSTMT }));
    productions.push_back(Production(NT_STMT, { NT_COMPOUNDSTMT }));
    productions.push_back(Production(NT_COMPOUNDSTMT, { TOK_LBRACE, NT_STMTS, TOK_RBRACE }));
    productions.push_back(Production(NT_STMTS, { NT_STMT, NT_STMTS }));
    productions.push_back(Production(NT_STMTS, {}));
    productions.push_back(Production(NT_IFSTMT, { TOK_IF, TOK_LEFT_PAREN, NT_BOOLEXPR, TOK_RIGHT_PAREN, TOK_THEN, NT_STMT, TOK_ELSE, NT_STMT }));
    productions.push_back(Production(NT_WHILESTMT, { TOK_WHILE, TOK_LEFT_PAREN, NT_BOOLEXPR, TOK_RIGHT_PAREN, NT_STMT }));
    productions.push_back(Production(NT_ASSGSTMT, { TOK_ID, TOK_EQUAL, NT_ARITHEXPR, TOK_SEMICOLON }));
    productions.push_back(Production(NT_BOOLEXPR, { NT_ARITHEXPR, NT_BOOLOP, NT_ARITHEXPR }));
    productions.push_back(Production(NT_BOOLOP, { TOK_LT }));
    productions.push_back(Production(NT_BOOLOP, { TOK_GT }));
    productions.push_back(Production(NT_BOOLOP, { TOK_LE }));
    productions.push_back(Production(NT_BOOLOP, { TOK_GE }));
    productions.push_back(Production(NT_BOOLOP, { TOK_EQ }));
    productions.push_back(Production(NT_ARITHEXPR, { NT_MULTEXPR, NT_ARITHEXPRPRIME }));
    productions.push_back(Production(NT_ARITHEXPRPRIME, { TOK_PLUS, NT_MULTEXPR, NT_ARITHEXPRPRIME }));
    productions.push_back(Production(NT_ARITHEXPRPRIME, { TOK_MINUS, NT_MULTEXPR, NT_ARITHEXPRPRIME }));
    productions.push_back(Production(NT_ARITHEXPRPRIME, {}));
    productions.push_back(Production(NT_MULTEXPR, { NT_SIMPLEEXPR, NT_MULTEXPRPRIME }));
    productions.push_back(Production(NT_MULTEXPRPRIME, { TOK_MUL, NT_SIMPLEEXPR, NT_MULTEXPRPRIME }));
    productions.push_back(Production(NT_MULTEXPRPRIME, { TOK_DIV, NT_SIMPLEEXPR, NT_MULTEXPRPRIME }));
    productions.push_back(Production(NT_MULTEXPRPRIME, {}));
    productions.push_back(Production(NT_SIMPLEEXPR, { TOK_ID }));
    productions.push_back(Production(NT_SIMPLEEXPR, { TOK_NUM }));
    productions.push_back(Production(NT_SIMPLEEXPR, { TOK_LEFT_PAREN, NT_ARITHEXPR, TOK_RIGHT_PAREN }));
}

// LR项集操作实现
bool SLRParser::areItemSetsEqual(const set<LR_item>& set1, const set<LR_item>& set2) {
    if (set1.size() != set2.size()) return false;

    auto it1 = set1.begin();
    auto it2 = set2.begin();

    while (it1 != set1.end() && it2 != set2.end()) {
        if (!(*it1 == *it2)) return false;
        ++it1;
        ++it2;
    }

    return true;
}

set<LR_item> SLRParser::closure(const set<LR_item>& items) {
    set<LR_item> closureSet = items;
    bool changed;

    do {
        changed = false;
        set<LR_item> newItems = closureSet;

        for (const auto& item : closureSet) {
            const Production& prod = productions[item.productionId];

            if (item.dotPos < prod.rhs.size()) {
                int nextSymbol = prod.rhs[item.dotPos];

                if (!isTerminal(nextSymbol)) {
                    for (int i = 0; i < (int)productions.size(); i++) {
                        if (productions[i].lhs == nextSymbol) {
                            LR_item newItem(i, 0);
                            if (newItems.find(newItem) == newItems.end()) {
                                newItems.insert(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }

        if (changed) {
            closureSet = newItems;
        }
    } while (changed);

    return closureSet;
}

set<LR_item> SLRParser::gotoFunction(const set<LR_item>& items, int symbol) {
    set<LR_item> newItems;

    for (const auto& item : items) {
        const Production& prod = productions[item.productionId];

        if (item.dotPos < prod.rhs.size() && prod.rhs[item.dotPos] == symbol) {
            newItems.insert(LR_item(item.productionId, item.dotPos + 1));
        }
    }

    return closure(newItems);
}

// LR(0)项集构造实现
void SLRParser::constructLR0ItemSets() {
    set<LR_item> initial;
    initial.insert(LR_item(0, 0));
    itemSets.push_back(closure(initial));

    for (size_t i = 0; i < itemSets.size(); i++) {
        for (int sym = TOK_LBRACE; sym <= NT_SIMPLEEXPR; sym++) {
            set<LR_item> gotoSet = gotoFunction(itemSets[i], sym);

            if (!gotoSet.empty()) {
                bool found = false;
                int foundIndex = -1;

                for (size_t j = 0; j < itemSets.size(); j++) {
                    if (areItemSetsEqual(itemSets[j], gotoSet)) {
                        found = true;
                        foundIndex = j;
                        break;
                    }
                }

                if (!found) {
                    itemSets.push_back(gotoSet);
                    foundIndex = itemSets.size() - 1;
                }

                if (isTerminal(sym)) {
                    actionTable[{i, sym}] = "s" + to_string(foundIndex);
                }
                else {
                    gotoTable[{i, sym}] = foundIndex;
                }
            }
        }
    }

    stateCount = itemSets.size();
}

// First和Follow集计算实现
void SLRParser::calculateFirstSets() {
    for (int sym = TOK_LBRACE; sym <= TOK_END; sym++) {
        firstSets[sym].insert(sym);
    }

    bool changed;
    do {
        changed = false;

        for (size_t i = 1; i < productions.size(); i++) {
            const Production& prod = productions[i];

            if (isEpsilonProduction(prod)) {
                if (firstSets[prod.lhs].insert(-1).second) {
                    changed = true;
                }
                continue;
            }

            bool allHaveEpsilon = true;
            for (int sym : prod.rhs) {
                for (int firstSym : firstSets[sym]) {
                    if (firstSym != -1) {
                        if (firstSets[prod.lhs].insert(firstSym).second) {
                            changed = true;
                        }
                    }
                }

                if (firstSets[sym].find(-1) == firstSets[sym].end()) {
                    allHaveEpsilon = false;
                    break;
                }
            }

            if (allHaveEpsilon) {
                if (firstSets[prod.lhs].insert(-1).second) {
                    changed = true;
                }
            }
        }
    } while (changed);
}

void SLRParser::calculateFollowSets() {
    followSets[NT_START].insert(TOK_END);

    bool changed;
    do {
        changed = false;

        for (const auto& prod : productions) {
            for (size_t i = 0; i < prod.rhs.size(); i++) {
                int B = prod.rhs[i];
                if (!isTerminal(B)) {
                    if (i == prod.rhs.size() - 1) {
                        for (int sym : followSets[prod.lhs]) {
                            if (followSets[B].insert(sym).second) {
                                changed = true;
                            }
                        }
                    }
                    else {
                        int next = prod.rhs[i + 1];
                        bool allHaveEpsilon = true;

                        for (size_t j = i + 1; j < prod.rhs.size(); j++) {
                            int current = prod.rhs[j];

                            for (int firstSym : firstSets[current]) {
                                if (firstSym != -1) {
                                    if (followSets[B].insert(firstSym).second) {
                                        changed = true;
                                    }
                                }
                            }

                            if (firstSets[current].find(-1) == firstSets[current].end()) {
                                allHaveEpsilon = false;
                                break;
                            }
                        }

                        if (allHaveEpsilon) {
                            for (int sym : followSets[prod.lhs]) {
                                if (followSets[B].insert(sym).second) {
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

// SLR分析表构造实现
void SLRParser::constructParsingTable() {
    for (int i = 0; i < stateCount; i++) {
        for (const auto& item : itemSets[i]) {
            const Production& prod = productions[item.productionId];

            if (item.dotPos == prod.rhs.size()) {
                if (item.productionId == 0) {
                    actionTable[{i, TOK_END}] = "acc";
                }
                else {
                    for (int followSym : followSets[prod.lhs]) {
                        if (followSym == -1) continue;
                        actionTable[{i, followSym}] = "r" + to_string(item.productionId);
                    }
                }
            }
        }
    }
}

// 产生式转字符串实现
string SLRParser::productionToString(int prodId) {
    const Production& prod = productions[prodId];
    string result = symbolToString(prod.lhs) + " ->";

    if (prod.rhs.empty()) {
        result += "";
    }
    else {
        for (int sym : prod.rhs) {
            result += " " + symbolToString(sym);
        }
    }

    return result;
}

// 错误处理实现
bool SLRParser::handleError(int state, int token, int lineNum, 
                           vector<pair<int, int>>& tokens, 
                           size_t& tokenIndex, 
                           stack<int>& stateStack, 
                           stack<int>& symbolStack) {
    if (!insertedSemicolon && !hasError) {
        if (actionTable.find({ state, TOK_SEMICOLON }) != actionTable.end()) {
            if (tokenIndex < tokens.size()) {
                int nextToken = tokens[tokenIndex].first;
                if (nextToken == TOK_RBRACE || nextToken == TOK_ID ||
                    nextToken == TOK_IF || nextToken == TOK_WHILE) {
                    cout << "语法错误，第" << lineNum - 1 << "行，缺少\";\"" << endl;
                    errorLine = lineNum;
                    hasError = true;
                    insertedSemicolon = true;

                    tokens.insert(tokens.begin() + tokenIndex, make_pair(TOK_SEMICOLON, lineNum));
                    return true;
                }
            }
        }
    }

    errorCount++;
    errorLine = lineNum;
    hasError = true;

    vector<string> expectedSymbols;
    for (int sym = TOK_LBRACE; sym <= TOK_END; sym++) {
        if (actionTable.find({ state, sym }) != actionTable.end()) {
            expectedSymbols.push_back(symbolToString(sym));
        }
    }

    if (!expectedSymbols.empty()) {
        string expected;
        for (size_t i = 0; i < expectedSymbols.size(); i++) {
            if (i > 0) expected += "或";
            expected += expectedSymbols[i];
        }
        cout << "语法错误，第" << lineNum << "行，缺少\"" << expected << "\"" << endl;
    }
    else {
        cout << "语法错误，第" << lineNum << "行" << endl;
    }
    tokenIndex++;
    return false;
}

// 字符串清理实现
string SLRParser::cleanString(const string& str) {
    string result;
    bool lastWasSpace = false;

    for (char c : str) {
        if (c == ' ') {
            if (!lastWasSpace) {
                if (!result.empty()) {
                    result += c;
                }
                lastWasSpace = true;
            }
        }
        else {
            result += c;
            lastWasSpace = false;
        }
    }

    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    while (!result.empty() && result[0] == ' ') {
        result = result.substr(1);
    }

    return result;
}

// 解析动作处理实现
void SLRParser::processShiftAction(const string& action, 
                                  stack<int>& stateStack, 
                                  stack<int>& symbolStack, 
                                  int currentToken, 
                                  size_t& tokenIndex) {
    int nextState = stoi(action.substr(1));
    stateStack.push(nextState);
    symbolStack.push(currentToken);
    tokenIndex++;
}

void SLRParser::processReduceAction(const string& action, 
                                   stack<int>& stateStack, 
                                   stack<int>& symbolStack) {
    int prodId = stoi(action.substr(1));
    const Production& prod = productions[prodId];

    rightmostDerivation.push_back(productionToString(prodId));

    if (!prod.rhs.empty()) {
        for (size_t i = 0; i < prod.rhs.size(); i++) {
            stateStack.pop();
            symbolStack.pop();
        }
    }

    auto gotoIt = gotoTable.find({ stateStack.top(), prod.lhs });
    if (gotoIt != gotoTable.end()) {
        int newState = gotoIt->second;
        stateStack.push(newState);
        symbolStack.push(prod.lhs);
    }
}

// 打印最右推导实现
void SLRParser::printDerivation() {
    if (!rightmostDerivation.empty()) {
        string current = "program";
        cout << current;

        for (int i = rightmostDerivation.size() - 1; i >= 0; i--) {
            cout << " => " << endl;

            string prodStr = rightmostDerivation[i];
            size_t arrowPos = prodStr.find("->");
            string lhs = prodStr.substr(0, arrowPos);
            string rhs;

            if (arrowPos + 2 < prodStr.length()) {
                rhs = prodStr.substr(arrowPos + 2);
            }

            while (!lhs.empty() && lhs.back() == ' ') lhs.pop_back();
            while (!rhs.empty() && rhs[0] == ' ') rhs = rhs.substr(1);

            size_t pos = current.rfind(lhs);
            if (pos != string::npos) {
                if (rhs.empty()) {
                    current.erase(pos, lhs.length());
                }
                else {
                    current.replace(pos, lhs.length(), rhs);
                }
            }

            current = cleanString(current);
            cout << current;
        }
        cout << endl;
    }
}

// 构造函数实现
SLRParser::SLRParser() : stateCount(0), errorCount(0), hasError(false), errorLine(0), insertedSemicolon(false) {
    initializeProductions();
    calculateFirstSets();
    calculateFollowSets();
    constructLR0ItemSets();
    constructParsingTable();
}

// 解析函数实现
bool SLRParser::parse(const string& prog) {
    vector<pair<int, int>> tokens = tokenize(prog);
    if (tokens.empty()) {
        return false;
    }

    tokens.push_back({ TOK_END, tokens.back().second });
    rightmostDerivation.clear();
    hasError = false;
    insertedSemicolon = false;
    errorLine = 0;

    stack<int> stateStack;
    stack<int> symbolStack;

    stateStack.push(0);
    symbolStack.push(TOK_END);

    size_t tokenIndex = 0;
    bool success = false;
    int maxSteps = 1000;
    int steps = 0;

    while (!success && steps < maxSteps) {
        steps++;
        int currentState = stateStack.top();
        int currentToken = tokens[tokenIndex].first;
        int lineNum = tokens[tokenIndex].second;

        auto actionIt = actionTable.find({ currentState, currentToken });

        if (actionIt == actionTable.end()) {
            if (handleError(currentState, currentToken, lineNum, tokens, tokenIndex, stateStack, symbolStack)) {
                continue;
            }

            if (tokenIndex >= tokens.size()) {
                break;
            }
            continue;
        }

        string action = actionIt->second;

        if (action[0] == 's') {
            processShiftAction(action, stateStack, symbolStack, currentToken, tokenIndex);
        }
        else if (action[0] == 'r') {
            processReduceAction(action, stateStack, symbolStack);
        }
        else if (action == "acc") {
            success = true;
            break;
        }
    }

    printDerivation();
    return success && !hasError;
}