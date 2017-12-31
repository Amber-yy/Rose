#pragma once

#include <string>
#include <vector>

class Rose
{
public:
	void addError(int line, const std::string &);
	void doFile(const std::string &fileName);
	void doFiles(const std::vector<std::string> &fileNames);
	void doString(std::string &code);
	Rose();
	~Rose();
protected:
	struct RoseData;
	RoseData *data;
};

