#include <vector>
#include "Lexer.h"
#include "Rose.h"

static inline bool isSpace(char c)
{
	return 0 <= c&&c <= ' ';
}

static inline bool isNumber(char c)
{
	return '0' <= c &&c <= '9';
}

static inline bool isLetter(char c)
{
	return ('A' <= c&&c <= 'Z') || ('a' <= c&&c <= 'z');
}

static inline bool isIdStart(char c)
{
	return isLetter(c) || c == '-';
}

static inline bool isReal(char c)
{
	return isNumber(c) || c == '.';
}

static inline bool isId(char c)
{
	return isIdStart(c) || isNumber(c);
}

static inline bool isPrimary(char c)
{
	return isId(c) || c == '\'' || c == '\"';
}

static inline int isQuotes(char c)
{
	if (c == '\'')
	{
		return 1;
	}
	else if (c == '\"')
	{
		return 2;
	}
	return 0;
}

static inline bool isDiv(char c)
{
	return !isId(c);
}

static bool isKeyword(std::string &w)
{
	/*function class rose args if else while do break continue	for return*/

	if (w == "function" || w == "class" || w == "rose" || w == "args" ||
		w == "if" || w == "else" || w == "while" || w == "do" || w == "break" ||
		w == "continue" || w == "for" || w == "return")
	{
		return true;
	}

	return false;
}

struct Lexer::lexerData
{
	std::vector<Token> allToken;
	std::string code;
	Rose *rose;
	int currentIndex;
	int codeIndex;
};

Lexer::Lexer(Rose *r)
{
	data = new lexerData;
	data->rose = r;
}

Lexer::~Lexer()
{
	delete data;
}

