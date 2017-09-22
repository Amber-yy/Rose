#pragma once

#include <string>

enum TokenType
{
	IntLiteral,
	RealLiteral,
	StringLiteral,
	Identifier,
	PreIncrement,//��������ǰ�õ�Ŀ�����
	PreDecrement,
	Positive,
	Negative,
	Negation,
	PostIncrement,//�������ź��õ�Ŀ�����
	PostDecrement,
	LeftBracket,
	RightBracket,
	LeftSqBracket,
	RightSqBracket,
	Add,//��������˫Ŀ�����
	Sub,
	Multi,
	Divi,
	Mod,
	Assign,
	Member,
	Comma,
	Greater,
	Less,
	Equal,
	NotEqual,
	GreaterEqual,
	LessEqual,
	And,
	Or,
	AddAssign,
	SubAssign,
	MultiAssign,
	DiviAssign,
	ModAssign,
	ConditionLeft,//��Ŀ�����
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