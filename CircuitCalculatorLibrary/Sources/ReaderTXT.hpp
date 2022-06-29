// файл "ReaderTXT.hpp"
// в данном файле описан класс ReaderTXT для чтения файлов формата .TXT

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"


namespace cc
{

class ReaderTXT : public FileReader // класс читатель .TXT файлов
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