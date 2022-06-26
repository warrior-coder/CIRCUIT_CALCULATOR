#pragma once
#include <fstream>
#include <vector>
#include <string>

namespace cc
{

class FileReader
{
protected:
	std::ifstream _ifs;

public:
	explicit FileReader(const std::string& path)
		: _ifs(path)
	{ }

	virtual ~FileReader() = default;

	virtual std::vector<std::string> Read() = 0;
};

}