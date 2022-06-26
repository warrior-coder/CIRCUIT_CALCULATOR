// файл "CircuitCalculator.hpp"
// в данном файле описан статический класс CircuitCalculator дл€ рассчета электрических цепей

#pragma once

#include <iostream>
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" дл€ работы с матрицами
#include "CircuitDC.hpp"

namespace cc
{

// статический класс калькул€тор электрической цепи
class CircuitCalculator
{
private:
	CircuitCalculator(); // объ€вл€ем закрытый констрцктор, чтобы запретить создание экземпл€ров класса

public:

	// функци€ расчета электрической цепи посто€нного тока методом узловых потенциалов
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

		// формируем матрицу соединений A дл€ графа цепи
		mtx::MatrixD A = circuitDC.GetNodalMatrix();
		std::cout << A << std::endl;

		// формируем матрицу проводимости G из матрицы RD
		mtx::MatrixD G = RD.GetReverse();
		std::cout << G << std::endl;

		// вычисл€ем потенциалы всех узлов цепи по отношению к базисному узлу
		mtx::MatrixD F = (A * G * A.GetTransposed()).GetReverse() * (-A * G * E - A * J);

		// вычисл€ем напр€жение на всех ветв€х цепи
		mtx::MatrixD U = A.GetTransposed() * F;

		// вычисл€ем токи в сопротивлени€х ветвей
		mtx::MatrixD IR = G * (U + E);

		return IR;
	}

};

}