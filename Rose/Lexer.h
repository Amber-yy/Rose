#pragma once

#include <string>
#include "Token.h"
class Rose;

class Lexer
{
public:
	Lexer(Rose *r);
	~Lexer();
	void parse(std::string &code);
	const Token &read();
	const Token &peek(int index);
	bool hasMore();
protected:
	struct lexerData;
	lexerData *data;
};

