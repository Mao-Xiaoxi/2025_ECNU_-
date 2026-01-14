// lexer_parser.cpp
#include "LL1Parser.h"
#include <iostream>
#include <stack>

using namespace std;

// TreeNode 实现（在全局命名空间）
TreeNode::TreeNode(string l, int ln) : label(l), lineNumber(ln) {}

TreeNode::~TreeNode() {
    for (auto child : children) {
        delete child;
    }
}

// Lexer 实现（在全局命名空间）
Lexer::Lexer() : currentPos(0), lineNumber(1) {
    // 使用MyLL1命名空间中的TokenType枚举值
    keywords = {
        {"if", MyLL1::IF},
        {"then", MyLL1::THEN},
        {"else", MyLL1::ELSE},
        {"while", MyLL1::WHILE}
    };
}

void Lexer::skipWhitespace(const string& prog, int& pos) {
    while (pos < prog.length() && (prog[pos] == ' ' || prog[pos] == '\t' || prog[pos] == '\n' || prog[pos] == '\r')) {
        if (prog[pos] == '\n') {
            lineNumber++;
        }
        pos++;
    }
}

bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

void Lexer::tokenize(const string& prog) {
    int pos = 0;
    int length = prog.length();
    
    while (pos < length) {
        skipWhitespace(prog, pos);
        if (pos >= length) break;
        
        char currentChar = prog[pos];
        
        if (isLetter(currentChar)) {
            string identifier;
            while (pos < length && (isLetter(prog[pos]) || isDigit(prog[pos]))) {
                identifier += prog[pos];
                pos++;
            }
            
            if (keywords.find(identifier) != keywords.end()) {
                tokens.push_back({keywords[identifier], identifier});
            } else {
                tokens.push_back({MyLL1::ID, identifier});
            }
            lineNumbers.push_back(lineNumber);
        }
        else if (isDigit(currentChar)) {
            string number;
            while (pos < length && isDigit(prog[pos])) {
                number += prog[pos];
                pos++;
            }
            tokens.push_back({MyLL1::NUM, number});
            lineNumbers.push_back(lineNumber);
        }
        else {
            MyLL1::TokenType type;
            string tokenStr;
            
            switch (currentChar) {
                case '{':
                    tokenStr = "{";
                    type = MyLL1::LEFT_BRACE;
                    pos++;
                    break;
                case '}':
                    tokenStr = "}";
                    type = MyLL1::RIGHT_BRACE;
                    pos++;
                    break;
                case '(':
                    tokenStr = "(";
                    type = MyLL1::LEFT_PAREN;
                    pos++;
                    break;
                case ')':
                    tokenStr = ")";
                    type = MyLL1::RIGHT_PAREN;
                    pos++;
                    break;
                case ';':
                    tokenStr = ";";
                    type = MyLL1::SEMICOLON;
                    pos++;
                    break;
                case '=':
                    if (pos + 1 < length && prog[pos + 1] == '=') {
                        tokenStr = "==";
                        type = MyLL1::EQUAL;
                        pos += 2;
                    } else {
                        tokenStr = "=";
                        type = MyLL1::ASSIGN;
                        pos++;
                    }
                    break;
                case '<':
                    if (pos + 1 < length && prog[pos + 1] == '=') {
                        tokenStr = "<=";
                        type = MyLL1::LESS_EQUAL;
                        pos += 2;
                    } else {
                        tokenStr = "<";
                        type = MyLL1::LESS;
                        pos++;
                    }
                    break;
                case '>':
                    if (pos + 1 < length && prog[pos + 1] == '=') {
                        tokenStr = ">=";
                        type = MyLL1::GREATER_EQUAL;
                        pos += 2;
                    } else {
                        tokenStr = ">";
                        type = MyLL1::GREATER;
                        pos++;
                    }
                    break;
                case '+':
                    tokenStr = "+";
                    type = MyLL1::PLUS;
                    pos++;
                    break;
                case '-':
                    tokenStr = "-";
                    type = MyLL1::MINUS;
                    pos++;
                    break;
                case '*':
                    tokenStr = "*";
                    type = MyLL1::MULTIPLY;
                    pos++;
                    break;
                case '/':
                    tokenStr = "/";
                    type = MyLL1::DIVIDE;
                    pos++;
                    break;
                default:
                    pos++;
                    continue;
            }
            
            tokens.push_back({type, tokenStr});
            lineNumbers.push_back(lineNumber);
        }
    }
    
    tokens.push_back({MyLL1::END_OF_FILE, "$"});
    lineNumbers.push_back(lineNumber);
}

