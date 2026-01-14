// lexer_parser.h
#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <string>
#include <vector>
#include <map>

// Token类型枚举
namespace MyLL1 {
    enum TokenType {
         LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    IF,             // if
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    THEN,           // then
    ELSE,           // else
    WHILE,          // while
    ID,             // ID
    ASSIGN,         // =
    NUM,            // NUM
    SEMICOLON,      // ;
    LESS,           // <
    GREATER,        // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    EQUAL,          // ==
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    END_OF_FILE,    // 文件结束
    EPSILON         // ε
    };
}


// 语法树节点结构
struct TreeNode {
    std::string label;
    std::vector<TreeNode*> children;
    int lineNumber;
    
    TreeNode(std::string l, int ln = 0);
    ~TreeNode();
};

// 词法分析器类
class Lexer {
private:
    std::vector<std::pair<MyLL1::TokenType, std::string>> tokens;
    int currentPos;
    int lineNumber;
    std::vector<int> lineNumbers; // 记录每个token的行号
    
    // 关键字映射表
    std::map<std::string, MyLL1::TokenType> keywords;
    
    // 跳过空白字符
    void skipWhitespace(const std::string& prog, int& pos);
    
    // 判断是否为字母
    bool isLetter(char c);
    
    // 判断是否为数字
    bool isDigit(char c);
    
public:
    Lexer();
    
    void tokenize(const std::string& prog);
    std::pair<MyLL1::TokenType, std::string> getNextToken();
    std::pair<MyLL1::TokenType, std::string> peekToken();
    void reset();
    int getTokenLine(int pos);
    int getCurrentLine() const;
};

// LL(1)解析器类
class Parser {
private:
    Lexer lexer;
    std::pair<MyLL1::TokenType, std::string> currentToken;
    TreeNode* syntaxTree;
    bool hasError;
    int tokenIndex;
    bool flag;
    
    // 打印语法树的辅助函数
    void printTree(TreeNode* node, int depth);
    
    // 错误恢复
    void errorRecovery(const std::string& message);
    
    // 辅助函数：创建新节点
    TreeNode* createNode(const std::string& label);
    
    // 语法分析函数
    TreeNode* program();
    TreeNode* compoundstmt();
    TreeNode* stmt();
    TreeNode* stmts();
    TreeNode* ifstmt();
    TreeNode* whilestmt();
    TreeNode* assgstmt();
    TreeNode* boolexpr();
    TreeNode* boolop();
    TreeNode* arithexpr();
    TreeNode* arithexprprime();
    TreeNode* multexpr();
    TreeNode* multexprprime();
    TreeNode* simpleexpr();
    
    // 辅助函数：检查当前token是否是预期的类型
    bool match(MyLL1::TokenType expected);
    int countNodes(TreeNode* node);
    int getTreeDepth(TreeNode* node);
    
    // 消费当前token，并返回其值
    std::string consume(MyLL1::TokenType expected, const std::string& errorMsg = "");
    
public:
    Parser();
    ~Parser();
    void drawTree();
    
    bool parse(const std::string& input);
    void printSyntaxTree();
    
    // 提供给外部调用的接口
    void Analysis(const std::string& prog);
};

#endif // LEXER_PARSER_H