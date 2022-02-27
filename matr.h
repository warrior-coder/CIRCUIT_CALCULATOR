// файл "matr.h"
// в данном файле описан класс Matr для работы с матрицами


// защита от повторного включения заголовочного файла
#pragma once


// подключение необходимых библиотек
#include <iostream>
#include <iomanip>
#include <cmath>
#include <initializer_list>


namespace matr
{

// класс матрица
class Matr
{
private:

	double** buffer;	// буфер хранения данных
	int h;			// высота матрицы (количество строк)
	int w;			// ширина матрицы (количество столбцов)
	int n;			// порядок матрицы (только для квадратных матриц, иначе 0)

public:

	// класс исключения
	class Ex;

	// конструкторы
	Matr(std::initializer_list<std::initializer_list<double>>);
	Matr(std::initializer_list<double>);
	Matr(int, int, double = 0);
	Matr(const Matr&);

	// деструктор
	~Matr();

	// метод индексации
	double& at(int, int);

	// аривметические операции
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

	// очистка матрицы
	void clear();

	// методы получения свойств матрицы
	int getH() const;
	int getW() const;
};

}