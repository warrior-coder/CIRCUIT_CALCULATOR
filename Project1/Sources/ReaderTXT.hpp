#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"

namespace cc
{

class ReaderTXT : public FileReader
{
private:

public:
	explicit ReaderTXT(const std::string& path)
		: FileReader(path)
	{}

	~ReaderTXT() override = default;

	std::vector<std::string> Read() override
	{
		std::vector<std::string> result;
		std::string line;

		// пропускаем первые две заглавные строки
		std::getline(_ifs, line);
		std::getline(_ifs, line);

		for (size_t i = 0; std::getline(_ifs, line); ++i)
		{
			result.emplace_back(line);
		}

		return result;
	}
};

}