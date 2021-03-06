#include "Token.h"

struct Token::tokenData
{
	std::string origin;
	double vr;
	long long vi;
	TokenType type;
	int line;
	bool isKey;
};

Token::Token()
{
	data = new tokenData;
	data->vr = data->vi = 0;
	data->line = 0;
	data->isKey = false;
}

Token::Token(Token && token)
{
	data = token.data;
	token.data = nullptr;
}

Token::~Token()
{
	delete data;
}

Token & Token::operator=(Token && token)
{
	delete data;
	data = token.data;
	token.data = nullptr;
	return *this;
}

TokenType Token::getType() const
{
	return data->type;
}

std::string & Token::getString() const
{
	return data->origin;
}

long long Token::getInteger() const
{
	return data->vi;
}

double Token::getReal() const 
{
	return data->vr;
}

int Token::getLine() const
{
	return data->line;
}

bool Token::isKeyword() const
{
	return data->isKey;
}

void Token::setKeyword(bool k)
{
	data->isKey = k;
}

void Token::setType(TokenType type)
{
	data->type = type;
}

void Token::setString(std::string & string)
{
	data->origin = std::move(string);
}

void Token::setInteger(long long vi)
{
	data->vi = vi;
}

void Token::setReal(double vr)
{
	data->vr = vr;
}

void Token::setLineNumber(int line)
{
	data->line = line;
}
