#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include<iostream>
#include <sstream>
#include <fstream>

#include "LexicalAnalyzer.h"
#include "LL1Parser.h"
#include "LRParser.h"
#include "Semantic.h"
using namespace std;

// 设置控制台编码
void setConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// 从标准输入读取代码
void readCodeFromInput(string& prog) {
    cout << "输入C代码 (Ctrl+Z结束):" << endl;
    char c;
    while (cin.get(c)) {
        prog += c;
    }
    cin.clear(); // 清除EOF标志
}

void readCodeFromFile(std::string file_path,std::string& prog){

    std::ifstream file(file_path);
    cout<<file_path<<endl;
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        prog="";
    }
    
    // 方法A：使用rdbuf()快速读取整个文件
    std::string line;
    while (std::getline(file, line)) {
        prog += line + "\n";  // 保留换行符
    }
}

void LexicalFunction(string code){
     // 创建分析器
    LexicalAnalysis analyzer;
    istringstream iss(code);
    
    // 执行分析
    int tokenCount = analyzer.analyze(iss);
    auto tokens = analyzer.getTokens();
    
    // 输出结果
    cout << "\n分析结果 (" << tokenCount << "个词法单元):" << endl;
    for (int i = 0; i < tokenCount; ++i) {
        cout << i+1 << ": <" << tokens[i].key << ", " << tokens[i].value << ">" << endl;
    }
    
}
void LL1Function(std::string code){
    Parser parser;
    try {
        if (parser.parse(code)) {
            cout << "\n分析成功！语法树如下：" << endl;
        } else {
            cout << "\n分析完成（发现语法错误）语法树如下：" << endl;
        }
        
        cout << "========================================" << endl;
        parser.printSyntaxTree();
        cout << endl;

        cout << "\n\n 彩色语法树可视化：" << endl;
        parser.drawTree();
        
    } catch (const exception& e) {
        cout << "分析过程中出现错误: " << e.what() << endl;
        return;
    }
    return;
}
void LRFunction(std::string code){
    SLRParser parser;
    bool result = parser.parse(code);
    return;
}
void SemanticsFunction(std::string code){
    SemanticAnalyzer analyzer;
    analyzer.analyze(code);
    analyzer.printResults();
    return;
}
