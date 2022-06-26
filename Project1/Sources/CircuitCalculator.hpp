#pragma once

#include <iostream>
#include <vector>
#include "MATRIX/Matrix.hpp"
#include "CircuitDC.hpp"


namespace cc
{

class CircuitCalculator
{
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
};

}



