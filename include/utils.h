#ifndef UTILS_H
#define UTILS_H

#include <string>


void setConsoleEncoding();
void readCodeFromInput(std::string& prog);
void readCodeFromFile(std::string file_path,std::string& prog);


void LexicalFunction(std::string code);
void LL1Function(std::string code);
void LRFunction(std::string code);
void SemanticsFunction(std::string code);


#endif