void Lexer::parse(std::string & code)
{
	data->code = std::move(code);
	data->currentIndex = 0;
	data->codeIndex = 0;

	auto getChar = [this]()->char
	{
		if (data->codeIndex >= data->code.size())
		{
			++data->codeIndex;
			return -1;
		}
		++data->codeIndex;
		return data->code[data->codeIndex - 1];
	};

	auto unGetChar = [this]()
	{
		--data->codeIndex;
	};

	auto findBinary = [this]()->bool
	{
		for (int start = data->allToken.size() - 1; start >= 0; --start)
		{
			TokenType type = data->allToken[start].getType();
			if (Add<=type&&type<=ModAssign)
			{
				return true;
			}
			else if (IntLiteral <= type&&type <= Identifier)
			{
				break;
			}
		}
		return false;
	};

	auto findValue = [this]()->bool
	{
		for (int start = data->allToken.size() - 1; start >= 0; --start)
		{
			TokenType type = data->allToken[start].getType();
			
			if (IntLiteral <= type&&type <= Identifier)
			{
				return true;
			}
		}
		return false;
	};

	auto clearLine = [this,&getChar](int line)
	{
		for (int start = data->allToken.size() - 1; start >= 0; --start)
		{
			if (data->allToken[start].getLine() == line)
			{
				data->allToken.pop_back();
			}
			else
			{
				break;
			}
		}

		char temp;

		while(true)
		{
			temp = getChar();
			if (temp == '\n')
			{
				++line;
				break;
			}
			else if (temp == ';')
			{
				break;
			}
			else if (temp == -1)
			{
				break;
			}
		}
	};

	std::string token;

	int line = 1;

	while (true)
	{
		char temp;
		token.clear();

		do
		{
			temp = getChar();
		} while (isSpace(temp));

		if (temp == -1)
		{
			break;
		}

		if (temp == '/')//注释
		{
			char t = getChar();
			if (t == '/')//单行注释
			{
				while (true)
				{
					t = getChar();
					if (t == '\n')
					{
						temp = t;
						break;
					}
				}
			}
			else if (t == '*')//多行注释
			{
				while (t != -1)
				{
					t = getChar();
					if (t == '*')
					{
						if (getChar() == '/')
						{
							break;
						}
					}
				}
				continue;
			}
			else
			{
				unGetChar();
				unGetChar();
			}
		}
		else if (isNumber(temp))//数字字面量，数字开头，中间可附带至多一个小数点
		{
			token += temp;
			bool real=false;
			bool ok = true;
			while(isReal(temp=getChar()))
			{
				if (temp == '.')
				{
					if (real)
					{
						//添加一条语法错误，数字字面量中有太多的.
						data->rose->addError(line, "数字字面量中含有太多'.'");
						clearLine(line);
						ok = false;
						break;
					}
					else
					{
						real = true;
					}
				}
				token += temp;
			}
			unGetChar();

			if (!isDiv(temp))
			{
				/*添加一条语法错误，整数字面量后紧挨的不是分隔符*/
				data->rose->addError(line, "数字字面量之后有不合法的文本运算符");
				clearLine(line);
				continue;
			}

			if (!ok)
			{
				continue;
			}

			Token t;
			t.setLineNumber(line);

			if (real)
			{
				t.setType(RealLiteral);
				t.setReal(atof(token.c_str()));
			}
			else
			{
				t.setType(IntLiteral);
				t.setInteger(atoll(token.c_str()));
			}
			
			t.setString(token);
			data->allToken.push_back(std::move(t));
		}
		else if (isIdStart(temp))//标识符，字母或下划线开头，以后可包含字母、数字、下划线
		{
			token += temp;
			while (isId(temp = getChar()))
			{
				token += temp;
			}
			unGetChar();

			Token t;
			t.setLineNumber(line);
			t.setString(token);
			t.setType(Identifier);

			if (isKeyword(token))
			{
				t.setKeyword(true);
			}

			data->allToken.push_back(std::move(t));
		}
		else if (isQuotes(temp))//字符串字面量
		{
			int quotes = isQuotes(temp);
			bool isTran=false;

			while (true)
			{
				temp = getChar();
				if (temp == '\\')
				{
					isTran = true;
				}
				else if (isTran)
				{
					if (temp == 'a')
					{
						token += '\a';
					}
					else if (temp == 'b')
					{
						token += '\b';
					}
					else if (temp == 'n')
					{
						token += '\n';
					}
					else if (temp == 't')
					{
						token += '\t';
					}
					else if (temp == '\\')
					{
						token += '\\';
					}
					else if (temp == '\'')
					{
						token += '\'';
					}
					else if (temp == '\"')
					{
						token += '\"';
					}
					else if (temp == '?')
					{
						token += '?';
					}
					else if (temp == '?')
					{
						token += '?';
					}
					else
					{
						token += temp;
					}

					isTran = false;
				}
				else if (isQuotes(temp) == quotes)
				{
					break;
				}
				else
				{
					token += temp;
				}
			}

			temp = getChar();

			if (!isDiv(temp))
			{
				/*一个语法错误*/
				data->rose->addError(line, "字符串字面量之后有不合法的文本运算符");
				clearLine(line);
				continue;
			}

			unGetChar();

			Token t;
			t.setLineNumber(line);
			t.setType(StringLiteral);
			t.setString(token);

			data->allToken.push_back(std::move(t));
		}
		else if (temp == '+')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(AddAssign);
				t.setString(std::string("+="));
				data->allToken.push_back(std::move(t));
				continue;
			}
			
			if (next == '+')
			{
				Token t;
				t.setLineNumber(line);
				if (findBinary()||!findValue())
				{
					t.setType(PreIncrement);	
				}
				else
				{
					t.setType(PostIncrement);
				}
				t.setString(std::string("++"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();

			Token t;
			t.setLineNumber(line);
			if (findBinary() || !findValue())
			{
				t.setType(Positive);
			}
			else
			{
				t.setType(Add);
			}
			t.setString(std::string("+"));
			data->allToken.push_back(std::move(t));
			continue;
		}
		else if (temp == '-')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(SubAssign);
				t.setString(std::string("-="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			if (next == '-')
			{
				Token t;
				t.setLineNumber(line);
				if (findBinary() || !findValue())
				{
					t.setType(PreDecrement);
				}
				else
				{
					t.setType(PostDecrement);
				}
				t.setString(std::string("--"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();

			Token t;
			t.setLineNumber(line);
			if (findBinary() || !findValue())
			{
				t.setType(Negative);
			}
			else
			{
				t.setType(Sub);
			}
			t.setString(std::string("-"));
			data->allToken.push_back(std::move(t));
			continue;
		}
		else if (temp == '*')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(MultiAssign);
				t.setString(std::string("*="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Multi);
			t.setString(std::string("*"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '/')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(DiviAssign);
				t.setString(std::string("/="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Divi);
			t.setString(std::string("/"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '%')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(ModAssign);
				t.setString(std::string("%="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Mod);
			t.setString(std::string("%"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '=')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(Equal);
				t.setString(std::string("=="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Assign);
			t.setString(std::string("="));
			data->allToken.push_back(std::move(t));
		}
		else if (temp =='.')
		{
			char next = getChar();
			if (!isIdStart(next))
			{
				/*添加一条语法错误，非法的标识符*/
				data->rose->addError(line, ".之后有非法的标识符");
				clearLine(line);
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Member);
			t.setString(std::string("."));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ',')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(Comma);
			t.setString(std::string(","));
			data->allToken.push_back(std::move(t));
		}
		else if (temp=='!')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(NotEqual);
				t.setString(std::string("!="));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Negation);
			t.setString(std::string("!"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '>')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(GreaterEqual);
				t.setString(std::string(">="));
				data->allToken.push_back(std::move(t));
				continue;
			}
			if (next == '>')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(RightCopy);
				t.setString(std::string(">>"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Greater);
			t.setString(std::string(">"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '<')
		{
			char next = getChar();
			if (next == '=')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(LessEqual);
				t.setString(std::string("<="));
				data->allToken.push_back(std::move(t));
				continue;
			}
			if (next == '<')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(LeftCopy);
				t.setString(std::string("<<"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Less);
			t.setString(std::string("<"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '&')
		{
			char next = getChar();
			if (next == '&')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(And);
				t.setString(std::string("&&"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			/*添加一条语法错误，非法的符号*/
			data->rose->addError(line, "不合法的&运算符");
			clearLine(line);
		}
		else if (temp == '|')
		{
			char next = getChar();
			if (next == '|')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(Or);
				t.setString(std::string("||"));
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			/*添加一条语法错误，非法的符号*/
			data->rose->addError(line, "不合法的|运算符");
			clearLine(line);
		}
		else if (temp == '(')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(LeftBracket);
			t.setString(std::string("("));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ')')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(RightBracket);
			t.setString(std::string(")"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '[')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(LeftSqBracket);
			t.setString(std::string("["));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ']')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(RightSqBracket);
			t.setString(std::string("]"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '{')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(LeftCurBarack);
			t.setString(std::string("{"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '}')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(RightCurBarack);
			t.setString(std::string("}"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '?')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(ConditionLeft);
			t.setString(std::string("?"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ':')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(ConditionRight);
			t.setString(std::string(":"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '#')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(In);
			t.setString(std::string("#"));
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '\n')
		{
			/*Token t;
			t.setLineNumber(line);
			t.setType(EndOfLine);
			data->allToken.push_back(std::move(t));*/
			++line;
		}
		else if (temp == ';')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(EndOfState);
			t.setString(std::string(";"));
			data->allToken.push_back(std::move(t));
		}
		else
		{
			/*添加一条语法错误，不能识别的标识符*/
			data->rose->addError(line, "不合法的标识符");
			clearLine(line);
		}

	}
	
}

const Token & Lexer::read()
{
	++data->currentIndex;
	return data->allToken[data->currentIndex - 1];
}

const Token & Lexer::peek(int index)
{
	return data->allToken[data->currentIndex +index];
}

bool Lexer::hasMore(int index = 0)
{
	return data->currentIndex+index<data->allToken.size();
}
