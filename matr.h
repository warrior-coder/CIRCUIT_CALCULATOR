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

	double** buffer;	// ����� �������� ������
	int h;			// ������ ������� (���������� �����)
	int w;			// ������ ������� (���������� ��������)
	int n;			// ������� ������� (������ ��� ���������� ������, ����� 0)

public:

	// ����� ����������
	class Ex;

	// ������������
	Matr(std::initializer_list<std::initializer_list<double>>);
	Matr(std::initializer_list<double>);
	Matr(int, int, double = 0);
	Matr(const Matr&);

	// ����������
	~Matr();

	// ����� ����������
	double& at(int, int);

	// �������������� ��������
	Matr operator-();
	Matr& operator=(const Matr&);
	friend Matr operator+(const Matr&, const Matr&);
	friend Matr operator-(const Matr&, const Matr&);
	friend Matr operator*(const Matr&, double);
	friend Matr operator*(const Matr&, const Matr&);
	friend std::ostream& operator<<(std::ostream&, const Matr&);

	friend double minor_r(const Matr&, int, int);
	double det();
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