#pragma once

#include <string>

enum TokenType
{
	IntLiteral, //0
	RealLiteral,
	StringLiteral,
	Identifier,
	PreIncrement,//��������ǰ�õ�Ŀ�����
	PreDecrement,
	Positive,
	Negative,
	Negation,
	LeftCurBarack,
	RightCurBarack,//10
	PostIncrement,//�������ź��õ�Ŀ�����
	PostDecrement, 
	LeftBracket,
	RightBracket,
	LeftSqBracket,
	RightSqBracket,
	Add,//��������˫Ŀ�����
	Sub,
	Multi,
	Divi,//20
	Mod,
	Assign,
	Member,
	Comma,
	Greater,
	Less,
	Equal,
	NotEqual,
	GreaterEqual,
	LessEqual,//30
	And,
	Or,
	LeftCopy,
	RightCopy,
	AddAssign,
	SubAssign,
	MultiAssign,
	DiviAssign,
	ModAssign,
	ConditionLeft,//��Ŀ����� 40
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
	long long getInteger() const;
	double getReal() const;
	int getLine() const;
	bool isKeyword() const;
	void setKeyword(bool k);
	void setType(TokenType type);
	void setString(std::string &string);
	void setInteger(long long vi);
	void setReal(double vr);
	void setLineNumber(int line);
protected:
	struct tokenData;
	tokenData *data;
};
