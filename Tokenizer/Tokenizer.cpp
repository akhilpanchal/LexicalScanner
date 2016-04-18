/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
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

#include "Tokenizer.h"
#include <iostream>
#include <cctype>

/////////////////////////////////////////////////////////////////////
// Private abstract class ConsumeState

class ConsumeState
{
public:
  ConsumeState(bool returnTokens = false);
  virtual ~ConsumeState();
  void attach(std::istream* pIn) { _pIn = pIn; }

  void setSpecialSingleChars(std::string ssc);
  void setSpecialDoubleChars(std::string sdc);

  virtual void eatChars() = 0;
  void consumeChars() { 
    _pState->eatChars(); 
    _pState = _pState->nextState();
  }
  bool canRead() { return _pIn->good(); }
  std::string getTok() { return token; }
  bool hasTok() { return token.size() > 0; }
  char getCurrChar() { return currChar;	}
  ConsumeState* nextState();
  bool getHasNext() { return hasNext; }
  bool isSpecialSingleChar(char c);
  bool isSpecialDoubleChar(char firstChar, char secondChar);
  void setReturnComments(bool val) { this->returnComments = val; }
protected:
  static std::string token;
  static std::istream* _pIn;
  static char currChar;
  static bool returnComments;
  static ConsumeState* _pState;
  static ConsumeState* _pEatCppComment;
  static ConsumeState* _pEatCComment;
  static ConsumeState* _pEatWhitespace;
  static ConsumeState* _pEatPunctuator;
  static ConsumeState* _pEatAlphanum;
  static ConsumeState* _pEatNewline;
  static ConsumeState* _pEatSpecialSingleChar;
  static ConsumeState* _pEatSpecialDoubleChars;
  static ConsumeState* _pEatQuotedString;

  static std::vector<char> specialSingleChars;
  static std::vector<std::string> specialDoubleChars;
private:
	bool hasNext;
};

//----< initialize static data members >-----------------------------

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
char ConsumeState::currChar;
bool ConsumeState::returnComments = false;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialDoubleChars = nullptr;
ConsumeState* ConsumeState::_pEatQuotedString = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
std::vector<char> ConsumeState::specialSingleChars = { '"', '\'', '<' ,'>' ,'[' ,']' ,':',';','!' };
std::vector<std::string> ConsumeState::specialDoubleChars = {	"<<", ">>", "::", "()"	};


//----< Sets a list of Special Single Characters >-----------------------------------

void ConsumeState::setSpecialSingleChars(std::string ssc)
{
	for (auto c : ssc)
	{
		specialSingleChars.push_back(c);
	}
}

//----< Sets a list of Special Double Characters >-----------------------------------

void ConsumeState::setSpecialDoubleChars(std::string sdc)
{
	std::string doubleChar;
	for (unsigned int i = 0; i < sdc.size(); i = i + 2)
	{
		doubleChar = sdc[i] + sdc[i + 1];
		specialDoubleChars.push_back(doubleChar);
	}
}

//----< Checks if a char is a Special Single Character >-----------------------------------

bool ConsumeState::isSpecialSingleChar(char currChar)
{
	for (unsigned int i = 0; i < specialSingleChars.size(); i++)
	{
		if (specialSingleChars[i] == currChar)
			return true;
	}
	return false;
}

//----< Checks if a char is a Special Double Character >-----------------------------------

bool ConsumeState::isSpecialDoubleChar(char firstChar, char secondChar)
{
	for (unsigned int i = 0; i < specialDoubleChars.size(); i++)
	{
		if (specialDoubleChars[i][0] == firstChar
			&& specialDoubleChars[i][1] == secondChar)
			return true;
	}
	return false;
}


//----< transition to next state >-----------------------------------

ConsumeState* ConsumeState::nextState()
{
  char chNext = _pIn->peek();
  if (currChar != EOF)
	  hasNext = true;
  else
	  hasNext = false;
  if (currChar == '"' || currChar == '\'')
	  return _pEatQuotedString;
  if (isSpecialDoubleChar(currChar, chNext))
	  return _pEatSpecialDoubleChars;
  if (isSpecialSingleChar(currChar))
	  return _pEatSpecialSingleChar;  
  if (std::isspace(currChar) && currChar != '\n')
    return _pEatWhitespace;
  if (currChar == '/' && chNext == '/')
    return _pEatCppComment;
  if (currChar == '/' && chNext == '*')
    return _pEatCComment;
  if (currChar == '\n')
    return _pEatNewline;
  if (std::isalnum(currChar))
    return _pEatAlphanum;
  if (ispunct(currChar))
    return _pEatPunctuator;
  throw(std::logic_error("invalid type"));
}
/////////////////////////////////////////////////////////////////////
// Derived state that consumes white space and throws it away