pair<MyLL1::TokenType, string> Lexer::getNextToken() {
    if (currentPos < tokens.size()) {
        return tokens[currentPos++];
    }
    return {MyLL1::END_OF_FILE, "$"};
}

pair<MyLL1::TokenType, string> Lexer::peekToken() {
    if (currentPos < tokens.size()) {
        return tokens[currentPos];
    }
    return {MyLL1::END_OF_FILE, "$"};
}

void Lexer::reset() {
    currentPos = 0;
}

int Lexer::getTokenLine(int pos) {
    if (pos < lineNumbers.size()) {
        return lineNumbers[pos];
    }
    return lineNumber;
}

int Lexer::getCurrentLine() const {
    return lineNumber;
}

// Parser 实现（在全局命名空间）
Parser::Parser() : syntaxTree(nullptr), hasError(false), tokenIndex(0), flag(true) {}

Parser::~Parser() {
    if (syntaxTree) {
        delete syntaxTree;
    }
}

void Parser::printTree(TreeNode* node, int depth) {
    if (!node) return;
    
    if(flag){
        flag = false;
        for (int i = 0; i < depth; i++) {
            cout << "\t";
        }
        cout << node->label;
        
        for (TreeNode* child : node->children) {
            printTree(child, depth + 1);
        }
    } else {
        cout << endl;
        for (int i = 0; i < depth; i++) {
            cout << "\t";
        }
        cout << node->label;
        
        for (TreeNode* child : node->children) {
            printTree(child, depth + 1);
        }
    }
}

void Parser::errorRecovery(const string& message) {
    if (!hasError) {
        cout << "语法错误,第" << lexer.getTokenLine(tokenIndex-1)-1 << "行," << message << endl;
        hasError = true;
    }
}

TreeNode* Parser::createNode(const string& label) {
    return new TreeNode(label);
}

bool Parser::match(MyLL1::TokenType expected) {
    return currentToken.first == expected;
}

string Parser::consume(MyLL1::TokenType expected, const string& errorMsg) {
    if (match(expected)) {
        string value = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        return value;
    } else if (!errorMsg.empty()) {
        errorRecovery(errorMsg);
        return expected == MyLL1::SEMICOLON ? ";" : "";
    }
    return "";
}

// 语法分析函数实现
TreeNode* Parser::program() {
    TreeNode* node = createNode("program");
    node->children.push_back(compoundstmt());
    return node;
}

TreeNode* Parser::compoundstmt() {
    TreeNode* node = createNode("compoundstmt");
    
    consume(MyLL1::LEFT_BRACE, "缺少\"{\"");
    TreeNode* leftBrace = createNode("{");
    leftBrace->label = "{";
    node->children.push_back(leftBrace);
    
    node->children.push_back(stmts());
    
    consume(MyLL1::RIGHT_BRACE, "缺少\"}\"");
    TreeNode* rightBrace = createNode("}");
    rightBrace->label = "}";
    node->children.push_back(rightBrace);
    
    return node;
}

TreeNode* Parser::stmt() {
    TreeNode* node = createNode("stmt");
    
    if (match(MyLL1::IF)) {
        node->children.push_back(ifstmt());
    } else if (match(MyLL1::WHILE)) {
        node->children.push_back(whilestmt());
    } else if (match(MyLL1::ID)) {
        node->children.push_back(assgstmt());
    } else if (match(MyLL1::LEFT_BRACE)) {
        node->children.push_back(compoundstmt());
    } else {
        errorRecovery("非预期的语句开始");
    }
    
    return node;
}

TreeNode* Parser::stmts() {
    TreeNode* node = createNode("stmts");
    
    if (match(MyLL1::IF) || match(MyLL1::WHILE) || match(MyLL1::ID) || match(MyLL1::LEFT_BRACE)) {
        node->children.push_back(stmt());
        node->children.push_back(stmts());
    } else {
        TreeNode* epsilon = createNode("E");
        node->children.push_back(epsilon);
    }
    
    return node;
}
 
