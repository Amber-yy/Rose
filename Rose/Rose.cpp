#include "Rose.h"
#include "Lexer.h"

#include <iostream>

void Rose::addError(int line, const std::string &info)
{
	std::cout << info<<'\n';
}

Rose::Rose()
{
	//Lexer *t = new Lexer(this);

	//std::string s = "a>>b";

	//t->parse(s);

	//while (t->hasMore())
	//{
	//	std::cout<<t->read().getString()<<'\n';
	//}

}


Rose::~Rose()
{
}
