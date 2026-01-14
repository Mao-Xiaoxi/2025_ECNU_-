// src/main.cpp
#define _CRT_SECURE_NO_WARNINGS 1

#include <string>
#include <iostream>
#include "utils.h"

using namespace std;

int main() {
    // 设置编码
    setConsoleEncoding();
    
    // 功能选择以及获取输入。
    string code;

    int mode;
    bool file_flag=true;
    string file_path;
    cout<<"选择实现功能"<<endl;
    cin>>mode;

    getchar();
    cout<<"终端输入(y)or文件输入(default)"<<endl;
    if(getchar()=='y')
        file_flag=false;
    getchar();

    if(file_flag){
        cout<<"输入测试文件路径"<<endl;
        cin>>file_path;
        readCodeFromFile(file_path,code);
    }
    else
        readCodeFromInput(code);
    
        
    if (code.empty()) {
        cout << "输入为空" << endl;

        return 0;
    }
    //执行操作
    switch(mode){
        case 1: //词法分析器
            LexicalFunction(code);
            break;
        case 2: //LL(1)语法分析器
            LL1Function(code);
            break;
        case 3: //LR(1)语法分析器
            LRFunction(code);
            break;
        case 4: //语义分析器
            SemanticsFunction(code);
            break;
        default:
            cout<<"无效参数！"<<endl;
    }

    cout<<"执行结束"<<endl;
    return 0;
}