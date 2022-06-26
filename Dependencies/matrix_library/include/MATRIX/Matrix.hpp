// ���� "Matrix.hpp"
// � ������ ����� ������ ��������� ����� Matrix ��� ������ � ���������

#pragma once // ������ �� ���������� ��������� ������������� �����

// ����������� ����������� ���������
#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>


namespace mtx
{

constexpr double _epsilon = 1e-15; // ���������� ����� ����� ��� double

template<typename T>
constexpr bool _IsZero(const T& value) noexcept // ������ ��������� ����� � ���������� �����
{
	return static_cast<double>(value) < _epsilon && -static_cast<double>(value) < _epsilon;
}

template<typename T>
constexpr bool _IsEqual(const T& value1, const T& value2) noexcept // ������ ��������� ���� ����� � ���������� ����� ���������
{
	return static_cast<double>(value1 - value2) < _epsilon && -static_cast<double>(value1 - value2) < _epsilon;
}

constexpr int _MinusOnePower(const int& power) noexcept // ������� ���������� -1 � ����� �������
{
	return power % 2 ? -1 : 1;
}

// ����� �������
template <typename T>
class Matrix
{
private:
	std::vector<std::vector<T>> _data;	// ������ �������� ������
	size_t _rows;						// ���������� ����� (������)
	size_t _columns;					// ���������� �������� (������)
	size_t _order;						// ������� ������� (������ ��� ����������, ����� 0)

public:
	explicit Matrix(std::vector<std::vector<T>>&& data); // ����������� ����������� ������� ��������
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

	inline bool IsSingleColumn() const noexcept; // �������� �� ������� ��������
	inline bool IsCompatibleTo(const Matrix& matrix2) const noexcept; // ���������� �� �������
	inline bool IsEqualSizeTo(const Matrix& matrix2) const noexcept; // ���������� �� ������� ������
	inline bool IsSquare() const noexcept; // �������� �� ������� ����������
	bool IsIdentity() const; // �������� �� ������� ���������
	bool IsDiagonal() const; // �������� �� ������� ������������

	inline size_t GetRows() const noexcept; // ������ ��������� ������� �������
	inline size_t GetColumns() const noexcept;
	inline size_t GetOrder() const noexcept;

	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) // ������� ������ ������� �� �������
	{
		for (const auto& row : matrix._data)
		{
			for (const auto& element : row)
			{
				if (!_IsZero(element))
				{
					os << std::setw(10) << element;
				}
				else
				{
					os << std::setw(10) << '0';
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