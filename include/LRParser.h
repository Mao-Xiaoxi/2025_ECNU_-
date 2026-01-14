// LRParser.h
#ifndef LRPARSER_H
#define LRPARSER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

// 与之前代码兼容的TokenType枚举
enum TokenType {
    TOK_LBRACE = 0,      // {
    TOK_RBRACE,          // }
    TOK_IF,              // if
    TOK_LEFT_PAREN,      // (
    TOK_RIGHT_PAREN,     // )
    TOK_THEN,            // then
    TOK_ELSE,            // else
    TOK_WHILE,           // while
    TOK_ID,              // ID
    TOK_EQUAL,           // =
    TOK_SEMICOLON,       // ;
    TOK_LT,              // <
    TOK_GT,              // >
    TOK_LE,              // <=
    TOK_GE,              // >=
    TOK_EQ,              // ==
    TOK_PLUS,            // +
    TOK_MINUS,           // -
    TOK_MUL,             // *
    TOK_DIV,             // /
    TOK_NUM,             // NUM
    TOK_END,             // $

    // 非终结符从100开始，避免与终结符冲突
    NT_PROGRAM = 100,
    NT_STMT,
    NT_COMPOUNDSTMT,
    NT_STMTS,
    NT_IFSTMT,
    NT_WHILESTMT,
    NT_ASSGSTMT,
    NT_BOOLEXPR,
    NT_BOOLOP,
    NT_ARITHEXPR,
    NT_ARITHEXPRPRIME,
    NT_MULTEXPR,
    NT_MULTEXPRPRIME,
    NT_SIMPLEEXPR,
    NT_START
};

// 产生式结构
struct Production {
    int lhs;
    std::vector<int> rhs;
    Production(int l = 0, std::vector<int> r = {}) : lhs(l), rhs(r) {}
};

// LR项结构
struct LR_item {
    int productionId;
    int dotPos;

    LR_item(int pid = 0, int pos = 0) : productionId(pid), dotPos(pos) {}

    bool operator<(const LR_item& other) const {
        if (productionId != other.productionId)
            return productionId < other.productionId;
        return dotPos < other.dotPos;
    }

    bool operator==(const LR_item& other) const {
        return productionId == other.productionId && dotPos == other.dotPos;
    }
};

// SLR解析器类
class SLRParser {
private:
    // 核心数据结构
    std::vector<Production> productions;
    std::map<int, std::set<int>> firstSets;
    std::map<int, std::set<int>> followSets;
    std::vector<std::set<LR_item>> itemSets;
    std::map<std::pair<int, int>, std::string> actionTable;
    std::map<std::pair<int, int>, int> gotoTable;
    
    // 解析状态
    int stateCount;
    std::vector<std::string> rightmostDerivation;
    int errorCount;
    bool hasError;
    int errorLine;
    bool insertedSemicolon;

    // 辅助方法
    std::string symbolToString(int symbol);
    bool isTerminal(int symbol);
    bool isEpsilonProduction(const Production& prod);
    
    // 词法分析
    std::vector<std::pair<int, int>> tokenize(const std::string& prog);
    int getTokenType(const std::string& token);
    
    // 文法初始化
    void initializeProductions();
    
    // LR项集操作
    bool areItemSetsEqual(const std::set<LR_item>& set1, const std::set<LR_item>& set2);
    std::set<LR_item> closure(const std::set<LR_item>& items);
    std::set<LR_item> gotoFunction(const std::set<LR_item>& items, int symbol);
    
    // LR(0)项集构造
    void constructLR0ItemSets();
    
    // First和Follow集计算
    void calculateFirstSets();
    void calculateFollowSets();
    
    // SLR分析表构造
    void constructParsingTable();
    
    // 产生式转字符串
    std::string productionToString(int prodId);
    
    // 错误处理
    bool handleError(int state, int token, int lineNum, 
                    std::vector<std::pair<int, int>>& tokens, 
                    size_t& tokenIndex, 
                    std::stack<int>& stateStack, 
                    std::stack<int>& symbolStack);
    
    // 字符串清理
    std::string cleanString(const std::string& str);
    
    // 解析动作处理
    void processShiftAction(const std::string& action, 
                           std::stack<int>& stateStack, 
                           std::stack<int>& symbolStack, 
                           int currentToken, 
                           size_t& tokenIndex);
    
    void processReduceAction(const std::string& action, 
                            std::stack<int>& stateStack, 
                            std::stack<int>& symbolStack);
    
    // 打印最右推导
    void printDerivation();

public:
    SLRParser();
    bool parse(const std::string& prog);
    
    // 获取解析结果
    bool hasErrorOccurred() const { return hasError; }
    int getErrorLine() const { return errorLine; }
    const std::vector<std::string>& getDerivation() const { return rightmostDerivation; }
};


#endif // LRPARSER_H