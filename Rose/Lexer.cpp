#include <vector>
#include "Lexer.h"
#include "Rose.h"

static inline bool isPrimary(char c)
{
	return isId(c) || c == '\'' || c == '\"';
}

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
	code = std::move(code);
	data->currentIndex = 0;
	data->codeIndex = 0;

	auto getChar = [this]()->char
	{
		if (data->codeIndex == data->code.size())
		{
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

	auto clearLine = [this](int line)
	{

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

		if (isNumber(temp))//���������������ֿ�ͷ���м�ɸ�������һ��С����
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
						//���һ���﷨������������������̫���.
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
				/*���һ���﷨��������������������Ĳ��Ƿָ���*/
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
				t.setReal(atoi(token.c_str()));
			}
			
			data->allToken.push_back(std::move(t));
		}
		else if (isIdStart(temp))//��ʶ������ĸ���»��߿�ͷ���Ժ�ɰ�����ĸ�����֡��»���
		{
			token += temp;
			while (isId(temp = getChar()))
			{
				token += temp;
			}
			unGetChar();

			Token t;
			t.setLineNumber(line);

			t.setType(Identifier);
			t.setString(token);
			data->allToken.push_back(std::move(t));
		}
		else if (isQuotes(temp))//�ַ���������
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
				/*һ���﷨����*/
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
				data->allToken.push_back(std::move(t));
				continue;
			}
			
			if (next == '+')
			{
				Token t;
				t.setLineNumber(line);
				if (findBinary())
				{
					t.setType(PreIncrement);	
				}
				else
				{
					t.setType(PostIncrement);
				}

				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();

			Token t;
			t.setLineNumber(line);
			if (findBinary())
			{
				t.setType(Positive);
			}
			else
			{
				t.setType(Add);
			}

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
				data->allToken.push_back(std::move(t));
				continue;
			}

			if (next == '-')
			{
				Token t;
				t.setLineNumber(line);
				if (findBinary())
				{
					t.setType(PreDecrement);
				}
				else
				{
					t.setType(PostDecrement);
				}

				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();

			Token t;
			t.setLineNumber(line);
			if (findBinary())
			{
				t.setType(Negative);
			}
			else
			{
				t.setType(Sub);
			}

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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Multi);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Divi);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Mod);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Assign);
			data->allToken.push_back(std::move(t));
		}
		else if (temp =='.')
		{
			char next = getChar();
			if (!isIdStart(next))
			{
				/*���һ���﷨���󣬷Ƿ��ı�ʶ��*/
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Member);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ',')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(Comma);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Negation);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Greater);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			Token t;
			t.setLineNumber(line);
			t.setType(Less);
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
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			/*���һ���﷨���󣬷Ƿ��ķ���*/
		}
		else if (temp == '|')
		{
			char next = getChar();
			if (next == '|')
			{
				Token t;
				t.setLineNumber(line);
				t.setType(Or);
				data->allToken.push_back(std::move(t));
				continue;
			}

			unGetChar();
			/*���һ���﷨���󣬷Ƿ��ķ���*/
		}
		else if (temp == '(')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(LeftBracket);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ')')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(RightBracket);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '[')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(LeftSqBracket);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ']')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(RightSqBracket);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '?')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(ConditionLeft);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == ':')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(ConditionRight);
			data->allToken.push_back(std::move(t));
		}
		else if (temp == '\n')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(EndOfLine);
			data->allToken.push_back(std::move(t));
			++line;
		}
		else if (temp == ';')
		{
			Token t;
			t.setLineNumber(line);
			t.setType(EndOfState);
			data->allToken.push_back(std::move(t));
		}
		else
		{
			/*���һ���﷨���󣬲���ʶ��ı�ʶ��*/
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

bool Lexer::hasMore()
{
	return data->currentIndex==data->allToken.size();
}
