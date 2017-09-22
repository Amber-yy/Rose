#pragma once

#include <string>

enum TokenType
{
	IntLiteral, //0
	RealLiteral,
	StringLiteral,
	Identifier,
	PreIncrement,//这个区间放前置单目运算符
	PreDecrement,
	Positive,
	Negative,
	Negation,
	PostIncrement,//这个区间放后置单目运算符
	PostDecrement, //10
	LeftBracket,
	RightBracket,
	LeftSqBracket,
	RightSqBracket,
	Add,//这个区间放双目运算符
	Sub,
	Multi,
	Divi,
	Mod,
	Assign,//20
	Member,
	Comma,
	Greater,
	Less,
	Equal,
	NotEqual,
	GreaterEqual,
	LessEqual,
	And,
	Or,//30
	AddAssign,
	SubAssign,
	MultiAssign,
	DiviAssign,
	ModAssign,
	ConditionLeft,//三目运算符
	ConditionRight,
	EndOfLine,
	EndOfState,
};

class Token
{
public:
	Token();
	Token(const Token&)=delete;
	Token(Token &&token);
	~Token();
	Token &operator=(const Token &) = delete;
	Token &operator=(Token &&token);
	TokenType getType() const;
	std::string &getString() const;
	int getInteger() const;
	double getReal() const;
	int getLine() const;
	void setType(TokenType type);
	void setString(std::string &string);
	void setInteger(int vi);
	void setReal(double vr);
	void setLineNumber(int line);
protected:
	struct tokenData;
	tokenData *data;
};
