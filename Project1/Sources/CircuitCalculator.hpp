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

// статический класс калькулятор электрической цепи
class CircuitCalculator
{
private:
	CircuitCalculator(); // объявляем закрытый констрцктор, чтобы запретить создание экземпляров класса

public:

	// функция расчета электрической цепи постоянного тока методом узловых потенциалов
	static mtx::MatrixD CalculateCircuit(const CircuitDC& circuitDC)
	{
		// задаем матрицы с исходными данными
		mtx::MatrixD R = circuitDC.GetResistorsMatrix();
		mtx::MatrixD E = circuitDC.GetVoltageSourcesMatrix();
		mtx::MatrixD J = circuitDC.GetCurrentSourcesMatrix();
		std::cout << R << std::endl;
		std::cout << E << std::endl;
		std::cout << J << std::endl;

		// формируем диагональную матрицу RD из матрицы R
		mtx::MatrixD RD = R.GetDiagonal();
		std::cout << RD << std::endl;

		// формируем матрицу соединений A для графа цепи
		mtx::MatrixD A = circuitDC.GetNodalMatrix();
		std::cout << A << std::endl;

		// формируем матрицу проводимости G из матрицы RD
		mtx::MatrixD G = RD.GetReverse();
		std::cout << G << std::endl;

		// вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
		mtx::MatrixD F = (A * G * A.GetTransposed()).GetReverse() * (-A * G * E - A * J);

		// вычисляем напряжение на всех ветвях цепи
		mtx::MatrixD U = A.GetTransposed() * F;

		// вычисляем токи в сопротивлениях ветвей
		mtx::MatrixD IR = G * (U + E);

		return IR;
	}
	
	// функция расчета электрической цепи переменного тока методом узловых потенциалов
	static mtx::MatrixC CalculateCircuit(const CircuitAC& circuitAC)
	{
		// задаем матрицы с исходными данными
		mtx::MatrixC Z = circuitAC.GetResistorsMatrix();
		mtx::MatrixC E = circuitAC.GetVoltageSourcesMatrix();
		mtx::MatrixC J = circuitAC.GetCurrentSourcesMatrix();
		std::cout << Z << std::endl;
		std::cout << E << std::endl;
		std::cout << J << std::endl;

		// формируем диагональную матрицу ZD из матрицы Z
		mtx::MatrixC ZD = Z.GetDiagonal();
		std::cout << ZD << std::endl;

		// формируем матрицу соединений A для графа цепи
		mtx::MatrixC A = circuitAC.GetNodalMatrix();
		std::cout << A << std::endl;

		// формируем матрицу проводимости Y из матрицы ZD
		mtx::MatrixC Y = ZD.GetReverse();
		std::cout << Y << std::endl;

		// вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
		mtx::MatrixC F = (A * Y * A.GetTransposed()).GetReverse() * (-A * Y * E - A * J);

		// вычисляем напряжение на всех ветвях цепи
		mtx::MatrixC U = A.GetTransposed() * F;

		// вычисляем токи в сопротивлениях ветвей
		mtx::MatrixC IR = Y * (U + E);

		return IR;
	}

};

}