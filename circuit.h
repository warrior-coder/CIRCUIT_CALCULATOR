// ���� "circuit.h"
// � ������ ����� ������ ����� Circuit ��� ������ � ������ ������������� ����


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include "matr.h"
#include <iostream>
#include <fstream>
#include <vector>


namespace circuit
{

// ����� ������������� ����
class Circuit
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
	std::vector<float> RValues;					// ������ �������� �������������
	std::vector<float> EValues;					// ������ �������� ����������
	std::vector<float> JValues;					// ������ �������� �����

public:
	// �����������
	explicit Circuit(const std::string&);

	// ���������� �� ���������
	~Circuit() = default;

	// ������ ��������� ������ ��� �������� ����
	matr::Matr getNodesMatrix() const;
	matr::Matr getCurrentSourcesMatrix() const;
	matr::Matr getVoltageSourcesMatrix() const;
	matr::Matr getResistorsMatrix() const;
};

// ������� ������� ������������� ���� ������� ������� �����������
matr::Matr calculateCircuit(const Circuit&);

}