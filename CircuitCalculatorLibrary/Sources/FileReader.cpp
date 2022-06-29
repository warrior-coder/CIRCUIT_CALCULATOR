// файл "FileReader.hpp"
// в данном файле реализован интерфейс FileReader для чтения файлов

#include "FileReader.hpp"


namespace cc
{

FileReader::FileReader(const std::string& path)
	: _path(path)
{}

}