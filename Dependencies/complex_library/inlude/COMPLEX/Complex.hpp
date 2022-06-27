// Complex.hpp
// в данном файле описан класс Complex для работы с комплексными числами

#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>


namespace cpx
{

constexpr double _pi{ 3.14159265358979 }; // число пи


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


inline double _AtanDeg(const double& value); // тригонометрические функции от градусов
inline double _CosDeg(const double& angleDeg);
inline double _SinDeg(const double& angleDeg);

inline void _NormalizeDeg(double& angleDeg); // нормализация угла
inline double _ArgDeg(const double& re, const double& im); // получение аргумента в грдусах комплексного числа по его алгебраическим параметрам
inline double _Mod(const double& re, const double& im); // получение модуля комплексного числа по его алгебраическим параметрам 

enum ConstructForm // флаг конструктора экспоненциальной формы
{
	EXP
};

enum class OutForm // флаги формы вывода
{
	OUT_ALG,
	OUT_EXP
};


class Complex // класс комплексное число
{
private:
	double _re;						// действительная часть (Real)
	double _im;						// мнимая часть (Imaginary)
	double _mod;					// модуль (Module)
	double _arg;					// аргумент (Argument)

	static OutForm _outForm;		// флаг формы вывода комплексного числа
	static size_t _outPrecision;	// флаг количества знаков после запятой при выводе комплексного числа

public:
	Complex(double re = 0.0, double im = 0.0); // конструкторо алгебраической формы
	
	explicit Complex(double mod, double arg, const ConstructForm&); // конструктор экспоненциальной формы

	~Complex() = default; // деструктор

	Complex operator-() const; // операция отрицания
	Complex operator*() const; // операция комплексной сопряжённости
	
	friend Complex operator+(const Complex& comlex1, const Complex& comlex2); // операция суммы
	friend Complex operator-(const Complex& comlex1, const Complex& comlex2); // операция разности
	friend Complex operator*(const Complex& comlex1, const Complex& comlex2); // операция умножения
	friend Complex operator/(const Complex& comlex1, const Complex& comlex2); // операция деления

	Complex& operator+=(const Complex& comlex2); // операция суммы с присваиванием
	Complex& operator-=(const Complex& comlex2); // операция разности с присваиванием
	Complex& operator*=(const Complex& comlex2); // операция умножения с присваиванием
	Complex& operator/=(const Complex& comlex2); // операция деления с присваиванием

	Complex& operator=(const Complex&) = default; // операция присваивания
	
	bool operator==(const Complex& comlex2) const; // операция равенства
	bool operator!=(const Complex& comlex2) const; // операция неравенства

	double GetRe() const noexcept; // методы получения свойств
	double GetIm() const noexcept;
	double GetMod() const noexcept;
	double GetArg() const noexcept;

	void SetRe(const double& re); // методы установки свойств
	void SetIm(const double& im);
	void SetMod(const double& mod);
	void SetArg(const double& arg);
	
	static void SetOutForm(const OutForm& outForm); // метод установки формы вывода
	static void SetOutPrecision(const size_t& outPrecision); // метод установки количества знаков после запятой при выводе комплексного числа

	friend std::ostream& operator<<(std::ostream& os, const Complex& complex) // операция помещения в поток вывода
	{
		os.setf(std::ios::fixed);
		os.precision(Complex::_outPrecision);

		if (Complex::_outForm == OutForm::OUT_ALG) // выбор формы вывода
		{
			if (!CompareDouble::IsZero(complex._re)) // проверяем значение на отличие от нуля, чтобы не выводить нулевые значения
			{
				os << complex._re;
			}
			else
			{
				os << '0';
			}

			if (!CompareDouble::IsZero(complex._im)) // проверяем значение на отличие от нуля, чтобы не выводить нулевые значения
			{
				os.setf(std::ios::showpos);
				os << complex._im << 'j';
				os.unsetf(std::ios::showpos);
			}
			else
			{
				os << "+0j";
			}
		}
		else
		{
			if (!CompareDouble::IsZero(complex._mod)) // проверяем значение на отличие от нуля, чтобы не выводить нулевые значения
			{
				os << complex._mod;

				if (!CompareDouble::IsZero(complex._arg)) // проверяем значение на отличие от нуля, чтобы не выводить нулевые значения
				{
					os << "e^" << complex._arg << 'j';
				}
			}
			else
			{
				os << '0';
			}
		}

		return os;
	}

	std::string ToString() const; // метод преобразования в строку
	operator double() const noexcept; // операция приведения Complex к double
};

}