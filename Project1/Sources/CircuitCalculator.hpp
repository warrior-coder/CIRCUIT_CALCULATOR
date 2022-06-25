#include "Matrix.hpp"
#include "CircuitDC.hpp"

namespace cc
{
	class CircuitCalculator
	{
	public:

		// ������� ������� ������������� ���� ������� ������� �����������
		static MatrixF CalculateCircuit(const CircuitDC& circuitDC);

	};
}

namespace cc
{
	// ������� ������� ������������� ���� ������� ������� �����������
	MatrixF CircuitCalculator::CalculateCircuit(const CircuitDC& circuitDC)
	{
		// ������ ������� � ��������� �������
		MatrixF R = circuitDC.GetResistorsMatrix();
		MatrixF E = circuitDC.GetVoltageSourcesMatrix();
		MatrixF J = circuitDC.GgetCurrentSourcesMatrix();

		// ��������� ������������ ������� RD �� ������� R
		MatrixF RD = diag(R);

		// ��������� ������� ���������� A ��� ����� ����
		MatrixF NodesMatrix = circuitDC.GetNodesMatrix();

		// ��������� ������� ������������ G �� ������� RD
		MatrixF G = RD.rev();

		// ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
		MatrixF F = (NodesMatrix * G * NodesMatrix.transp()).rev() * (-NodesMatrix * G * E - NodesMatrix * J);

		// ��������� ���������� �� ���� ������ ����
		MatrixF U = NodesMatrix.transp() * F;

		// ��������� ���� � �������������� ������
		MatrixF IR = G * (U + E);

		return IR;
	}
}
