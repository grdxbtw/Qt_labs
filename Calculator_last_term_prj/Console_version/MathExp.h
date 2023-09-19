#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

class MathExp: public _exception
{
public:
	MathExp()
	{
		error = "math operation error";
	}

	MathExp(std::string ex):error(ex)
	{
		
	}

	const char* what() const 
	{
		return error.c_str();
	}

private:
	std::string error;

};

