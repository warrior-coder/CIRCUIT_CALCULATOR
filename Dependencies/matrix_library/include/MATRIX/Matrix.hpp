// файл "Matrix.hpp"
// в данном файле описан шаблонный класс Matrix для работы с матрицами
// выведение шаблона класса происходит во время компиляции, поэтому вынести реализацию в .lib (в .cpp) не получится
// реализацию необходимо отставить в .h файле (для удобства она вынесена в .inl файл, а при компиляции будет встроена)
// таким образом библиотека header-only

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
	template<typename T>
	static bool IsZero(const T& value) noexcept; // макрос сравнения числа с бесконечно малым
	
	template<typename T>
	static bool AreEqual(const T& value1, const T& value2) noexcept; // макрос равенства двух чисел с бесконечно малой точностью

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

	void SetOutPrecision(const size_t& outPrecision)
	{
		Matrix::_outPrecision = outPrecision;
	}

	void SetOutColumnWidth(const size_t& outColumnWidth)
	{
		Matrix::_outColumnWidth = outColumnWidth;
	}

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

}


#include "Matrix.inl" // встраиваем файл с реализацией шаблона класса Matrix