class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes C++ comment and throws it away
// - note that Project #1 requires you to have an option to
//   return comments

class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
      if (!_pIn->good())  // end of stream
        return;
	  if (returnComments)
		  token += currChar;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes C comment and throws it away
// - note that Project #1 requires you to have an option to
//   return comments

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
	  char nextChar;
    token.clear();
    //std::cout << "\n  eating C comment";
    do {
      if (!_pIn->good())  // end of stream
        return;
	  if (returnComments)
		  token += currChar;
      currChar = _pIn->get();
	  nextChar = _pIn->peek();
    } while (!(currChar == '*' &&  nextChar == '/'));
	if (returnComments)
	{
		token += currChar;
		token += nextChar;
	}
	currChar = _pIn->get();
	if (_pIn->peek() != EOF)
		currChar = _pIn->get();
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes punctuators and returns as token
// - note that Project #1 requires you to return special punctuators
//   as single or two character tokens.  This class does not do that.

class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating punctuator";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (ispunct(currChar));
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes alphanumerics and returns as token

class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar));
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes newline and returns as single
// character token.  You might extend this to return other single
// and two character tokens as required by Project #1.

class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

/////////////////////////////////////////////////////////////////////
// Derived state that consumes special single characters that can 
// be set by setSpecialSingleChars(string) function provided in the
// public interface of the Toker Class.
// It returns the single character token

class EatSpecialSingleChars : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isSpecialSingleChar(currChar));
	}
};


/////////////////////////////////////////////////////////////////////
// Derived state that consumes special single characters that can 
// be set by setSpecialSingleChars(string) function provided in the
// public interface of the Toker Class.
// It returns the single character token

class EatSpecialDoubleChars : public ConsumeState
{
public:
	virtual void eatChars()
	{
		char secondChar = _pIn->get();
		token.clear();
		do {
			token += currChar;
			token += secondChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			secondChar = _pIn->peek();
		} while (isSpecialDoubleChar(currChar, secondChar));
	}
};

class EatQuotedStrings : public ConsumeState
{
public:
	virtual void eatChars()
	{
		char quoteType = currChar;
		token.clear();
		token += currChar;
		do {
			currChar = _pIn->get();
			token += currChar;
		} while (currChar != quoteType);
		currChar = _pIn->get();
	}
};

//----< base class member function to create derived states >--------

ConsumeState::ConsumeState(bool returnComments)
{
  this->returnComments = returnComments;
  hasNext = true;
  static bool first = true;
  if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatSpecialSingleChar = new EatSpecialSingleChars();
	_pEatSpecialDoubleChars = new EatSpecialDoubleChars();
	_pEatQuotedString = new EatQuotedStrings();
    _pState = _pEatWhitespace;
  }
}
//----< remove derived states from native heap >---------------------

ConsumeState::~ConsumeState()
{
  static bool first = true;
  if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
  }
}
//----< initialize the toker to start with EatWhitespace >-----------

Toker::Toker(bool returnComments) : pConsumer(new EatWhitespace())
{
	pConsumer->setReturnComments(returnComments);
}

Toker::~Toker() { delete pConsumer; }

void Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

void Toker::setSpecialDoubleChars(std::string sdc)
{
	pConsumer->setSpecialDoubleChars(sdc);
}

//----< function allows us to attach to a file or stringstream >-----

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}
//----< this method will be called by SemiExpression >---------------

std::string Toker::getTok()
{
  while(true) 
  {
    pConsumer->consumeChars();  // attempt to fill token - not all states do that
    if (pConsumer->hasTok())
      break;
    if (!pConsumer->canRead())  // end of stream (file or string)?
      return "";
  }
  return pConsumer->getTok();   // return filled token
}

bool Toker::canRead()
{
	//std::string lastToken = pConsumer->getTok();
	//if (lastToken[lastToken.size() - 1] == pConsumer->getCurrChar())
	//	return pConsumer->canRead();
	//else
	//	return true;
	if (pConsumer->getHasNext())
		return true;
	return false;
		//return pConsumer->canRead();

}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

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
  toker.setSpecialSingleChars(";,\"'");
  toker.setSpecialDoubleChars("<<,>>,");
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
	  std::cout <<std::endl<< ex.what() << std::endl;
  }
  return 0;
}

#endif
