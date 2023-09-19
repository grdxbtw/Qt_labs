#pragma once
#include<regex>
#include<string>

extern const char* functionNames[];
extern const std::size_t functionNamesSize;
enum Operation
{
    ADD,
    NEG,
    MULT,
    DIV
};

enum TrygonometryFunction
{
    SIN,
    COS,
    TG,
    CTG,
    ABS,
    SQRT

};
extern const char OPEN_BRACKET;
extern const char CLOSE_BRACKET;

extern const std::string opmarks;
extern std::regex numb;
extern std::regex numb2;
extern std::regex numb_3;
