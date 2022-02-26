// ���� "matr.h"
// � ������ ����� ������ ����� Matr ��� ������ � ���������


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include <iostream>
#include <iomanip>
#include <cmath>
#include <initializer_list>


namespace matr
{

// ����� �������
class Matr
{
private:

	float** buffer;	// ����� �������� ������
	int h;			// ������ ������� (���������� �����)
	int w;			// ������ ������� (���������� ��������)
	int n;			// ������� ������� (������ ��� ���������� ������, ����� 0)

public:

	// ����� ����������
	class Ex;

	// ������������
	Matr(std::initializer_list<std::initializer_list<float>>);
	Matr(std::initializer_list<float>);
	Matr(int, int, float = 0);
	Matr(const Matr&);

	// ����������
	~Matr();

	// ����� ����������
	float& at(int, int);

	// �������������� ��������
	Matr operator-();
	Matr& operator=(const Matr&);
	friend Matr operator+(const Matr&, const Matr&);
	friend Matr operator-(const Matr&, const Matr&);
	friend Matr operator*(const Matr&, float);
	friend Matr operator*(const Matr&, const Matr&);
	friend std::ostream& operator<<(std::ostream&, const Matr&);

	friend float minor_r(const Matr&, int, int);
	float det();
	Matr rev();
	friend Matr diag(const Matr&);
	Matr transp();

	// ������� �������
	void clear();

	// ������ ��������� ������� �������
	int getH() const;
	int getW() const;
};

}