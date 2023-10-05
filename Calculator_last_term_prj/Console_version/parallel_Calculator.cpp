// parallel_Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>
#include "../newCalculator/Kalkulator.h"
#include <future>
#include <functional>
#include <queue>
#include <chrono>


std::string Calc(std::string expression)
{
	Kalkulator<double> k;
	k.Parsing(expression);
	this_thread::sleep_for(std::chrono::seconds(5));
	return expression;
}


int main()
{
	using namespace std;
	using namespace std::chrono_literals;
	std::string expression;
	
	queue<future<string>> qfuture_results;

	while (true)
	{

		getline(cin, expression);
		if (expression == "q" )
			break;
		else
		{	
			packaged_task<string(string)> task(Calc);
			qfuture_results.push(move(task.get_future()));
			thread t(move(task),expression);
			t.detach();

			//qfuture_results.push(move(async(launch::async, Calc, expression)));
		}

		while (!qfuture_results.empty())
		{
			auto& test_f = qfuture_results.front();
			if (test_f.wait_for(1s) == future_status::ready)
			{
				cout << test_f.get() << '\n';
				qfuture_results.pop();
			}
			else
				break;
		}



	}
}

