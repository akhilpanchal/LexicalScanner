/////////////////////////////////////////////////////////////////////
// SemiExpression.cpp -	read words from a std::stream and return a //
//						list of tokens, that are enough to detect  //
//						language constructs and perform analysis   //
// ver 3.0                                                         //
// Language:  C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Akhil Panchal, TA CSE 687 - Spring 2016			   //
//				MS Computer Science, Syracuse University, 2014-16  //
//				Email:		ahpancha@syr.edu					   //
//				Contact:	(408)-921-0731						   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

#include"SemiExpression.h"

bool SemiExpression::get(bool clear)
{
	return false;
}

size_t SemiExpression::length()
{
	return 0;
}

std::string& SemiExpression::operator[](int n)
{
	std::string s = "";
	return std::move(s);
}

size_t SemiExpression::find(const std::string& tok)
{
	return 0;
}

void SemiExpression::push_back(const std::string& tok)
{

}

bool SemiExpression::merge(const std::string& firstTok, const std::string& secondTok)
{
	return false;
}

bool SemiExpression::remove(const std::string& tok)
{
	return false;
}

bool SemiExpression::remove(size_t i)
{
	return false;
}

void SemiExpression::toLower()
{

}

void SemiExpression::trimFront()
{

}

void SemiExpression::clear()
{

}

std::string SemiExpression::show(bool showNewLines)
{
	std::string s;
	return s;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_SEMIEXPRESSION

#include <fstream>
#include<iostream>

int main()
{
	//  std::string fileSpec = "../Tokenizer/Tokenizer.cpp";

	std::string fileSpec = "../TestFiles/test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker(true);
	toker.attach(&in);
	try
	{
		while (toker.canRead())
		{
			std::string tok = toker.getTok();
			if (tok == "\n")
				tok = "newline";
			std::cout << "\n -- " << tok;
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << std::endl << ex.what() << std::endl;
	}
	return 0;
}

#endif