TreeNode* Parser::ifstmt() {
    TreeNode* node = createNode("ifstmt");
    
    consume(MyLL1::IF, "缺少\"if\"");
    TreeNode* ifToken = createNode("if");
    ifToken->label = "if";
    node->children.push_back(ifToken);
    
    consume(MyLL1::LEFT_PAREN, "缺少\"(\"");
    TreeNode* leftParen = createNode("(");
    leftParen->label = "(";
    node->children.push_back(leftParen);
    
    node->children.push_back(boolexpr());
    
    consume(MyLL1::RIGHT_PAREN, "缺少\")\"");
    TreeNode* rightParen = createNode(")");
    rightParen->label = ")";
    node->children.push_back(rightParen);
    
    consume(MyLL1::THEN, "缺少\"then\"");
    TreeNode* thenToken = createNode("then");
    thenToken->label = "then";
    node->children.push_back(thenToken);
    
    node->children.push_back(stmt());
    
    consume(MyLL1::ELSE, "缺少\"else\"");
    TreeNode* elseToken = createNode("else");
    elseToken->label = "else";
    node->children.push_back(elseToken);
    
    node->children.push_back(stmt());
    
    return node;
}

TreeNode* Parser::whilestmt() {
    TreeNode* node = createNode("whilestmt");
    
    consume(MyLL1::WHILE, "缺少\"while\"");
    TreeNode* whileToken = createNode("while");
    whileToken->label = "while";
    node->children.push_back(whileToken);
    
    consume(MyLL1::LEFT_PAREN, "缺少\"(\"");
    TreeNode* leftParen = createNode("(");
    leftParen->label = "(";
    node->children.push_back(leftParen);
    
    node->children.push_back(boolexpr());
    
    consume(MyLL1::RIGHT_PAREN, "缺少\")\"");
    TreeNode* rightParen = createNode(")");
    rightParen->label = ")";
    node->children.push_back(rightParen);
    
    node->children.push_back(stmt());
    
    return node;
}

TreeNode* Parser::assgstmt() {
    TreeNode* node = createNode("assgstmt");
    
    string idValue = consume(MyLL1::ID, "缺少标识符");
    TreeNode* idNode = createNode("ID");
    idNode->label = idValue;
    node->children.push_back(idNode);
    
    consume(MyLL1::ASSIGN, "缺少\"=\"");
    TreeNode* assignNode = createNode("=");
    assignNode->label = "=";
    node->children.push_back(assignNode);
    
    node->children.push_back(arithexpr());
    
    if (!match(MyLL1::SEMICOLON)) {
        errorRecovery("缺少\";\"");
        TreeNode* semicolonNode = createNode(";");
        semicolonNode->label = ";";
        node->children.push_back(semicolonNode);
    } else {
        consume(MyLL1::SEMICOLON, "");
        TreeNode* semicolonNode = createNode(";");
        semicolonNode->label = ";";
        node->children.push_back(semicolonNode);
    }
    
    return node;
}

TreeNode* Parser::boolexpr() {
    TreeNode* node = createNode("boolexpr");
    
    node->children.push_back(arithexpr());
    node->children.push_back(boolop());
    node->children.push_back(arithexpr());
    
    return node;
}

TreeNode* Parser::boolop() {
    TreeNode* node = createNode("boolop");
    
    if (match(MyLL1::LESS) || match(MyLL1::GREATER) || match(MyLL1::LESS_EQUAL) || 
        match(MyLL1::GREATER_EQUAL) || match(MyLL1::EQUAL)) {
        string opValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* opNode = createNode(opValue);
        opNode->label = opValue;
        node->children.push_back(opNode);
    } else {
        errorRecovery("缺少关系运算符");
    }
    
    return node;
}

TreeNode* Parser::arithexpr() {
    TreeNode* node = createNode("arithexpr");
    
    node->children.push_back(multexpr());
    node->children.push_back(arithexprprime());
    
    return node;
}

TreeNode* Parser::arithexprprime() {
    TreeNode* node = createNode("arithexprprime");
    
    if (match(MyLL1::PLUS) || match(MyLL1::MINUS)) {
        string opValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* opNode = createNode(opValue);
        opNode->label = opValue;
        node->children.push_back(opNode);
        
        node->children.push_back(multexpr());
        node->children.push_back(arithexprprime());
    } else {
        TreeNode* epsilon = createNode("E");
        node->children.push_back(epsilon);
    }
    
    return node;
}

