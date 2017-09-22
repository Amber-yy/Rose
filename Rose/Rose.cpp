#include <vector>
#include "Rose.h"
#include "Lexer.h"

#include <iostream>

struct error
{
	error(int l, std::string d) :line(l), detail(std::move(d)) {};
	int line;
	std::string detail;
};

struct Rose::roseData
{
	roseData(Rose *r) :lexer(r) {};
	Lexer lexer;
	std::vector<error> errors;
};

Rose::Rose()
{
	data = new roseData(this);
}

Rose::~Rose()
{
	delete data;
}

void Rose::lex(const std::string & code)
{
	std::string c = code;
	data->lexer.parse(c);

	if (data->errors.size())
	{
		for (error &t : data->errors)
		{
			std::cout << "error£ºµÚ"<<t.line<<"ÐÐ£º"<<t.detail<<'\n';
		}
	}

	while (data->lexer.hasMore())
	{
		std::cout << data->lexer.read().getType()<<'\n';
	}
}

void Rose::addError(int line, const std::string & detail)
{
	data->errors.push_back(error(line, detail));
}
