#pragma once
#include <string>

class Rose
{
public:
	Rose();
	~Rose();
	void lex(const std::string &code);
	void addError(int line,const std::string &detail);
protected:
	struct roseData;
	roseData *data;
};