TreeNode* Parser::multexpr() {
    TreeNode* node = createNode("multexpr");
    
    node->children.push_back(simpleexpr());
    node->children.push_back(multexprprime());
    
    return node;
}

TreeNode* Parser::multexprprime() {
    TreeNode* node = createNode("multexprprime");
    
    if (match(MyLL1::MULTIPLY) || match(MyLL1::DIVIDE)) {
        string opValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* opNode = createNode(opValue);
        opNode->label = opValue;
        node->children.push_back(opNode);
        
        node->children.push_back(simpleexpr());
        node->children.push_back(multexprprime());
    } else {
        TreeNode* epsilon = createNode("E");
        node->children.push_back(epsilon);
    }
    
    return node;
}

TreeNode* Parser::simpleexpr() {
    TreeNode* node = createNode("simpleexpr");
    
    if (match(MyLL1::ID)) {
        string idValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* idNode = createNode("ID");
        idNode->label = idValue;
        node->children.push_back(idNode);
    } else if (match(MyLL1::NUM)) {
        string numValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* numNode = createNode("NUM");
        numNode->label = numValue;
        node->children.push_back(numNode);
    } else if (match(MyLL1::LEFT_PAREN)) {
        string parenValue = currentToken.second;
        currentToken = lexer.getNextToken();
        tokenIndex++;
        
        TreeNode* leftParen = createNode("(");
        leftParen->label = "(";
        node->children.push_back(leftParen);
        
        node->children.push_back(arithexpr());
        
        if (!match(MyLL1::RIGHT_PAREN)) {
            errorRecovery("缺少\")\"");
        } else {
            currentToken = lexer.getNextToken();
            tokenIndex++;
        }
        TreeNode* rightParen = createNode(")");
        rightParen->label = ")";
        node->children.push_back(rightParen);
    } else {
        errorRecovery("非法的简单表达式");
    }
    
    return node;
}
void Parser::drawTree() {
    if (!syntaxTree) {
        cout << "No syntax tree available. Please parse a program first." << endl;
        return;
    }
    
    // ANSI颜色代码
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";
    const string DIM = "\033[2m";
    
    // 绘制标题
    cout << BOLD << CYAN << "\n╔══════════════════════════════════════════╗" << RESET << endl;
    cout << BOLD << CYAN << "║        SYNTAX TREE VISUALIZATION         ║" << RESET << endl;
    cout << BOLD << CYAN << "╚══════════════════════════════════════════╝" << RESET << endl;
    
    // 使用栈进行非递归遍历
    struct StackItem {
        TreeNode* node;
        int depth;
        vector<bool> isLastStack;  // 记录每一层是否是最后一个兄弟节点
    };
    
    stack<StackItem> nodeStack;
    vector<bool> initialStack;
    nodeStack.push({syntaxTree, 0, initialStack});
    
    while (!nodeStack.empty()) {
        StackItem current = nodeStack.top();
        nodeStack.pop();
        
        TreeNode* node = current.node;
        int depth = current.depth;
        vector<bool> isLastStack = current.isLastStack;
        
        // 绘制连接线
        for (int i = 0; i < depth; i++) {
            if (i < depth - 1) {
                // 中间层：根据上层是否是最后一个兄弟节点来决定是否绘制竖线
                if (isLastStack[i]) {
                    cout << "    ";
                } else {
                    cout << "│   ";
                }
            } else {
                // 当前层：连接符号
                if (depth > 0) {
                    if (isLastStack[depth - 1]) {
                        cout << "└── ";
                    } else {
                        cout << "├── ";
                    }
                }
            }
        }
        
        // 选择节点颜色
        string color = RESET;
        string symbol = "";
        
        string label = node->label;
        
        if (label == "program") {
            color = BOLD + RED;
            symbol = "■ ";
        } else if (label == "compoundstmt" || label == "stmt" || 
                   label == "stmts" || label == "ifstmt" || 
                   label == "whilestmt" || label == "assgstmt") {
            color = BOLD + GREEN;
            symbol = "▣ ";
        } else if (label.find("expr") != string::npos) {
            color = YELLOW;
            symbol = "◇ ";
        } else if (label == "ID") {
            color = BLUE;
            symbol = "＠";
        } else if (label == "NUM") {
            color = MAGENTA;
            symbol = "＃";
        } else if (label == "E") {
            color = DIM;
            symbol = "ε ";
        } else if (label.size() == 1) {
            // 单个字符的运算符或分隔符
            if (label == "+" || label == "-" || label == "*" || label == "/") {
                color = CYAN;
                symbol = "◆ ";
            } else if (label == "<" || label == ">" || label == "=") {
                color = BOLD + CYAN;
                symbol = "◆ ";
            } else if (label == "{" || label == "}" || label == "(" || label == ")" || label == ";") {
                color = DIM;
                symbol = "▫ ";
            } else {
                color = RESET;
                symbol = "";
            }
        } else if (label == "if" || label == "then" || label == "else" || label == "while") {
            color = BOLD + MAGENTA;
            symbol = "★ ";
        } else {
            color = RESET;
            symbol = "";
        }
        
        // 打印节点
        if (depth == 0) {
            cout << BOLD << "● " << color << label << RESET;
        } else {
            cout << color << symbol << label << RESET;
        }
        
        // 显示行号（如果有）
        if (node->lineNumber > 0) {
            cout << DIM << " [" << node->lineNumber << "]" << RESET;
        }
        
        cout << endl;
        
        // 将子节点逆序压入栈中（保证显示顺序正确）
        int childCount = node->children.size();
        for (int i = childCount - 1; i >= 0; i--) {
            vector<bool> newIsLastStack = isLastStack;
            newIsLastStack.push_back(i == childCount - 1);
            nodeStack.push({node->children[i], depth + 1, newIsLastStack});
        }
    }
    
    // 绘制图例
    cout << BOLD << CYAN << "\n╔══════════════════════════════════════════╗" << RESET << endl;
    cout << BOLD << CYAN << "║                LEGEND                    ║" << RESET << endl;
    cout << BOLD << CYAN << "╚══════════════════════════════════════════╝" << RESET << endl;
    
    cout << BOLD << RED << "  ■ program       " << RESET << " - Program root node" << endl;
    cout << BOLD << GREEN << "  ▣ stmt/compound " << RESET << " - Statement nodes" << endl;
    cout << YELLOW << "  ◇ expression    " << RESET << " - Expression nodes" << endl;
    cout << BLUE << "  ＠ ID           " << RESET << " - Identifier" << endl;
    cout << MAGENTA << "  ＃ NUM          " << RESET << " - Number" << endl;
    cout << BOLD << MAGENTA << "  ★ keyword      " << RESET << " - Keywords (if/then/else/while)" << endl;
    cout << CYAN << "  ◆ operator     " << RESET << " - Operators (+, -, *, /, <, >, =)" << endl;
    cout << DIM << "  ▫ separator    " << RESET << " - Separators ({, }, (, ), ;)" << endl;
    cout << DIM << "  ε empty        " << RESET << " - Empty production (E)" << endl;
    
    // 绘制底部边框
    cout << BOLD << CYAN << "\n════════════════════════════════════════════" << RESET << endl;
    cout << "Total nodes: " << countNodes(syntaxTree) << endl;
    cout << "Tree depth: " << getTreeDepth(syntaxTree) << endl;
    cout << BOLD << CYAN << "════════════════════════════════════════════" << RESET << endl << endl;
}

// 辅助函数：统计节点数量
int Parser::countNodes(TreeNode* node) {
    if (!node) return 0;
    
    int count = 1;  // 当前节点
    for (auto child : node->children) {
        count += countNodes(child);
    }
    return count;
}

// 辅助函数：获取树的最大深度
int Parser::getTreeDepth(TreeNode* node) {
    if (!node) return 0;
    
    int maxDepth = 0;
    for (auto child : node->children) {
        int childDepth = getTreeDepth(child);
        if (childDepth > maxDepth) {
            maxDepth = childDepth;
        }
    }
    return maxDepth + 1;
}

bool Parser::parse(const string& input) {
    lexer.tokenize(input);
    currentToken = lexer.getNextToken();
    tokenIndex = 1;
    hasError = false;
    flag = true;
    
    syntaxTree = program();
    
    if (currentToken.first != MyLL1::END_OF_FILE && !hasError) {
        errorRecovery("未预期的输入结束");
    }
    
    return !hasError;
}

void Parser::printSyntaxTree() {
    if (syntaxTree) {
        printTree(syntaxTree, 0);
    }
}

void Parser::Analysis(const std::string& prog) {
    if (parse(prog)) {
        printSyntaxTree();
    } else {
        printSyntaxTree();
    }
}