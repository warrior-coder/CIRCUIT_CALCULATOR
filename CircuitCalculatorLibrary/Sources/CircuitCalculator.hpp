// ���� "CircuitCalculator.hpp"
// � ������ ����� ������ ����������� ����� CircuitCalculator ��� �������� ������������� �����

#pragma once

#include <iostream>
#include "COMPLEX/Complex.hpp" // ���������� ���������� "Complex.hpp" ��� ������ � ������������ �������
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������
#include "CircuitDC.hpp"
#include "CircuitAC.hpp"
#include "ReaderTXT.hpp" // ������� ���������� �������� .TXT ������


namespace cc
{

enum CalculationLog // ������������ ������ ������ ������������� ���������� ������������� ����
{
	LOG_NO			= 0b00000000,
	LOG_R_MATRIX	= 0b00000001,
	LOG_E_MATRIX	= 0b00000010,
	LOG_J_MATRIX	= 0b00000100,
	LOG_RD_MATRIX	= 0b00001000,
	LOG_A_MATRIX	= 0b00010000,
	LOG_G_MATRIX	= 0b00100000,
	LOG_U_MATRIX	= 0b01000000,
	LOG_F_MATRIX	= 0b10000000,
	LOG_ALL			= 0b11111111
};


// ����������� ����� ����������� ������������� ����
class CircuitCalculator
{
private:
	CircuitCalculator(); // ��������� �������� �����������, ����� ��������� �������� ����������� ������

	static char _calculationLog; // ���� ����������� ������������� ���������� ������������� ����

public:
	static mtx::MatrixD CalculateCircuit(const CircuitDC& circuitDC); // ������� ������� ������������� ���� ����������� ���� ������� ������� �����������
	static mtx::MatrixC CalculateCircuit(const CircuitAC& circuitAC); // ������� ������� ������������� ���� ����������� ���� ������� ������� �����������

	static char GetCalculationLog() noexcept; // ����� ��������� �������
	static void SetCalculateLog(const char& calculationLog) noexcept; // ����� ��������� �������
};

}


// ���������� ����� CircuitCalculator ��������������� ��� �������� ������������� �����
namespace cc
{

// ����������� ����������� ����������
char CircuitCalculator::_calculationLog{ LOG_NO };


// ������� ������� ������������� ���� ����������� ���� ������� ������� �����������
mtx::MatrixD CircuitCalculator::CalculateCircuit(const CircuitDC& circuitDC)
{
	mtx::MatrixD R = circuitDC.GetResistorsMatrix(); // ������ ������� � ��������� ������� �������������
	if (_calculationLog & LOG_R_MATRIX)
	{
		std::cout << "Resistors matrix:\n" << R << std::endl;
	}

	mtx::MatrixD E = circuitDC.GetVoltageSourcesMatrix(); // ������ ������� � ��������� ������� ���������� ����������
	if (_calculationLog & LOG_E_MATRIX)
	{
		std::cout << "Voltage sources matrix:\n" << E << std::endl;
	}

	mtx::MatrixD J = circuitDC.GetCurrentSourcesMatrix(); // ������ ������� � ��������� ������� ���������� ����
	if (_calculationLog & LOG_J_MATRIX)
	{
		std::cout << "Current sources matrix:\n" << J << std::endl;
	}

	mtx::MatrixD RD = R.GetDiagonal(); // ��������� ������������ ������� RD �� ������� R
	if (_calculationLog & LOG_RD_MATRIX)
	{
		std::cout << "Resistors diagonal matrix:\n" << RD << std::endl;
	}

	mtx::MatrixD A = circuitDC.GetNodalMatrix(); // ��������� ������� ���������� A ��� ����� ����
	if (_calculationLog & LOG_A_MATRIX)
	{
		std::cout << "Nodal matrix:\n" << A << std::endl;
	}

	mtx::MatrixD G = RD.GetReverse(); // ��������� ������� ������������ G �� ������� RD
	if (_calculationLog & LOG_G_MATRIX)
	{
		std::cout << "Conductivity diagonal matrix:\n" << G << std::endl;
	}

	mtx::MatrixD F = (A * G * A.GetTransposed()).GetReverse() * (-A * G * E - A * J); // ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
	if (_calculationLog & LOG_F_MATRIX)
	{
		std::cout << "Nodes potentials (relative to the last):\n" << F << std::endl;
	}

	mtx::MatrixD U = A.GetTransposed() * F; // ��������� ���������� �� ���� ������ ����
	if (_calculationLog & LOG_U_MATRIX)
	{
		std::cout << "Voltages on branches:\n" << U << std::endl;
	}

	mtx::MatrixD IR = G * (U + E); // ��������� ���� � �������������� ������

	return IR;
}

// ������� ������� ������������� ���� ����������� ���� ������� ������� �����������
mtx::MatrixC CircuitCalculator::CalculateCircuit(const CircuitAC& circuitAC)
{
	mtx::MatrixC Z = circuitAC.GetResistorsMatrix(); // ������ ������� � ��������� ������� �������������
	if (_calculationLog & LOG_R_MATRIX)
	{
		std::cout << Z << std::endl;
	}

	mtx::MatrixC E = circuitAC.GetVoltageSourcesMatrix(); // ������ ������� � ��������� ������� ���������� ����������
	if (_calculationLog & LOG_E_MATRIX)
	{
		std::cout << E << std::endl;
	}

	mtx::MatrixC J = circuitAC.GetCurrentSourcesMatrix(); // ������ ������� � ��������� ������� ���������� ����
	if (_calculationLog & LOG_J_MATRIX)
	{
		std::cout << J << std::endl;
	}

	mtx::MatrixC ZD = Z.GetDiagonal(); // ��������� ������������ ������� ZD �� ������� Z
	if (_calculationLog & LOG_RD_MATRIX)
	{
		std::cout << ZD << std::endl;
	}

	mtx::MatrixC A = circuitAC.GetNodalMatrix(); // ��������� ������� ���������� A ��� ����� ����
	if (_calculationLog & LOG_A_MATRIX)
	{
		std::cout << A << std::endl;
	}

	mtx::MatrixC Y = ZD.GetReverse(); // ��������� ������� ������������ Y �� ������� ZD
	if (_calculationLog & LOG_G_MATRIX)
	{
		std::cout << Y << std::endl;
	}

	mtx::MatrixC F = (A * Y * A.GetTransposed()).GetReverse() * (-A * Y * E - A * J); // ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
	if (_calculationLog & LOG_F_MATRIX)
	{
		std::cout << F << std::endl;
	}

	mtx::MatrixC U = A.GetTransposed() * F; // ��������� ���������� �� ���� ������ ����
	if (_calculationLog & LOG_U_MATRIX)
	{
		std::cout << U << std::endl;
	}

	mtx::MatrixC IR = Y * (U + E); // ��������� ���� � �������������� ������

	return IR;
}

// ����� ��������� �������
char CircuitCalculator::GetCalculationLog() noexcept
{
	return _calculationLog;
}

// ����� ��������� �������
void CircuitCalculator::SetCalculateLog(const char& calculationLog) noexcept
{
	_calculationLog = calculationLog;
}

}