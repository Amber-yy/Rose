#include "Rose.h"
#include "ASTree.h"
#include "Lexer.h"

#include <map>
#include <list>
#include <set>

class ParseExcepetion :public std::exception
{
public:
	ParseExcepetion(const char *msg) :exception(msg){}
};

struct Rose::RoseData
{
	ASTree getState();
	ASTree getPrimary();
	ASTree getVDefination();
	ASTree getBlock();
	ASTree getIfState();
	ASTree getWhileState();
	ASTree getDoWhileState();
	ASTree getForState();
	ASTree getReturnState();
	ASTree getExprState();
	ASTree getFunDefination();
	void getToken(const std::string &t);
	void getGlobalDefination();
	void addError(int line, const std::string &);
	bool isVDefination();
	Lexer *currentLexer;
	std::vector<std::map<std::string, int>> vName;
	std::vector<Iterator> globalVariables;
	std::list<Variable> variables;
	int currentLevel;
};

void Rose::addError(int line, const std::string &info)
{
	data->addError(line, info);
}

void Rose::doFile(const std::string & fileName)
{

}

void Rose::doFiles(const std::vector<std::string>& fileNames)
{

}

void Rose::doString(const std::string & code)
{
	try
	{
		Lexer t(this);
		data->currentLexer = &t;

		data->getGlobalDefination();//����ȫ�ֱ���

		//Ȼ������������ඨ��

		while (data->currentLexer->hasMore())
		{
			const Token &t = data->currentLexer->peek(0);

			if (t.getString() == "function")
			{
				data->currentLevel = 1;
				data->getFunDefination();
			}
			else if (t.getString() == "class")
			{

			}
			else
			{
				addError(t.getLine(), "��������Ķ����ʽ����");
			}
		}
	}
	catch (ParseExcepetion &p)
	{
		//������ʾ������Ϣ
		return;
	}
}

ASTree Rose::RoseData::getFunDefination()
{
	currentLexer->read();
	const Token &t= currentLexer->read();

	if (t.getType()!=Identifier)
	{
		addError(t.getLine(), "Ӧ���뺯����");
	}

	if (t.isKeyword())
	{
		addError(t.getLine(), "����ʹ�ùؼ�����Ϊ������");
	}

	if(vName[0].find(t.getString())!=vName[0].end())
	{
		addError(t.getLine(), "ȫ�ֱ�ʶ���ض���");
	}

	getToken("(");
	getToken(")");

	FunDefination fun = std::make_shared<FunDefinationC>();
	fun->name = t.getString();
	fun->block=getBlock();

	return fun;
}

ASTree Rose::RoseData::getBlock()
{
	++currentLevel;
	getToken("{");

	Block b = std::make_shared<BlockC>();

	while (currentLexer->hasMore())
	{
		const Token &t = currentLexer->peek(0);
		if (t.getString() == "}")
		{
			break;
		}
		if (t.getString() == "function" || t.getString() == "class")
		{
			addError(t.getLine(), "ȱ��}");
		}

		if (t.getString() == "{")
		{
			b->statements.push_back(getBlock());
		}
		else
		{
			b->statements.push_back(getState());
		}
	}

	getToken("}");
	--currentLevel;
}

ASTree Rose::RoseData::getIfState()
{
	return ASTree();
}

ASTree Rose::RoseData::getWhileState()
{
	return ASTree();
}

ASTree Rose::RoseData::getDoWhileState()
{
	return ASTree();
}

ASTree Rose::RoseData::getForState()
{
	return ASTree();
}

ASTree Rose::RoseData::getReturnState()
{
	return ASTree();
}

ASTree Rose::RoseData::getExprState()
{
	return ASTree();
}

ASTree Rose::RoseData::getState()
{
	const Token &t = currentLexer->peek(0);
	if (t.getString() == "if")
	{
		return getIfState();
	}
	if (t.getString() == "while")
	{
		return getWhileState();
	}
	if (t.getString() == "do")
	{
		return getDoWhileState();
	}
	if (t.getString() == "for")
	{
		return getForState();
	}
	if (t.getString() == "return")
	{
		return getReturnState();
	}

	return getExprState();
}

void Rose::RoseData::getToken(const std::string & t)
{
	if (currentLexer->peek(0).getString == t)
	{
		currentLexer->read();
	}
	addError(currentLexer->peek(0).getLine(),"ȱ��"+t);
}

#include <iostream>

Rose::Rose()
{
	data = new RoseData;
	Lexer t(this);
	t.parse(std::string("i++ += ++b"));

	while (t.hasMore())
	{
		std::cout<<t.read().getType()<<'\n';
	}

}

Rose::~Rose()
{
	delete data;
}

ASTree Rose::RoseData::getPrimary()
{
	return ASTree();
}

ASTree Rose::RoseData::getVDefination()
{
	
	return ASTree();
}

void Rose::RoseData::getGlobalDefination()
{
	if (!isVDefination())
	{
		return;
	}

	std::vector<std::string> names;
	std::set<std::string> set;

	while (true)
	{
		const Token &t = currentLexer->peek(0);
		if (t.getType() == Identifier)
		{
			if (vName[0].find(t.getString()) != vName[0].end()||set.find(t.getString()) != set.end())
			{
				addError(t.getLine(),"ȫ�ֱ�ʶ���ض���");
			}

			names.push_back(t.getString());
			set.insert(t.getString());
			currentLexer->read();

			TokenType p= currentLexer->peek(0).getType();

			if (p == Comma)
			{
				currentLexer->read();
				continue;
			}
			else if (p == EndOfState)
			{
				currentLexer->read();
				if (isVDefination())
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}

		addError(t.getLine(), "����������ʽ����");
	}

	int start = vName[0].size();
	for (int i = 0; i<names.size(); ++i, ++start)
	{
		vName[0].insert(std::pair<std::string, int>(names[i], start));
		variables.push_back(Variable());
		globalVariables.push_back(std::move(--variables.end()));//ȫ�ֱ����������ʵ�ʵ�������ת
	}

}

void Rose::RoseData::addError(int line, const std::string &e)
{
	std::string s=std::to_string(line) + e;
	throw ParseExcepetion(s.c_str());
}

bool Rose::RoseData::isVDefination()
{
	int i;
	for (i = 0;currentLexer->hasMore(); ++i)
	{
		const Token &t = currentLexer->peek(i);
		TokenType p = t.getType();
		if (p == EndOfState)
		{
			return true;
		}
		if (t.isKeyword())
		{
			return false;
		}
		if (p != Identifier&&p != Comma)
		{
			return false;
		}
	}

	addError(currentLexer->peek(i-1).getLine(),"ȱ�ٷֺ�");
	return false;
}
