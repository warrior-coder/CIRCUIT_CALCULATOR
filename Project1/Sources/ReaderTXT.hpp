// ���� "ReaderTXT.hpp"
// � ������ ����� ������ ����� ReaderTXT ��� ������ ������ ������� .txt

#pragma once

#include <algorithm>
#include <sstream>
#include "FileReader.hpp"


namespace cc
{

class ReaderTXT : public FileReader // ����� �������� .txt ������
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