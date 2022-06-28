// ���� "Matrix.hpp"
// � ������ ����� ������ ��������� ����� Matrix ��� ������ � ���������
// ��������� ������� ������ ���������� �� ����� ����������, ������� ������� �������������� ���������� � .LIB �� ���������
// ���������� ���������� ��������� � .H ����� (��� �������� ��� �������� � .INL ����, � ��� ���������� ��� ���������� ����� ��������)
// ����� ������� ���������� �������� header-only

#pragma once // ������ �� ���������� ��������� ������������� �����

// ����������� ����������� ���������
#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>


namespace mtx
{


class CompareDouble // ����������� ����� ��������� ����� double � ��������� epsilon
{
private:
	CompareDouble(); // ��������� �������� �����������, ����� ��������� �������� ����������� ������

	static double _epsilon; // ���������� ����� ����� ��� double

public:
	static bool IsZero(const double& value) noexcept; // ��������� ����� � ���������� �����
	static bool AreEqual(const double& value1, const double& value2) noexcept; // ��������� ���� ����� � ���������� ����� ���������

#ifdef _COMPLEX_HPP_ // ���������� ���������� ��� ����������� ����� ��� ������� ������������ ����������

	// ��������� ����� � ���������� �����
	static bool IsZero(const cpx::Complex& complex) noexcept
	{
		return complex.GetMod() < _epsilon && -complex.GetMod() < _epsilon;
	}

	// ��������� ���� ����� � ���������� ����� ���������
	static bool AreEqual(const cpx::Complex& complex1, const cpx::Complex& complex2) noexcept
	{
		return (complex1 - complex2).GetMod() < _epsilon && -(complex1 - complex2).GetMod() < _epsilon;
	}

#endif // _COMPLEX_HPP_

	static void SetEpsilon(const double& epsilon) noexcept; // ����� ��������� �������
	static double GetEpsilon(const double& epsilon) noexcept; // ����� ��������� �������
};


constexpr int _MinusOnePower(const int& power) noexcept // ������� ���������� -1 � ����� �������
{
	return power % 2 ? -1 : 1;
}


template <typename T>
class Matrix // ����� �������
{
private:
	std::vector<std::vector<T>> _data;	// ������ �������� ������
	size_t _rows;						// ���������� ����� (������)
	size_t _columns;					// ���������� �������� (������)
	size_t _order;						// ������� ������� (������ ��� ����������, ����� 0)

	static size_t _outPrecision;		// ���� ���������� ������ ����� ������� ��� ������
	static size_t _outColumnWidth;		// ���� ������ ������� ������� ��� ������

public:
	explicit Matrix(std::vector<std::vector<T>>&& data) noexcept; // ����������� ����������� ������� ��������
	explicit Matrix(const std::vector<std::vector<T>>& data); // ����������� ����������� ������� ��������
	explicit Matrix(size_t rows, size_t columns, const T& value); // ����������� ���������� ������� ������������ ��������
	explicit Matrix(size_t order, const T& value); // ����������� ���������� ������������ ������� ������������� �������
	explicit Matrix(const std::vector<T>& data); // ����������� ����������� �������
	Matrix(const Matrix& matrix); // ����������� ����������� �������
	Matrix(Matrix&& matrix); // ����������� ����������� �������

	~Matrix() = default; // ����������

	void Validate() const; // ����� ��������� �������

	Matrix& operator=(const Matrix&) = default;
	Matrix& operator=(Matrix&&) noexcept = default;

	Matrix operator-() const; // �������� ��������� �������
	Matrix operator*(const T& value) const; // �������� ��������� ������� �� �����

	Matrix operator+(const Matrix& matrix) const; // �������� �������� ������
	Matrix operator-(const Matrix& matrix) const; // �������� ��������� ������
	Matrix operator*(const Matrix& matrix) const; // �������� ��������� ������
	
	bool operator==(const Matrix& matrix2) const; // ��������� ������
	bool operator!=(const Matrix& matrix2) const;

	const std::vector<T>& operator[](const size_t& i) const; // ����������� �������� ����������
	T& at(const size_t& i, const size_t& j); // ����� ����������

	Matrix GetMinorMatrix(const size_t& minorRow, const size_t& minorColumn) const; // ��������� �������� �������
	T GetMinor(const size_t& minorRow, const size_t& minorColumn) const; // ����� ����� �������
	T GetDeterminant() const; // ����� ������������ �������
	Matrix GetReverse() const; // ����� �������� �������

	Matrix GetDiagonal() const; // ����� ������������ �������
	Matrix GetTransposed() const; // ����� ����������������� �������

	Matrix& Clear(const T& value = 0); // ������� ������� ���������

	bool IsSingleColumn() const noexcept; // �������� �� ������� ��������
	bool IsCompatibleTo(const Matrix& matrix2) const noexcept; // ���������� �� �������
	bool IsEqualSizeTo(const Matrix& matrix2) const noexcept; // ���������� �� ������� ������
	bool IsSquare() const noexcept; // �������� �� ������� ����������
	bool IsIdentity() const; // �������� �� ������� ���������
	bool IsDiagonal() const; // �������� �� ������� ������������

	size_t GetRows() const noexcept; // ������ ��������� ������� �������
	size_t GetColumns() const noexcept;
	size_t GetOrder() const noexcept;

	void SetOutPrecision(const size_t& outPrecision); // ������ ��������� ������ ������
	void SetOutColumnWidth(const size_t& outColumnWidth);

	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) // ������� ������ ������� �� �������
	{
		os.precision(Matrix::_outPrecision);

		for (const auto& row : matrix._data)
		{
			for (const auto& element : row)
			{
				if (!CompareDouble::IsZero(element))
				{
					os << std::setw(Matrix::_outColumnWidth) << element;
				}
				else
				{
					os << std::setw(Matrix::_outColumnWidth) << '0';
				}
			}
			os << '\n';
		}

		return os;
	}
};


using Double = double;
using MatrixD = Matrix<Double>;


#ifdef _COMPLEX_HPP_ // ���������� ������� ����������� ����� ��� ������� ������������ ����������

using MatrixC = Matrix<cpx::Complex>;

#endif // _COMPLEX_HPP_


}


#include "Matrix.inl" // ���������� ���� � ����������� ������� ������ Matrix