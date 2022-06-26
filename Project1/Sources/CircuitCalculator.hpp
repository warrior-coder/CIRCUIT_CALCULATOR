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
	// ������� ������� ������������� ���� ����������� ���� ������� ������� �����������
	static mtx::MatrixD CalculateCircuit(const CircuitDC& circuitDC)
	{
		// ������ ������� � ��������� �������
		mtx::MatrixD R = circuitDC.GetResistorsMatrix();
		mtx::MatrixD E = circuitDC.GetVoltageSourcesMatrix();
		mtx::MatrixD J = circuitDC.GetCurrentSourcesMatrix();
		std::cout << R << std::endl;
		std::cout << E << std::endl;
		std::cout << J << std::endl;

		// ��������� ������������ ������� RD �� ������� R
		mtx::MatrixD RD = R.GetDiagonal();
		std::cout << RD << std::endl;

		// ��������� ������� ���������� A ��� ����� ����
		mtx::MatrixD A = circuitDC.GetNodalMatrix();
		std::cout << A << std::endl;

		// ��������� ������� ������������ G �� ������� RD
		mtx::MatrixD G = RD.GetReverse();
		std::cout << G << std::endl;

		// ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
		mtx::MatrixD F = (A * G * A.GetTransposed()).GetReverse() * (-A * G * E - A * J);

		// ��������� ���������� �� ���� ������ ����
		mtx::MatrixD U = A.GetTransposed() * F;

		// ��������� ���� � �������������� ������
		mtx::MatrixD IR = G * (U + E);

		return IR;
	}
};

}



