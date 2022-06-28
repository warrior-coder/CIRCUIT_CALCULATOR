// файл "ReaderTXT.hpp"
// в данном файле описан класс ReaderTXT для чтения файлов формата .txt

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"


namespace cc
{

class ReaderTXT : public FileReader // класс читатель .txt файлов
{
public:
	ReaderTXT(); // конструктор по умолчанию
	explicit ReaderTXT(const std::string& path); // конструктор

	~ReaderTXT() override = default; // деструктор по умолчанию

	std::vector<std::string> Read() override; // метод чтения файла
	
	std::string GetPath() const noexcept override; // метод получения свойств
	void SetPath(const std::string& path) noexcept override; // метод установки свойств
};

}

namespace cc
{

// конструктор
ReaderTXT::ReaderTXT()
	: FileReader(std::string(""))
{}

// конструктор
ReaderTXT::ReaderTXT(const std::string& path)
	: FileReader(path)
{}

// метод чтения файла
std::vector<std::string> ReaderTXT::Read()
{
	std::ifstream ifs(_path);
	std::vector<std::string> result;
	std::string line;

	// пропускаем первые две заглавные строки
	std::getline(ifs, line);
	std::getline(ifs, line);

	// строки из файла и копируются в вектор строк
	while (std::getline(ifs, line))
	{
		result.push_back(line);
	}

	return result;
}

// метод получения свойств
std::string ReaderTXT::GetPath() const noexcept
{
	return _path;
}

// метод установки свойств
void ReaderTXT::SetPath(const std::string& path) noexcept
{
	_path = path;
}

}