// Semantic.h
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>

// 变量结构体
struct Var {
    std::string type;   // int / real
    double value;
};

// 语义分析器类
class SemanticAnalyzer {
private:
    // 全局数据
    std::vector<std::string> tokens;
    int posi;
    std::map<std::string, Var> symtab;
    std::vector<std::string> errors;
    bool flag;
    
    // 工具函数
    void error(const std::string& msg);
    std::string peek();
    std::string get();
    bool isID(const std::string& s);
    bool isInt(const std::string& s);
    bool isReal(const std::string& s);
    
    // 表达式相关
    std::pair<std::string, double> arithexpr();
    std::pair<std::string, double> simpleexpr();
    std::pair<std::string, double> multexpr();
    bool boolexpr();
    
    // 语句相关
    void stmt(bool execute);
    void compoundstmt(bool execute);
    void assgstmt(bool execute);
    void ifstmt(bool execute);
    
    // 声明相关
    void decls();
    
public:
    SemanticAnalyzer();
    void analyze(const std::string& prog);
    void printResults() const;
};

#endif // SEMANTIC_H