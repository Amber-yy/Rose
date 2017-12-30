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
	ASTree getId();
	ASTree getPrimary();
	ASTree getVDefination();
	ASTree getBlock();
	ASTree getIfState();
	ASTree getWhileState();
	ASTree getDoWhileState();
	ASTree getForState();
	ASTree getReturnState();
	ASTree getExprState();
	ASTree getExpr();
	ASTree getFunDefination();
	void getToken(const std::string &t);
	void getGlobalDefination(std::vector<std::string> &names,int start);
	void addError(int line, const std::string &);
	bool isVDefination();
	Rose *rose;
	Lexer *currentLexer;
	std::vector<std::map<std::string, int>> vName;//����ʱ���ű�
	std::map<std::string, int> globals;//ȫ�ַ��ű�
	std::map<std::string, int> functionName;//�Ӻ�������ʵ�ʺ�����ӳ��
	std::vector<ASTree> functions;//ʵ�ʵĺ���
	std::vector<Iterator> globalVariablesIndex;
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
		data->currentLevel = 1;
		std::vector<std::string> globalV;
		data->getGlobalDefination(globalV,data->globals.size());//����ȫ�ֱ���

		//Ȼ������������ඨ��

		while (data->currentLexer->hasMore())
		{
			const Token &t = data->currentLexer->peek(0);

			if (t.getString() == "function")
			{
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

		int start = data->globals.size();
		for (int i = 0; i<globalV.size(); ++i, ++start)//����Ϊ���������ռ䣬��Ϊ��������
		{
			data->globals.insert(std::pair<std::string, int>(globalV[i], start));//ȫ�ֱ�������һ��
			data->variables.push_back(Variable());
			data->globalVariablesIndex.push_back(std::move(--data->variables.end()));//ȫ�ֱ����������ʵ�ʵ�������ת
		}

		data->vName.clear();
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

	if(globals.find(t.getString())!=globals.end()||vName[0].find(t.getString())!=vName[0].end())
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
	b->rose = rose;

	while (currentLexer->hasMore())
	{
		const Token &t = currentLexer->peek(0);
		if (t.getType() ==RightCurBarack)
		{
			break;
		}
		if (t.getString() == "function" || t.getString() == "class")
		{
			addError(t.getLine(), "ȱ��}");
		}

		if (t.getType() == LeftCurBarack)
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
	IfState t= std::make_shared<IfStateC>();

	currentLexer->read();
	getToken("(");
	t->condition=getExpr();
	getToken(")");

	if (currentLexer->peek(0).getType() ==LeftCurBarack)
	{
		t->states=getBlock();
	}
	else
	{
		t->states = getState();
	}

	if (currentLexer->peek(0).getString() == "else")
	{
		currentLexer->read();
		if (currentLexer->peek(0).getType() == LeftCurBarack)
		{
			t->eStates = getBlock();
		}
		else
		{
			t->eStates = getState();
		}
	}

	t->rose = rose;
	
	return t;
}

ASTree Rose::RoseData::getExpr()
{
	return ASTree();
}

ASTree Rose::RoseData::getWhileState()
{
	WhileState t = std::make_shared<WhileStateC>();

	currentLexer->read();
	getToken("(");
	t->condition = getExpr();
	getToken(")");

	if (currentLexer->peek(0).getType() == LeftCurBarack)
	{
		t->states = getBlock();
	}
	else
	{
		t->states = getState();
	}

	return t;
}

ASTree Rose::RoseData::getDoWhileState()
{
	DoWhileState t = std::make_shared<DoWhileStateC>();

	currentLexer->read();

	if (currentLexer->peek(0).getType() == LeftCurBarack)
	{
		t->states = getBlock();
	}
	else
	{
		t->states = getState();
	}

	getToken("while");
	getToken("(");
	t->condition = getExpr();
	getToken(")");
	getToken(";");

	return t;
}

ASTree Rose::RoseData::getForState()
{
	ASTree t;

	bool isForEach = false;

	for (int i = 0; currentLexer->hasMore(i); ++i)
	{
		const Token &t = currentLexer->peek(i);
		if (t.getType() == RightBracket)
		{
			break;
		}
		if (t.getType() == In)
		{
			isForEach = true;
			break;
		}
		if (t.getType() == EndOfState)
		{
			break;
		}
	}

	if (isForEach)
	{
		ForEach s = std::make_shared<ForEachC>();

		currentLexer->read();
		getToken("(");
		s->it = getId();
		getToken("#");
		s->container = getExpr();
		getToken(")");

		if (currentLexer->peek(0).getType() == LeftCurBarack)
		{
			s->states = getBlock();
		}
		else
		{
			s->states = getState();
		}

		t = s;
	}
	else
	{
		ForState s = std::make_shared<ForStateC>();

		currentLexer->read();
		getToken("(");
		s->ini = getExprState();
		s->condition = getExprState();
		s->change = getExpr();
		getToken(")");

		if (currentLexer->peek(0).getType() == LeftCurBarack)
		{
			s->states = getBlock();
		}
		else
		{
			s->states = getState();
		}

		t = s;
	}


	return t;
}

ASTree Rose::RoseData::getReturnState()
{
	ReturnState t = std::make_unique<ReturnStateC>();
	
	currentLexer->read();
	t->value = getExprState();

	return t;
}

ASTree Rose::RoseData::getExprState()
{
	ASTree t=getExpr();
	getToken(";");
	return t;
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
	if (isVDefination())
	{
		return getVDefination();
	}

	return getExprState();
}

ASTree Rose::RoseData::getId()
{
	return ASTree();
}

ASTree Rose::RoseData::getVDefination()
{
	std::vector<std::string> names;

	while (true)
	{
		const Token &t = currentLexer->peek(0);
		if (t.getType() == Identifier)
		{
			if (vName[currentLevel].find(t.getString()) != vName[currentLevel].end())
			{
				addError(t.getLine(), "�ֲ������ض���");
			}

			names.push_back(t.getString());
			vName[currentLevel].insert(std::pair<std::string, int>(t.getString(), vName[currentLevel].size()));
			currentLexer->read();

			TokenType p = currentLexer->peek(0).getType();

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
	}

	VDefination t = std::make_shared<VDefinationC>();
	t->names = std::move(names);
	t->rose = rose;

	return t;
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
	data->rose = this;
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

void Rose::RoseData::getGlobalDefination(std::vector<std::string> &names,int start)
{
	if (!isVDefination())
	{
		return;
	}

	while (true)
	{
		const Token &t = currentLexer->peek(0);
		if (t.getType() == Identifier)
		{
			if (globals.find(t.getString()) != globals.end() || vName[0].find(t.getString()) != vName[0].end())
			{
				addError(t.getLine(),"ȫ�ֱ�ʶ���ض���");
			}

			names.push_back(t.getString());
			vName[0].insert(std::pair<std::string, int>(t.getString(),start));
			++start;
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

}

void Rose::RoseData::addError(int line, const std::string &e)
{
	std::string s=std::to_string(line) + e;
	throw ParseExcepetion(s.c_str());
}

bool Rose::RoseData::isVDefination()
{
	int i;
	for (i = 0;currentLexer->hasMore(i); ++i)
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
