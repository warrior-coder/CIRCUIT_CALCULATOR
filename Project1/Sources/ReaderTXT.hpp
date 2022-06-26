// файл "ReaderTXT.hpp"
// в данном файле описан класс ReaderTXT дл€ чтени€ файлов формата .txt

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"

namespace cc
{

// класс читатель .txt файлов
class ReaderTXT : public FileReader
{
public:
	// конструктор
	explicit ReaderTXT(const std::string& path)
		: FileReader(path)
	{}

	~ReaderTXT() override = default; // деструктор по умолчанию

	// метод чтени€ файла
	std::vector<std::string> Read() override
	{
		std::ifstream ifs(_path);
		std::vector<std::string> result;
		std::string line;

		// пропускаем первые две заглавные строки
		std::getline(ifs, line);
		std::getline(ifs, line);
		
		while (std::getline(ifs, line))
		{
			result.emplace_back(line);
		}

		return result;
	}

	// метод получени€ свойств
	std::string GetPath() const noexcept override
	{
		return _path;
	}

	// метод установки свойств
	void SetPath(const std::string& path) override
	{
		_path = path;
	}
};

}