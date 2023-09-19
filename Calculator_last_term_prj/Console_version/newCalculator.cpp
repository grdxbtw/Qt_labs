#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <stdexcept>
#include "Kalkulator.h"

using namespace std;

int main()
{
    Kalkulator<float> K;
    Reader file;

    string expression;
    smatch match;

    cout << endl;
    cout << endl;

    cout << setw(50) << "MENU" << endl;
    cout << "\t" << "U can write expression or write the path to file " << endl;

    cout << endl;
    cout << "\t" << "Press 'q' if u want to leave" << endl;
    cout << endl;
    
    do
    {   
        
        cin.clear();
        getline(cin, expression);
        if (expression == "q")
            exit(0);

        try
        {
            if (regex_search(expression, match, numb_3))
            {
                file.ReadfromFile(expression);
            }
            else
            {
                K.Parsing(expression);
                cout << "Result = " << expression << endl;
            }
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

    } while (true);


    return 0;
}