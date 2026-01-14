// LexicalAnalyzer.h
#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>
#include <unordered_map>

// Token结构体
struct Token {
    std::string key;
    int value;
    Token(const std::string& k, int v) : key(k), value(v) {}
};

class LexicalAnalysis {
public:
    // 主分析函数
    int analyze(std::istringstream& iss);
    
    // 获取分析结果
    std::vector<Token> getTokens() const;
    
    // 清空结果
    void clear();
    
private:
    // 内部识别函数
    int is_identifiers(const std::string& lexical);
    int is_operators(const std::string& lexical);
    bool is_constant(const std::string& lexical);
    int identify(const std::string& current);
    
    // 存储结果
    std::vector<Token> lexical_analysis;
    
    // 辅助函数
    template<typename T>
    bool contains(const std::vector<T>& vec, const T& value);
    
    // 词法元素表
    static std::unordered_map<std::string, int> Keywords;
    static std::unordered_map<std::string, int> Operators;
    static std::vector<char> equal_label;
    static std::vector<char> double_label;
    static std::vector<char> single_label;
    static std::vector<char> digits;
    static std::vector<char> invalid_chars;
    static std::vector<char> format_specifiers;
};

// 辅助函数声明
void store_answer(std::string& lexical, int result, int& count, std::vector<Token>& analysis);
void read_prog(std::string& prog);

#endif // LEXICAL_ANALYZER_H