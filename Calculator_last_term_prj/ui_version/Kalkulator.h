#pragma once
#include"KalkulatorConst.h"
#include<regex>
#include<cctype>
#include<string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <QMessageBox>

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

    for (int i = 0; i < functionNamesSize; i++) // rename
    {
        int fpos = pos - strlen(functionNames[i]) + 1; //////// rename
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
        throw "undefined function";
    }
}

///////////// classes: validator parser trigonometryResolver calculator file_rider

template <typename T>
class Kalkulator
{

public:
	 void Parsing(string& text) const;

protected:
    T Calculate(T op1, T op2, Operation opr) const;  
    void simplify(std::regex local_numb, std::string& text) const;
    void Replace_newtext(std::string& text, int& pos, int& i) const; 
    void TextErase(std::string& text) const;
   
};

template<typename T>
void Kalkulator<T>::Parsing(std::string& text) const
{
    TextErase(text);

    int counter = 0; 
    int pos = -1;
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == open_bracket) 
        {
            if (counter == 0)
                pos = i;
            counter++;
        }
        else if (text[i] == close_bracket)
        {
            counter--;
            if (counter == 0)
                Replace_newtext(text, pos, i);
        }
    }
    ///////////////////////////////////////

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
inline void Kalkulator<T>::Replace_newtext(std::string& text, int& pos, int& i) const
{
                    /////////open brackets
    if (pos == -1) 
        throw "it isn't pair brackets; brackets error";

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
            throw "Divide by 0";
        return  op1 / op2;

    case Operation::MULT:
        return op1 * op2;

    default:
        throw "Error operation";

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

        if (((l_operand < 0 and r_operand < 0) or (l_operand > 0 and r_operand > 0)) and (op == MULT or op == DIV)) //чтоб не было такой ошибки 4  -7* -7 = 4 49
            stringrez = "+" + stringrez;  ////// чтоб не было такой ошибки 4  +7.2 = 4 3.5

        //////////////////////////

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
    explicit Reader(Kalkulator<float> x):parser(x) {}
  
protected:
    void ParseStream(ifstream& fin, ofstream& fout) const;

};

void Reader::ReadfromFile(const string& path ) const
{
    ifstream fin;
    fin.open(path);
    QMessageBox box;
    box.setSizeIncrement(20,10);
    if (fin.is_open())
    {
        cout<<"fin is open" << endl;

        box.setWindowTitle("information");
        box.setText("file is open");

        string filename(path, 0, path.size() - 4);
        filename = filename + "_result.txt";
        ofstream fout(filename);
        if (fout.is_open())
        {
            cout<<"fout is open" << endl;
            box.setInformativeText("output file is done!");
            box.exec();

            try
            {
                ParseStream(fin, fout);
            }
            catch (const char* e)
            {
                cerr << e << endl;
            }
        }
        else
        {
            cerr << "File for write isn't open" << endl;
            box.setInformativeText("output file isn't open!");
            box.exec();

        }
        fout.close();
    }
    else
    {
        cerr << "File for read isn't open" << endl;
        box.setWindowTitle("warning");
        box.setText("file isn't open");
        box.exec();
    }
    fin.close();
}

inline void Reader::ParseStream(ifstream& fin, ofstream& fout) const
{
    while (fin)
    {
        string record;
        getline(fin, record);
        if (record.size() > 0)
        {
            fout << record;
            parser.Parsing(record);
            fout << " = " << record << endl;
        }
    }
}




