#include "Token.h"

struct Token::tokenData
{
	double vr;
	std::string origin;
	TokenType type;
	int vi;
	int line;
};

Token::Token()
{
	data = new tokenData;
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

int Token::getInteger() const
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

void Token::setType(TokenType type)
{
	data->type = type;
}

void Token::setString(std::string & string)
{
	data->origin = std::move(string);
}

void Token::setInteger(int vi)
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
