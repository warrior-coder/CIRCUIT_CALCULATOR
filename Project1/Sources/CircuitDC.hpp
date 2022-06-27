// ���� "CircuitDC.hpp"
// � ������ ����� ������ ����� CircuitDC ��������������� ������������� ���� ����������� ����

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������


namespace cc
{

// ���������� ����� ����� ���� ������������ ���������������� ����������� ������������� (R)
// � ��������� ��� (E) � ���������� ����������� �� ���������� ���� (J)
//
//    +------(J)--------+
//    |                 |   I,U
// ---+---(E)---[ R ]---+---->---
//
typedef struct _CircuitBranch // ��������� ���������� ����� ������������� ����
{
	size_t beginNode;	// ������ ���������� ����
	size_t endNode;		// ������ ��������� ����
	double RValue;		// �������� ������������� �����
	double EValue;		// �������� ��������� ���������� �����
	double JValue;		// �������� ��������� ���� �����
}CircuitBranch;


// ����� ������������� ���� ����������� ����
class CircuitDC
{
private:
	size_t _branchesCount;							// ���������� ������ ����
	size_t _nodesCount;								// ���������� ����� ����
	std::vector<CircuitBranch> _circuitBranches;	// ��������������� ���� ������

public:
	CircuitDC() = delete; // ����������� �� ���������

	// ����������� �� ������� ����� ������ (������ � ������� ������ ���� ��������� ���������)
	explicit CircuitDC(const std::vector<std::string>& data)
		: _branchesCount(0u)
		, _nodesCount(0u)
	{

		for (const auto& line : data)
		{
			std::istrstream iss(line.c_str());
			CircuitBranch circuitBranch{};

			iss >> _branchesCount >> circuitBranch.beginNode >> circuitBranch.endNode >> circuitBranch.RValue >> circuitBranch.EValue >> circuitBranch.JValue;
			_nodesCount = std::max(_nodesCount, circuitBranch.beginNode);
			_nodesCount = std::max(_nodesCount, circuitBranch.endNode);
			circuitBranch.beginNode -= 1;
			circuitBranch.endNode -= 1;
			_circuitBranches.push_back(circuitBranch);
		}
	}

	CircuitDC(const CircuitDC&) = default; // ����������� ����������� �� ���������
	CircuitDC(CircuitDC&&) noexcept = default; // ����������� ����������� �� ���������

	~CircuitDC() = default; // ���������� �� ���������

	// ������ ��������� �������
	size_t GetBranchesCount() const noexcept
	{
		return _branchesCount;
	}
	
	size_t GetNodesCount() const noexcept
	{
		return _nodesCount;
	}

	// ������� �������������� ������� ������� �� ����� �������� ����
	// � ������� ���������� A ������ ����� ����� ����� ��� ������ (���������), � ������ � ����� ������ �����
	// ������� ������� Aij �����
	//  0, ���� ����� j �� ����������� ���� i
	//  1, ���� ����� j ������� �� ���� i
	// -1, ���� ����� j ������ � ���� i
	mtx::MatrixD GetNodalMatrix() const
	{
		mtx::MatrixD A(_nodesCount - 1, _branchesCount, 0.0);

		for (size_t i = 0; i < A.GetRows(); ++i)
		{
			for (size_t j = 0; j < A.GetColumns(); ++j)
			{
				if (_circuitBranches[j].beginNode == i)
				{
					A.at(i, j) = 1.0;
				}
				else if (_circuitBranches[j].endNode == i)
				{
					A.at(i, j) = -1.0;
				}
				else
				{
					A.at(i, j) = 0.0;
				}
			}
		}

		return A;
	}

	// ������� ������� ���������� ���� ��� �������� ���� 
	mtx::MatrixD GetCurrentSourcesMatrix() const
	{
		mtx::MatrixD J(_branchesCount, 1, 0.0);

		for (size_t i = 0; i < J.GetRows(); ++i)
		{
			J.at(i, 0) = _circuitBranches[i].JValue;
		}

		return J;
	}

	// ������� ������� ���������� ���������� ��� �������� ����
	mtx::MatrixD GetVoltageSourcesMatrix() const
	{
		mtx::MatrixD E(_branchesCount, 1, 0.0);

		for (size_t i = 0; i < E.GetRows(); ++i)
		{
			E.at(i, 0) = _circuitBranches[i].EValue;
		}

		return E;
	}

	// ������� ������� ������������� ��� �������� ����
	mtx::MatrixD GetResistorsMatrix() const
	{
		mtx::MatrixD R(_branchesCount, 1, 0.0);

		for (size_t i = 0; i < R.GetRows(); ++i)
		{
			R.at(i, 0) = _circuitBranches[i].RValue;
		}

		return R;
	}
};

}