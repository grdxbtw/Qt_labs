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
extern const char open_bracket;
extern const char close_bracket;

extern const std::string opmarks;
extern std::regex numb;
extern std::regex numb2;
extern std::regex numb_3;