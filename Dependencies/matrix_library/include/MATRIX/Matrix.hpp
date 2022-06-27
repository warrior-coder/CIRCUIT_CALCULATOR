// ���� "Matrix.hpp"
// � ������ ����� ������ ��������� ����� Matrix ��� ������ � ���������
// ��������� ������� ������ ���������� �� ����� ����������, ������� ������� ���������� � .lib (� .cpp) �� ���������
// ���������� ���������� ��������� � .h ����� (��� �������� ��� �������� � .inl ����, � ��� ���������� ����� ��������)
// ����� ������� ���������� header-only

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
	template<typename T>
	static bool IsZero(const T& value) noexcept; // ������ ��������� ����� � ���������� �����
	
	template<typename T>
	static bool AreEqual(const T& value1, const T& value2) noexcept; // ������ ��������� ���� ����� � ���������� ����� ���������

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

	void SetOutPrecision(const size_t& outPrecision)
	{
		Matrix::_outPrecision = outPrecision;
	}

	void SetOutColumnWidth(const size_t& outColumnWidth)
	{
		Matrix::_outColumnWidth = outColumnWidth;
	}

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

}


#include "Matrix.inl" // ���������� ���� � ����������� ������� ������ Matrix