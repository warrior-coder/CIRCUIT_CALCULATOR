// файл "circuit.h"
// в данном файле реализован класс Circuit для работы с графом электрической цепи


#include "circuit.h"


namespace circuit
{

// конструктор
// считывает исходные данные цепи из файла и на их основе формирует граф электрической цепи
Circuit::Circuit(const std::string& path)
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

// создает топологическую узловую матрицу по графу исходной цепи
matr::Matr Circuit::getNodesMatrix() const
{
	matr::Matr nodesMartix(nodesCount - 1, branchesCount);

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

// создает матрицу источников тока для исходной цепи 
matr::Matr Circuit::getCurrentSourcesMatrix() const
{
	matr::Matr J(branchesCount, 1);

	for (int i = 0; i < branchesCount; i++)
	{
		J.at(i, 0) = JValues[i];
	}

	return J;
}

// создает матрицу источников напряжения для исходной цепи
matr::Matr Circuit::getVoltageSourcesMatrix() const
{
	matr::Matr E(branchesCount, 1);

	for (int i = 0; i < branchesCount; i++)
	{
		E.at(i, 0) = EValues[i];
	}

	return E;
}

// создает матрицу сопротивлений для исходной цепи
matr::Matr Circuit::getResistorsMatrix() const
{
	matr::Matr R(branchesCount, 1);

	for (int i = 0; i < branchesCount; i++)
	{
		R.at(i, 0) = RValues[i] < 1.0e-10 ? 1.0e-10 : RValues[i];
	}

	return R;
}

// функция расчета электрической цепи методом узловых потенциалов
matr::Matr calculateCircuit(const Circuit& circuit)
{
	// задаем матрицы с исходными данными
	matr::Matr R = circuit.getResistorsMatrix();
	matr::Matr E = circuit.getVoltageSourcesMatrix();
	matr::Matr J = circuit.getCurrentSourcesMatrix();

	// формируем диагональную матрицу RD из матрицы R
	matr::Matr RD = diag(R);

	// формируем матрицу соединений A для графа цепи
	matr::Matr NodesMatrix = circuit.getNodesMatrix();

	// формируем матрицу проводимости G из матрицы RD
	matr::Matr G = RD.rev();

	// вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
	matr::Matr F = (NodesMatrix * G * NodesMatrix.transp()).rev() * (-NodesMatrix * G * E - NodesMatrix * J);

	// вычисляем напряжение на всех ветвях цепи
	matr::Matr U = NodesMatrix.transp() * F;

	// вычисляем токи в сопротивлениях ветвей
	matr::Matr IR = G * (U + E);

	return IR;
}

}