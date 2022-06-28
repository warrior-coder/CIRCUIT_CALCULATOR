// файл "Matrix.hpp"
// в данном файле описан шаблонный класс Matrix для работы с матрицами
// выведение шаблона класса происходит во время компиляции, поэтому заранее скомиплировать реализацию в .LIB не получится
// реализацию необходимо отставить в .H файле (для удобства она вынесена в .INL файл, а при компиляции его содержание будет встроено)
// таким образом библиотека является header-only

#pragma once // защита от повторного включения заголовочного файла

// подключение необходимых библиотек
#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>


namespace mtx
{


class CompareDouble // статический класс сравненяи чисел double с точностью epsilon
{
private:
	CompareDouble(); // объявляем закрытый конструктор, чтобы запретить создание экземпляров класса

	static double _epsilon; // бесконечно малое число для double

public:
	static bool IsZero(const double& value) noexcept; // сравнение числа с бесконечно малым
	static bool AreEqual(const double& value1, const double& value2) noexcept; // равенство двух чисел с бесконечно малой точностью

#ifdef _COMPLEX_HPP_ // используем перегрузки для комплексных чисел при условии подключенной библиотеки

	// сравнение числа с бесконечно малым
	static bool IsZero(const cpx::Complex& complex) noexcept
	{
		return complex.GetMod() < _epsilon && -complex.GetMod() < _epsilon;
	}

	// равенство двух чисел с бесконечно малой точностью
	static bool AreEqual(const cpx::Complex& complex1, const cpx::Complex& complex2) noexcept
	{
		return (complex1 - complex2).GetMod() < _epsilon && -(complex1 - complex2).GetMod() < _epsilon;
	}

#endif // _COMPLEX_HPP_

	static void SetEpsilon(const double& epsilon) noexcept; // метод установки свойств
	static double GetEpsilon(const double& epsilon) noexcept; // метод получения свойств
};


constexpr int _MinusOnePower(const int& power) noexcept // функция возведения -1 в целую степень
{
	return power % 2 ? -1 : 1;
}


template <typename T>
class Matrix // класс матрица
{
private:
	std::vector<std::vector<T>> _data;	// вектор векторов данных
	size_t _rows;						// количество строк (высота)
	size_t _columns;					// количество столбцов (ширина)
	size_t _order;						// порядок матрицы (только для квадратных, иначе 0)

	static size_t _outPrecision;		// флаг количества знаков после запятой при выводе
	static size_t _outColumnWidth;		// флаг ширины колонок матрицы при выводе

public:
	explicit Matrix(std::vector<std::vector<T>>&& data) noexcept; // конструктор перемещения вектора векторов
	explicit Matrix(const std::vector<std::vector<T>>& data); // конструктор копирования вектора векторов
	explicit Matrix(size_t rows, size_t columns, const T& value); // конструктор заполнения матрицы произвольных размеров
	explicit Matrix(size_t order, const T& value); // конструктор заполнения диагональной матрицы определенного порядка
	explicit Matrix(const std::vector<T>& data); // конструктор копирования вектора
	Matrix(const Matrix& matrix); // конструктор копирования матрицы
	Matrix(Matrix&& matrix); // конструктор перемещения матрицы

	~Matrix() = default; // деструктор

	void Validate() const; // метод валидации матрицы

	Matrix& operator=(const Matrix&) = default;
	Matrix& operator=(Matrix&&) noexcept = default;

	Matrix operator-() const; // операция отрицания матрицы
	Matrix operator*(const T& value) const; // операция умножения матрицы на число

	Matrix operator+(const Matrix& matrix) const; // операция сложения матриц
	Matrix operator-(const Matrix& matrix) const; // операция вычитания матриц
	Matrix operator*(const Matrix& matrix) const; // операция умножения матриц
	
	bool operator==(const Matrix& matrix2) const; // сравнение матриц
	bool operator!=(const Matrix& matrix2) const;

	const std::vector<T>& operator[](const size_t& i) const; // константная операция индексации
	T& at(const size_t& i, const size_t& j); // метод индексации

	Matrix GetMinorMatrix(const size_t& minorRow, const size_t& minorColumn) const; // составить минорную матрицу
	T GetMinor(const size_t& minorRow, const size_t& minorColumn) const; // найти минор матрицы
	T GetDeterminant() const; // найти определитель матрицы
	Matrix GetReverse() const; // найти обратную матрицу

	Matrix GetDiagonal() const; // найти диагональную матрицу
	Matrix GetTransposed() const; // найти транспонированную матрицу

	Matrix& Clear(const T& value = 0); // очистка матрицы значением

	bool IsSingleColumn() const noexcept; // является ли матрица столбцом
	bool IsCompatibleTo(const Matrix& matrix2) const noexcept; // совместимы ли матрицы
	bool IsEqualSizeTo(const Matrix& matrix2) const noexcept; // одинаковые ли размеры матриц
	bool IsSquare() const noexcept; // является ли матрица квадратной
	bool IsIdentity() const; // является ли матрица единичной
	bool IsDiagonal() const; // является ли матрица диагональной

	size_t GetRows() const noexcept; // методы получения свойств матрицы
	size_t GetColumns() const noexcept;
	size_t GetOrder() const noexcept;

	void SetOutPrecision(const size_t& outPrecision); // методы установки флагов вывода
	void SetOutColumnWidth(const size_t& outColumnWidth);

	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) // функция вывода матрицы на консоль
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


#ifdef _COMPLEX_HPP_ // определяем матрицу комплексных чисел при условии подключенной библиотеки

using MatrixC = Matrix<cpx::Complex>;

#endif // _COMPLEX_HPP_


}


#include "Matrix.inl" // встраиваем файл с реализацией шаблона класса Matrix