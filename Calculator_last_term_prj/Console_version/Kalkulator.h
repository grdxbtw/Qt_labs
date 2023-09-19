#pragma once
#pragma once
#include "CalculatorConst.h"
#include "MathExp.h"
#include<regex>
#include<cctype>
#include<string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>
#include<type_traits>
#include <stdexcept>

using namespace std;

class Trygonometry
{
public:
    static int get_trygonometry_Function(const std::string& text, int& index);
    static double calculate_trygonometry_function(int index, double value);

};

//второй параметр изменяемый, и задача функции найти перед открывающий скобочкой функцию,
//если функция есть то меняем позицию(индекс) на начало функции

inline int Trygonometry::get_trygonometry_Function(const string& text, int& index)
{
    /////////////////////// is_trigonometery
    if (index <= 0)
        return -1;

    int pos = text.find_last_not_of(" \t", index - 1);
    if (pos == string::npos)
        return -1;
    /////////////////

    for (int i = 0; i < functionNamesSize; i++)
    {
        int fpos = pos - strlen(functionNames[i]) + 1;
        if (fpos < 0)
            continue;

        if (text.compare(fpos, strlen(functionNames[i]), functionNames[i]) == 0)
        {
            index = fpos;
            return i;
        }
    }
    return -1;

}

inline double Trygonometry::calculate_trygonometry_function(int index, double value)
{
    TrygonometryFunction trygonometry_function = static_cast<TrygonometryFunction>(index);

    switch (trygonometry_function)
    {
    case TrygonometryFunction::SIN:
        return sin(value);

    case TrygonometryFunction::COS:
        return cos(value);

    case TrygonometryFunction::TG:
        return tan(value);

    case TrygonometryFunction::CTG:
        return 1 / tan(value);

    case TrygonometryFunction::ABS:
        return abs(value);

    case TrygonometryFunction::SQRT:
        return sqrt(value);

    default:
        string message = "In " + string(__func__) + " undefined function with index  " + to_string(index);
        throw std::logic_error(message);
    }
}

///////////// classes: validator parser trigonometryResolver calculator file_rider

template <typename T>
class Kalkulator
{

public:
    static_assert(std::is_arithmetic<T>::value, "wrong parametr");
    void Parsing(string& text) const;

protected:
    T Calculate(T op1, T op2, Operation opr) const;
    void simplify(std::regex local_numb, std::string& text) const;
    void Replace_newtext(std::string& text, int& pos, int& i) const;
    void TextErase(std::string& text) const;
    void FindBrackets(std::string& text) const;

};

template<typename T>
void Kalkulator<T>::Parsing(std::string& text) const
{
    TextErase(text);

    FindBrackets(text);

    simplify(numb, text);
    simplify(numb2, text);
}

template<typename T>
void Kalkulator<T>::TextErase(std::string& text) const
{
    text.erase(std::remove_if(text.begin(),
        text.end(), [](unsigned char x) { return std::isspace(x); }),
        text.end());

}

template<typename T>
void Kalkulator<T>::FindBrackets(std::string& text) const
{
    int counter = 0;
    int pos = -1;
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == OPEN_BRACKET)
        {
            if (counter == 0)
                pos = i;
            counter++;
        }
        else if (text[i] == CLOSE_BRACKET)
        {
            counter--;
            if (counter == 0)
                Replace_newtext(text, pos, i);
        }
    }

}

template<typename T>
inline void Kalkulator<T>::Replace_newtext(std::string& text, int& pos, int& i) const
{
    /////////open brackets
    if (pos == -1)
    {
        string message = "In " + string(__FUNCTION__) + " brackets error; it isn't pair brackets ";
        throw std::logic_error(message);    
    }

    string part = text.substr(pos + 1, i - 1 - pos);
    Parsing(part);

    int functionNomber = Trygonometry::get_trygonometry_Function(text, pos);
    if (functionNomber != -1)
    {
        T value = static_cast<T> (Trygonometry::calculate_trygonometry_function(functionNomber, stod(part)));
        part = to_string(value);
    }
    /////////////////////////
    text.replace(pos, i - pos + 1, part);
    i = pos - 1; /*-1 компенсация инкрементом в форе(цикле)*/
    pos = -1;

}


template<typename T>
inline T Kalkulator<T>::Calculate(T op1, T op2, Operation opr) const
{
    switch (opr)
    {
    case Operation::ADD:
        return op1 + op2;

    case Operation::NEG:
        return op1 - op2;

    case Operation::DIV:
        if (op2 == 0)
            throw MathExp("divide by zero"); ///?
        return  op1 / op2;

    case Operation::MULT:
        return op1 * op2;

    default:
        string message = "In " + string(__FUNCTION__) + " undefined function with index  " + to_string(opr);
        throw std::logic_error(message);

    }
}

template<typename T>
inline void Kalkulator<T>::simplify(regex local_numb, string& text) const
{
    smatch matchGroup;

    while (regex_search(text, matchGroup, local_numb))
    {
        T l_operand = static_cast<T>(stod(matchGroup[1]));
        T r_operand = static_cast<T>(stod(matchGroup[3]));
        Operation op = static_cast<Operation>(opmarks.find(matchGroup[2]));
        T rez = Calculate(l_operand, r_operand, op);
        string stringrez = to_string(rez);

        if (((l_operand < 0 and r_operand < 0) or (l_operand > 0 and r_operand > 0)) and (op == MULT or op == DIV)) // to avoid such a mistake 4  -7* -7 = 4 49
            stringrez = "+" + stringrez;  ////// to avoid such a mistake 4  +7/2 = 4 3.5

        text.replace(matchGroup.prefix().length(), matchGroup[0].length(), stringrez);
    }
}


class Reader
{
private:
    Kalkulator<float> parser;

public:
    void ReadfromFile(const string& path) const;
    Reader() = default;
    explicit Reader(Kalkulator<float> x) :parser(x) {}

protected:
    void ParseStream(ifstream& fin, ofstream& fout) const;

};

void Reader::ReadfromFile(const string& path) const
{
    ifstream fin;
    fin.open(path);
    if (fin.is_open())
    {
        string filename(path, 0, path.size() - 4);
        filename = filename + "_result.txt";
        ofstream fout(filename);
        if (fout.is_open())
        {
            try
            {
                ParseStream(fin, fout);
            }
            catch (MathExp& e)
            {
                cerr << e.what() << endl;
            }
            catch (std::logic_error& e)
            {
                cerr << e.what() << endl;
            }
            catch (std::exception& e)
            {
                cerr << e.what() << endl;
            }
        }
        else
            cerr << "File isn't open" << endl;

        fout.close();

    }
    else
        cerr << "File isn't open" << endl;

    fin.close();
}

inline void Reader::ParseStream(ifstream& fin, ofstream& fout) const
{
    while (fin)
    {
        string record;
        getline(fin, record);
        fout << record;
        parser.Parsing(record);
        fout << " = result_of_calculation " << record << endl;
    }
}

