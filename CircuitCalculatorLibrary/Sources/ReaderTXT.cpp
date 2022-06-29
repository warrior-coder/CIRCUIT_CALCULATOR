// ���� "ReaderTXT.cpp"
// � ������ ����� ���������� ����� ReaderTXT ��� ������ ������ ������� .TXT

#include "ReaderTXT.hpp"


namespace cc
{

// �����������
ReaderTXT::ReaderTXT()
	: FileReader(std::string(""))
{}

// �����������
ReaderTXT::ReaderTXT(const std::string& path)
	: FileReader(path)
{}

// ����� ������ �����
std::vector<std::string> ReaderTXT::Read()
{
	std::ifstream ifs(_path);
	std::vector<std::string> result;
	std::string line;

	// ���������� ������ ��� ��������� ������
	std::getline(ifs, line);
	std::getline(ifs, line);

	// ������ �� ����� � ���������� � ������ �����
	while (std::getline(ifs, line))
	{
		result.push_back(line);
	}

	return result;
}

// ����� ��������� �������
std::string ReaderTXT::GetPath() const noexcept
{
	return _path;
}

// ����� ��������� �������
void ReaderTXT::SetPath(const std::string& path) noexcept
{
	_path = path;
}

}