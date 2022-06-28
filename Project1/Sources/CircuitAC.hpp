// ���� "CircuitAC.hpp"
// � ������ ����� ������ ����� CircuitAC ��������������� ������������� ���� ����������� ����

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include "COMPLEX/Complex.hpp" // ���������� ���������� "Complex.hpp" ��� ������ � ������������ �������
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������


namespace cc
{

// ���������� ����� ����� ���� ������������ ���������������� ����������� ������������� (R)
// � ��������� ��� (E) � ���������� ����������� �� ���������� ���� (J)
//
//    +------(J)--------+
//    |                 |   I,U
// ---+---(E)---[ Z ]---+---->---
//
// ��� Z = R + jXL - jXC
//
typedef struct _CircuitACBranch // ��������� ���������� ����� ������������� ����
{
	size_t beginNode;		// ������ ���������� ����
	size_t endNode;			// ������ ��������� ����
	cpx::Complex ZValue;	// �������� ������ �����
	cpx::Complex EValue;	// �������� ��������� ���������� �����
	cpx::Complex JValue;	// �������� ��������� ���� �����
}CircuitACBranch;


class CircuitAC // ����� ������������� ���� ����������� ����
{
private:
	size_t _branchesCount;							// ���������� ������ ����
	size_t _nodesCount;								// ���������� ����� ����
	std::vector<CircuitACBranch> _circuitBranches;	// ��������������� ���� ������

public:
	CircuitAC() = delete; // ����������� �� ��������� �����������
	explicit CircuitAC(const std::vector<std::string>& data); // ����������� �� ������� ����� ������ (������ � ������� ������ ���� ��������� ���������)

	CircuitAC(const CircuitAC&) = default; // ����������� ����������� �� ���������
	CircuitAC(CircuitAC&&) noexcept = default; // ����������� ����������� �� ���������

	~CircuitAC() = default; // ���������� �� ���������

	size_t GetBranchesCount() const noexcept; // ������ ��������� �������
	size_t GetNodesCount() const noexcept;

	// � ������� ���������� A ������ ����� ����� ����� ��� ������ (���������), � ������ � ����� ������ �����
	// ������� ������� Aij �����
	//  0, ���� ����� j �� ����������� ���� i
	//  1, ���� ����� j ������� �� ���� i
	// -1, ���� ����� j ������ � ���� i
	mtx::MatrixC GetNodalMatrix() const; // ������� �������������� ������� ������� �� ����� �������� ����
	
	mtx::MatrixC GetCurrentSourcesMatrix() const; // ������� ������� ���������� ���� ��� �������� ���� 
	mtx::MatrixC GetVoltageSourcesMatrix() const; // ������� ������� ���������� ���������� ��� �������� ����
	mtx::MatrixC GetResistorsMatrix() const; // ������� ������� ������������� ��� �������� ����
};

}

namespace cc
{

// ����������� �� ������� ����� ������ (������ � ������� ������ ���� ��������� ���������)
CircuitAC::CircuitAC(const std::vector<std::string>& data)
	: _branchesCount(0u)
	, _nodesCount(0u)
{
	CircuitACBranch circuitBranch{};
	double RValue{}, XLValue{}, XCValue{};
	double EValueMod{}, EValueArg{}, JValueMod{}, JValueArg{};

	for (const auto& line : data)
	{
		std::istrstream iss(line.c_str());

		iss >> _branchesCount
			>> circuitBranch.beginNode >> circuitBranch.endNode
			>> RValue >> XLValue >> XCValue
			>> EValueMod >> EValueArg
			>> JValueMod >> JValueArg;

		_nodesCount = std::max(_nodesCount, circuitBranch.beginNode);
		_nodesCount = std::max(_nodesCount, circuitBranch.endNode);
		circuitBranch.beginNode -= 1;
		circuitBranch.endNode -= 1;
		circuitBranch.ZValue = cpx::Complex(RValue) + cpx::Complex(0.0, 1.0) * cpx::Complex(XLValue) - cpx::Complex(0.0, 1.0) * cpx::Complex(XCValue);
		circuitBranch.EValue = cpx::Complex(EValueMod, EValueArg, cpx::EXP);
		circuitBranch.JValue = cpx::Complex(JValueMod, JValueArg, cpx::EXP);

		_circuitBranches.push_back(circuitBranch);
	}
}

// ������ ��������� �������
size_t CircuitAC::GetBranchesCount() const noexcept
{
	return _branchesCount;
}

size_t CircuitAC::GetNodesCount() const noexcept
{
	return _nodesCount;
}

// ������� �������������� ������� ������� �� ����� �������� ����
mtx::MatrixC CircuitAC::GetNodalMatrix() const
{
	mtx::MatrixC A(_nodesCount - 1, _branchesCount, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < A.GetRows(); ++i)
	{
		for (size_t j = 0; j < A.GetColumns(); ++j)
		{
			if (_circuitBranches[j].beginNode == i)
			{
				A.at(i, j) = cpx::Complex(1.0, 0.0);
			}
			else if (_circuitBranches[j].endNode == i)
			{
				A.at(i, j) = cpx::Complex(-1.0, 0.0);
			}
			else
			{
				A.at(i, j) = cpx::Complex(0.0, 0.0);
			}
		}
	}

	return A;
}

// ������� ������� ���������� ���� ��� �������� ���� 
mtx::MatrixC CircuitAC::GetCurrentSourcesMatrix() const
{
	mtx::MatrixC J(_branchesCount, 1, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < J.GetRows(); ++i)
	{
		J.at(i, 0) = _circuitBranches[i].JValue;
	}

	return J;
}

// ������� ������� ���������� ���������� ��� �������� ����
mtx::MatrixC CircuitAC::GetVoltageSourcesMatrix() const
{
	mtx::MatrixC E(_branchesCount, 1, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < E.GetRows(); ++i)
	{
		E.at(i, 0) = _circuitBranches[i].EValue;
	}

	return E;
}

// ������� ������� ������������� ��� �������� ����
mtx::MatrixC CircuitAC::GetResistorsMatrix() const
{
	mtx::MatrixC R(_branchesCount, 1, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < R.GetRows(); ++i)
	{
		R.at(i, 0) = _circuitBranches[i].ZValue;
	}

	return R;
}

}