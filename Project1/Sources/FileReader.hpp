// файл "FileReader.hpp"
// в данном файле описан интерфейс FileReader для чтения файлов

#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace cc
{

// интерфейс читатель файлов
class FileReader
{
protected:
	std::string _path;

public:
	explicit FileReader(const std::string& path)
		: _path(path)
	{}

	virtual ~FileReader() = default; // виртуальный деструктор по умолчанию
	virtual std::vector<std::string> Read() = 0; // чисто виртуальный метод чтения файла
	virtual std::string GetPath() const noexcept = 0; // чисто виртуальный метод получения свойств
	virtual void SetPath(const std::string& path) = 0; // чисто виртуальный метод установки свойств
};

}