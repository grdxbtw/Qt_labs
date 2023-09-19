#include "KalkulatorConst.h"

const char* functionNames[]{ "sin","cos","tg","ctg","abs","sqrt" };

const std::size_t functionNamesSize = sizeof(functionNames) / sizeof(functionNames[0]);
const std::string opmarks = "+-*/";
const char open_bracket = '(';
const char close_bracket = ')';

std::regex numb("([+-]?\\d+(?:[.,]\\d+)?)([*/])([+-]?\\d+(?:[.,]\\d+)?)");
std::regex numb2("([+-]?\\d+(?:[.,]\\d+)?)([+-])([+-]?\\d+(?:[.,]\\d+)?)"); //повторить регулярные выражения
std::regex numb_3("\\w+\\.txt$");
