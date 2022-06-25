// ���� "circuit.h"
// � ������ ����� ������ ����� CircuitDC ��� ������ � ������ ������������� ����


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include <iostream>
#include <fstream>
#include <vector>

#include "Matrix.hpp"


namespace cc
{

// ����� ������������� ����
class CircuitDC
{
	// ��������� ����� ��������������� �����
	// ���������� ����������� ����� ����� ��������� ���������� � ��������� ����� 
	typedef struct _GraphBranch
	{
		int begin;
		int end;
	}GraphBranch;

	int branchesCount;							// ���������� ������ ����
	int nodesCount;								// ���������� ����� ����
	std::vector<GraphBranch> circuitBranches;	// ��������������� ���� ������
	std::vector<double> RValues;					// ������ �������� �������������
	std::vector<double> EValues;					// ������ �������� ����������
	std::vector<double> JValues;					// ������ �������� �����

public:
	// �����������
	explicit CircuitDC(const std::string&);

	// ���������� �� ���������
	~CircuitDC() = default;

	// ������ ��������� ������ ��� �������� ����
	MatrixF GetNodesMatrix() const;
	MatrixF GetCurrentSourcesMatrix() const;
	MatrixF GetVoltageSourcesMatrix() const;
	MatrixF GetResistorsMatrix() const;
};

}


namespace cc
{

	// �����������
	// ��������� �������� ������ ���� �� ����� � �� �� ������ ��������� ���� ������������� ����
	CircuitDC::CircuitDC(const std::string& path)
		: branchesCount(0)
		, nodesCount(0)
	{
		std::ifstream ifl(path);

		int valuei, branchBegin, branchEnd;
		double valuef;
		char skipLine[256];

		ifl.getline(skipLine, 256);
		ifl.getline(skipLine, 256);

		while (!ifl.eof())
		{
			ifl >> branchesCount;

			ifl >> valuei;
			if ((branchBegin = valuei / 10) > nodesCount) nodesCount = branchBegin;
			if ((branchEnd = valuei % 10) > nodesCount) nodesCount = branchEnd;
			circuitBranches.push_back({ branchBegin - 1, branchEnd - 1 });

			ifl >> valuef;
			RValues.push_back(valuef);

			ifl >> valuef;
			EValues.push_back(valuef);

			ifl >> valuef;
			JValues.push_back(valuef);
		}

		ifl.close();
	}

	// ������� �������������� ������� ������� �� ����� �������� ����
	MatrixF CircuitDC::GetNodesMatrix() const
	{
		MatrixF nodesMartix(nodesCount - 1, branchesCount);

		for (int i = 0; i < nodesMartix.getH(); i++)
		{
			for (int j = 0; j < nodesMartix.getW(); j++)
			{
				if (circuitBranches[j].begin == i) nodesMartix.at(i, j) = 1;
				else if (circuitBranches[j].end == i) nodesMartix.at(i, j) = -1;
				else nodesMartix.at(i, j) = 0;
			}
		}

		return nodesMartix;
	}

	// ������� ������� ���������� ���� ��� �������� ���� 
	MatrixF CircuitDC::GetCurrentSourcesMatrix() const
	{
		MatrixF J(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			J.at(i, 0) = JValues[i];
		}

		return J;
	}

	// ������� ������� ���������� ���������� ��� �������� ����
	MatrixF CircuitDC::GetVoltageSourcesMatrix() const
	{
		MatrixF E(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			E.at(i, 0) = EValues[i];
		}

		return E;
	}

	// ������� ������� ������������� ��� �������� ����
	MatrixF CircuitDC::GetResistorsMatrix() const
	{
		MatrixF R(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			R.at(i, 0) = RValues[i] < 1.0e-10 ? 1.0e-10 : RValues[i];
		}

		return R;
	}


}