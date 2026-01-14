# 华东师范大学编译原理期末项目

华东师范大学软件工程学院嵌入式方向编译原理与技术实践。

github 链接：https://github.com/Mao-Xiaoxi/2025_ECNU_-.git

## 项目文件结构

本实验使用了经典的C++项目文件结构，将四个实现功能代码进行整合。

```
项目名称
├── .vscode/          # VS Code配置目录
│   └── tasks.json    # 编译任务配置文件
├── build/            # 编译构建目录（编译后生成）
│   ├── bin/
│   │   └── main.exe  # 可执行文件
│   ├── CMakeFiles/   # CMake编译中间文件S
│   ├── cmake_install.cmake
│   ├── CMakeCache.txt
│   └── Makefile      # 编译生成的Makefile
├── data/             # 测试数据目录
│   └── LexicalTest.txt  # 分析测试用例
├── include/          # 头文件目录
│   ├── LexicalAnalyzer.h  # 词法分析器头文件
│   ├── LL1Parser.h        # LL1语法分析器头文件
│   ├── LRParser.h         # LR语法分析器头文件
│   ├── Semantic.h         # 语义分析头文件
│   └── utils.h            # 工具函数头文件
├── src/              # 源文件目录
│   ├── LexicalAnalyzer.cpp  # 词法分析器实现
│   ├── LL1Parser.cpp        # LL1语法分析器实现
│   ├── LRParser.cpp         # LR语法分析器实现
│   ├── main.cpp             # 程序入口
│   ├── Semantic.cpp         # 语义分析实现
│   └── utils.cpp            # 工具函数实现
├── run_tests.bat     # 批处理测试脚本
├── test_config.json  # 批处理测试配置文件
├── CMakeLists.txt    # CMake构建配置文件
└── README.md         # 项目说明文档
```

## 功能实现

* 词法分析器
* LL(1)语法分析器
    * 语法树的生成
* LR(1)语法分析器
* 语义分析器

## 代码编译与验证

本实验已生成已编译文件供检查使用，位于./build/bin/main.exe，可直接使用进行测试。

```
root\build\bin>main.exe
选择实现功能
1
终端输入(y)or文件输入(default)
y
输入C代码 (Ctrl+Z结束):
int a=0;
^Z

分析结果 (5个词法单元):
1: <int, 17>
2: <a, 81>
3: <=, 72>
4: <0, 80>
5: <;, 53>
执行结束
```

同时，也可以自己编译可执行文件进行测试，示例如下：

```bash
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```
生成可执行文件位于root\build\bin\main.exe。

本实验提供了window批处理文件实现样例的批量测试，可通过配置test_config.json文件具体设置参数。具体操作如下：
```
./run_tests.bat
```