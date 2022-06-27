// ���� "CircuitCalculator.hpp"
// � ������ ����� ������ ����������� ����� CircuitCalculator ��� �������� ������������� �����

#pragma once

#include <iostream>
#include "COMPLEX/Complex.hpp" // ���������� ���������� "Complex.hpp" ��� ������ � ������������ �������
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������
#include "CircuitDC.hpp"
#include "CircuitAC.hpp"

namespace cc
{

// ����������� ����� ����������� ������������� ����
class CircuitCalculator
{
private:
	CircuitCalculator(); // ��������� �������� �����������, ����� ��������� �������� ����������� ������

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
	
	// ������� ������� ������������� ���� ����������� ���� ������� ������� �����������
	static mtx::MatrixC CalculateCircuit(const CircuitAC& circuitAC)
	{
		// ������ ������� � ��������� �������
		mtx::MatrixC Z = circuitAC.GetResistorsMatrix();
		mtx::MatrixC E = circuitAC.GetVoltageSourcesMatrix();
		mtx::MatrixC J = circuitAC.GetCurrentSourcesMatrix();
		std::cout << Z << std::endl;
		std::cout << E << std::endl;
		std::cout << J << std::endl;

		// ��������� ������������ ������� ZD �� ������� Z
		mtx::MatrixC ZD = Z.GetDiagonal();
		std::cout << ZD << std::endl;

		// ��������� ������� ���������� A ��� ����� ����
		mtx::MatrixC A = circuitAC.GetNodalMatrix();
		std::cout << A << std::endl;

		// ��������� ������� ������������ Y �� ������� ZD
		mtx::MatrixC Y = ZD.GetReverse();
		std::cout << Y << std::endl;

		// ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
		mtx::MatrixC F = (A * Y * A.GetTransposed()).GetReverse() * (-A * Y * E - A * J);

		// ��������� ���������� �� ���� ������ ����
		mtx::MatrixC U = A.GetTransposed() * F;

		// ��������� ���� � �������������� ������
		mtx::MatrixC IR = Y * (U + E);

		return IR;
	}

};

}