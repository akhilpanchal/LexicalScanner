#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
/////////////////////////////////////////////////////////////////////
// SemiExpression.h -	read words from a std::stream and return a //
//						list of tokens, that are enough to detect  //
//						language constructs and perform analysis   //
// ver 3.2                                                         //
// Language:	C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Akhil Panchal, TA CSE 687 - Spring 2016			   //
//				MS Computer Science, Syracuse University, 2014-16  //
//				Email:		ahpancha@syr.edu					   //
//				Contact:	(408)-921-0731						   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* TODO:
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Tokenizer.sln /rebuild debug
*
* Maintenance History:
* --------------------
*
*
* Planned Additions and Changes:
* ------------------------------
* TODO:
*/
#include <iosfwd>
#include <string>
#include <vector>
#include "../Tokenizer/ITokCollection.h"
#include "../Tokenizer/Tokenizer.h"

class SemiExpression : public ITokCollection
{
public:
	virtual bool get(bool clear = true);
	virtual size_t length();
	virtual std::string& operator[](int n);
	virtual size_t find(const std::string& tok);
	virtual void push_back(const std::string& tok);
	virtual bool merge(const std::string& firstTok, const std::string& secondTok);
	virtual bool remove(const std::string& tok);
	virtual bool remove(size_t i);
	virtual void toLower();
	virtual void trimFront();
	virtual void clear();
	virtual std::string show(bool showNewLines = false);
private:

};

#endif
