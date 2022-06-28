// файл "CircuitCalculator.hpp"
// в данном файле описан статический класс CircuitCalculator для рассчета электрических цепей

#pragma once

#include <iostream>
#include "COMPLEX/Complex.hpp" // подключаем библиотеку "Complex.hpp" для работы с комплексными числами
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами
#include "CircuitDC.hpp"
#include "CircuitAC.hpp"


namespace cc
{

enum CalculationLog // перечисление флагов вывода промежуточных вычислений электрической цепи
{
	NO_LOG			= 0b00000000,
	LOG_R_MATRIX	= 0b00000001,
	LOG_E_MATRIX	= 0b00000010,
	LOG_J_MATRIX	= 0b00000100,
	LOG_RD_MATRIX	= 0b00001000,
	LOG_A_MATRIX	= 0b00010000,
	LOG_G_MATRIX	= 0b00100000,
	LOG_U_MATRIX	= 0b01000000,
	LOG_F_MATRIX	= 0b10000000
};


// статический класс калькулятор электрической цепи
class CircuitCalculator
{
private:
	CircuitCalculator(); // объявляем закрытый констрцктор, чтобы запретить создание экземпляров класса

	static char _calculationLog; // флаг логирования промежуточных вычислений электрической цепи

public:
	static mtx::MatrixD CalculateCircuit(const CircuitDC& circuitDC); // функция расчета электрической цепи постоянного тока методом узловых потенциалов
	static mtx::MatrixC CalculateCircuit(const CircuitAC& circuitAC); // функция расчета электрической цепи переменного тока методом узловых потенциалов

	static char GetCalculationLog() noexcept; // метод получения свойств
	static void SetCalculateLog(const char& calculationLog) noexcept; // метод установки свойств
};

}


namespace cc
{

// определение статической переменной
char CircuitCalculator::_calculationLog{ NO_LOG };


// функция расчета электрической цепи постоянного тока методом узловых потенциалов
mtx::MatrixD CircuitCalculator::CalculateCircuit(const CircuitDC& circuitDC)
{
	mtx::MatrixD R = circuitDC.GetResistorsMatrix(); // задаем матрицу с исходными данными сопротивлений
	if (_calculationLog & LOG_R_MATRIX)
	{
		std::cout << R << std::endl;
	}

	mtx::MatrixD E = circuitDC.GetVoltageSourcesMatrix(); // задаем матрицу с исходными данными источников напряжений
	if (_calculationLog & LOG_E_MATRIX)
	{
		std::cout << E << std::endl;
	}

	mtx::MatrixD J = circuitDC.GetCurrentSourcesMatrix(); // задаем матрицу с исходными данными источников тока
	if (_calculationLog & LOG_J_MATRIX)
	{
		std::cout << J << std::endl;
	}

	mtx::MatrixD RD = R.GetDiagonal(); // формируем диагональную матрицу RD из матрицы R
	if (_calculationLog & LOG_RD_MATRIX)
	{
		std::cout << RD << std::endl;
	}

	mtx::MatrixD A = circuitDC.GetNodalMatrix(); // формируем матрицу соединений A для графа цепи
	if (_calculationLog & LOG_A_MATRIX)
	{
		std::cout << A << std::endl;
	}

	mtx::MatrixD G = RD.GetReverse(); // формируем матрицу проводимости G из матрицы RD
	if (_calculationLog & LOG_G_MATRIX)
	{
		std::cout << G << std::endl;
	}

	mtx::MatrixD F = (A * G * A.GetTransposed()).GetReverse() * (-A * G * E - A * J); // вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
	if (_calculationLog & LOG_F_MATRIX)
	{
		std::cout << F << std::endl;
	}

	mtx::MatrixD U = A.GetTransposed() * F; // вычисляем напряжение на всех ветвях цепи
	if (_calculationLog & LOG_U_MATRIX)
	{
		std::cout << U << std::endl;
	}

	mtx::MatrixD IR = G * (U + E); // вычисляем токи в сопротивлениях ветвей

	return IR;
}

// функция расчета электрической цепи переменного тока методом узловых потенциалов
mtx::MatrixC CircuitCalculator::CalculateCircuit(const CircuitAC& circuitAC)
{
	mtx::MatrixC Z = circuitAC.GetResistorsMatrix(); // задаем матрицу с исходными данными сопротивлений
	if (_calculationLog & LOG_R_MATRIX)
	{
		std::cout << Z << std::endl;
	}

	mtx::MatrixC E = circuitAC.GetVoltageSourcesMatrix(); // задаем матрицу с исходными данными источников напряжений
	if (_calculationLog & LOG_E_MATRIX)
	{
		std::cout << E << std::endl;
	}

	mtx::MatrixC J = circuitAC.GetCurrentSourcesMatrix(); // задаем матрицу с исходными данными источников тока
	if (_calculationLog & LOG_J_MATRIX)
	{
		std::cout << J << std::endl;
	}

	mtx::MatrixC ZD = Z.GetDiagonal(); // формируем диагональную матрицу ZD из матрицы Z
	if (_calculationLog & LOG_RD_MATRIX)
	{
		std::cout << ZD << std::endl;
	}

	mtx::MatrixC A = circuitAC.GetNodalMatrix(); // формируем матрицу соединений A для графа цепи
	if (_calculationLog & LOG_A_MATRIX)
	{
		std::cout << A << std::endl;
	}

	mtx::MatrixC Y = ZD.GetReverse(); // формируем матрицу проводимости Y из матрицы ZD
	if (_calculationLog & LOG_G_MATRIX)
	{
		std::cout << Y << std::endl;
	}

	mtx::MatrixC F = (A * Y * A.GetTransposed()).GetReverse() * (-A * Y * E - A * J); // вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
	if (_calculationLog & LOG_F_MATRIX)
	{
		std::cout << F << std::endl;
	}

	mtx::MatrixC U = A.GetTransposed() * F; // вычисляем напряжение на всех ветвях цепи
	if (_calculationLog & LOG_U_MATRIX)
	{
		std::cout << U << std::endl;
	}

	mtx::MatrixC IR = Y * (U + E); // вычисляем токи в сопротивлениях ветвей

	return IR;
}

// метод получения свойств
char CircuitCalculator::GetCalculationLog() noexcept
{
	return _calculationLog;
}

// метод установки свойств
void CircuitCalculator::SetCalculateLog(const char& calculationLog) noexcept
{
	_calculationLog = calculationLog;
}

}