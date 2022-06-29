// ���� "ReaderTXT.hpp"
// � ������ ����� ������ ����� ReaderTXT ��� ������ ������ ������� .TXT

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"


namespace cc
{

class ReaderTXT : public FileReader // ����� �������� .TXT ������
{
public:
	ReaderTXT(); // ����������� �� ���������
	explicit ReaderTXT(const std::string& path); // �����������

	~ReaderTXT() override = default; // ���������� �� ���������

	std::vector<std::string> Read() override; // ����� ������ �����
	
	std::string GetPath() const noexcept override; // ����� ��������� �������
	void SetPath(const std::string& path) noexcept override; // ����� ��������� �������
};

}