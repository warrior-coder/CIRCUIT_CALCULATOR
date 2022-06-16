#include "Matrix.hpp"
#include "CircuitDC.hpp"

namespace cc
{
	class CircuitCalculator
	{
	public:

		// функция расчета электрической цепи методом узловых потенциалов
		static MatrixF CalculateCircuit(const CircuitDC& circuitDC);

	};
}

namespace cc
{
	// функция расчета электрической цепи методом узловых потенциалов
	MatrixF CircuitCalculator::CalculateCircuit(const CircuitDC& circuitDC)
	{
		// задаем матрицы с исходными данными
		MatrixF R = circuitDC.GetResistorsMatrix();
		MatrixF E = circuitDC.GetVoltageSourcesMatrix();
		MatrixF J = circuitDC.GgetCurrentSourcesMatrix();

		// формируем диагональную матрицу RD из матрицы R
		MatrixF RD = diag(R);

		// формируем матрицу соединений A для графа цепи
		MatrixF NodesMatrix = circuitDC.GetNodesMatrix();

		// формируем матрицу проводимости G из матрицы RD
		MatrixF G = RD.rev();

		// вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
		MatrixF F = (NodesMatrix * G * NodesMatrix.transp()).rev() * (-NodesMatrix * G * E - NodesMatrix * J);

		// вычисляем напряжение на всех ветвях цепи
		MatrixF U = NodesMatrix.transp() * F;

		// вычисляем токи в сопротивлениях ветвей
		MatrixF IR = G * (U + E);

		return IR;
	}
}
