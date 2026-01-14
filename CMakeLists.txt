cmake_minimum_required(VERSION 3.15)
project(LexicalAnalyzer
    VERSION 1.0.0
    DESCRIPTION "C语言词法分析器"
    LANGUAGES CXX
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 创建可执行文件
add_executable(lexical_analyzer)

# 添加源文件
target_sources(lexical_analyzer
    PRIVATE
        src/main.cpp
        src/LexicalAnalyzer.cpp
        src/utils.cpp
        src/LL1Parser.cpp
        src/LRParser.cpp
        src/Semantic.cpp
)

# 添加头文件目录
target_include_directories(lexical_analyzer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置编译特性
target_compile_features(lexical_analyzer PRIVATE cxx_std_17)

# 设置属性
set_target_properties(lexical_analyzer PROPERTIES
    OUTPUT_NAME "main"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


if(WIN32)
    target_compile_definitions(lexical_analyzer PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
# 在CMakeLists.txt中添加

if(MSVC)
    # 对于MSVC编译器
    add_compile_options(/utf-8)
else()
    # 对于GCC/MinGW
    add_compile_options(-fexec-charset=UTF-8 -finput-charset=UTF-8)
endif()
