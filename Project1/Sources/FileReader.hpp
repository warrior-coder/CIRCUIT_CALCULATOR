// ���� "FileReader.hpp"
// � ������ ����� ������ ��������� FileReader ��� ������ ������

#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace cc
{

// ��������� �������� ������
class FileReader
{
protected:
	std::string _path;

public:
	explicit FileReader(const std::string& path)
		: _path(path)
	{}

	virtual ~FileReader() = default; // ����������� ���������� �� ���������
	virtual std::vector<std::string> Read() = 0; // ����� ����������� ����� ������ �����
	virtual std::string GetPath() const noexcept = 0; // ����� ����������� ����� ��������� �������
	virtual void SetPath(const std::string& path) = 0; // ����� ����������� ����� ��������� �������
};

}