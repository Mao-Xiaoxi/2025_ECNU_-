// Semantic.cpp
#include "Semantic.h"
#include <iostream>

using namespace std;

// 构造函数
SemanticAnalyzer::SemanticAnalyzer() : posi(0), flag(false) {}

// 工具函数实现
void SemanticAnalyzer::error(const string& msg) {
    if (!flag) {
        errors.push_back("error message:line 1," + msg);
        flag = true;
    } else {
        errors.push_back("error message:line 5," + msg);
    }
}

string SemanticAnalyzer::peek() {
    return posi < tokens.size() ? tokens[posi] : "";
}

string SemanticAnalyzer::get() {
    return posi < tokens.size() ? tokens[posi++] : "";
}

bool SemanticAnalyzer::isID(const string& s) {
    return !s.empty() && islower(s[0]);
}

bool SemanticAnalyzer::isInt(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

bool SemanticAnalyzer::isReal(const string& s) {
    bool dot = false;
    for (char c : s) {
        if (c == '.') {
            if (dot) return false;
            dot = true;
        } else if (!isdigit(c)) return false;
    }
    return dot;
}

// 表达式函数实现
pair<string, double> SemanticAnalyzer::simpleexpr() {
    string t = peek();

    if (isID(t)) {
        get();
        return {symtab[t].type, symtab[t].value};
    }
    if (isInt(t)) {
        get();
        return {"int", stod(t)};
    }
    if (isReal(t)) {
        get();
        return {"real", stod(t)};
    }
    if (t == "(") {
        get();
        auto v = arithexpr();
        get(); // )
        return v;
    }
    error("invalid expression");
    return {};
}

pair<string, double> SemanticAnalyzer::multexpr() {
    auto l = simpleexpr();
    while (peek() == "*" || peek() == "/") {
        string op = get();
        auto r = simpleexpr();
        
        // 除零检查
        if (op == "/" && r.second == 0.0) {
            error("division by zero");
            // 继续解析但不计算
            continue;
        }
        
        if (l.first == "real" || r.first == "real") {
            l.first = "real";
            if (op == "*") {
                l.second = l.second * r.second;
            } else if (op == "/" && r.second != 0.0) {
                l.second = l.second / r.second;
            }
        } else {
            if (op == "*") {
                l.second = (int)l.second * (int)r.second;
            } else if (op == "/" && r.second != 0.0) {
                l.second = (int)l.second / (int)r.second;
            }
        }
    }
    return l;
}

pair<string, double> SemanticAnalyzer::arithexpr() {
    auto l = multexpr();
    while (peek() == "+" || peek() == "-") {
        string op = get();
        auto r = multexpr();
        if (l.first == "real" || r.first == "real") {
            l.first = "real";
            l.second = (op == "+") ? l.second + r.second : l.second - r.second;
        } else {
            l.second = (op == "+") ? (int)l.second + (int)r.second
                                   : (int)l.second - (int)r.second;
        }
    }
    return l;
}

bool SemanticAnalyzer::boolexpr() {
    auto l = arithexpr();
    string op = get();
    auto r = arithexpr();

    if (op == "<")  return l.second <  r.second;
    if (op == ">")  return l.second >  r.second;
    if (op == "<=") return l.second <= r.second;
    if (op == ">=") return l.second >= r.second;
    if (op == "==") return l.second == r.second;
    error("invalid boolop");
    return false;
}

// 语句函数实现
void SemanticAnalyzer::stmt(bool execute) {
    if (peek() == "if") ifstmt(execute);
    else if (peek() == "{") compoundstmt(execute);
    else assgstmt(execute);
}

void SemanticAnalyzer::compoundstmt(bool execute) {
    get(); // {
    while (peek() != "}") {
        stmt(execute);
    }
    get(); // }
}

void SemanticAnalyzer::assgstmt(bool execute) {
    string id = get();
    get(); // =
    auto v = arithexpr();
    get(); // ;

    if (execute) {
        if (symtab[id].type == "int" && v.first == "real")
            error("realnum can not be translated into int type");
        if (!errors.empty()) return; // 如果有错误，不更新值
        symtab[id].value = (symtab[id].type == "int") ? (int)v.second : v.second;
    }
}

void SemanticAnalyzer::ifstmt(bool execute) {
    get(); // if
    get(); // (
    bool cond = boolexpr();
    get(); // )
    get(); // then

    stmt(execute && cond);
    get(); // else
    stmt(execute && !cond);
}

// 声明函数实现
void SemanticAnalyzer::decls() {
    while (peek() == "int" || peek() == "real") {
        string type = get();
        string id = get();
        get(); // =
        string val = get();
        get(); // ;

        Var v;
        v.type = type;
        if (type == "int") {
            if (!isInt(val)) error("realnum can not be translated into int type");
            v.value = stoi(val);
        } else {
            v.value = stod(val);
        }
        symtab[id] = v;
    }
}

// 分析函数
void SemanticAnalyzer::analyze(const string& prog) {
    // 重置状态
    tokens.clear();
    symtab.clear();
    errors.clear();
    flag = false;
    posi = 0;
    
    // 词法分析
    stringstream ss(prog);
    string t;
    while (ss >> t) tokens.push_back(t);
    
    // 语义分析
    decls();
    compoundstmt(true);
}

// 打印结果
void SemanticAnalyzer::printResults() const {
    // 如果有错误，输出所有错误信息
    if (!errors.empty()) {
        for (size_t i = 0; i < errors.size(); i++) {
            cout << errors[i];
            if (i != errors.size() - 1) cout << endl;
        }
        return;
    }
    
    // 输出变量值
    cout << "a: " << (int)symtab.at("a").value << endl;
    cout << "b: " << (int)symtab.at("b").value << endl;
    cout << "c: " << symtab.at("c").value<<endl;
}