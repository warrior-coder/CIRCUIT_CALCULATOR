// ���� "ReaderTXT.hpp"
// � ������ ����� ������ ����� ReaderTXT ��� ������ ������ ������� .txt

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"

namespace cc
{

// ����� �������� .txt ������
class ReaderTXT : public FileReader
{
public:
	// �����������
	explicit ReaderTXT(const std::string& path)
		: FileReader(path)
	{}

	~ReaderTXT() override = default; // ���������� �� ���������

	// ����� ������ �����
	std::vector<std::string> Read() override
	{
		std::ifstream ifs(_path);
		std::vector<std::string> result;
		std::string line;

		// ���������� ������ ��� ��������� ������
		std::getline(ifs, line);
		std::getline(ifs, line);
		
		while (std::getline(ifs, line))
		{
			result.emplace_back(line);
		}

		return result;
	}

	// ����� ��������� �������
	std::string GetPath() const noexcept override
	{
		return _path;
	}

	// ����� ��������� �������
	void SetPath(const std::string& path) override
	{
		_path = path;
	}
};

}