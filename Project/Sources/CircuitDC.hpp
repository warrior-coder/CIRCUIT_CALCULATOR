// файл "circuit.h"
// в данном файле описан класс CircuitDC для работы с графом электрической цепи


// защита от повторного включения заголовочного файла
#pragma once


// подключение необходимых библиотек
#include <iostream>
#include <fstream>
#include <vector>

#include "Matrix.hpp"


namespace cc
{

// класс электрическая цепь
class CircuitDC
{
	// структура ветви ориентированный графа
	// определяет направление ветви графа индексами начального и конечного узлов 
	typedef struct _GraphBranch
	{
		int begin;
		int end;
	}GraphBranch;

	int branchesCount;							// количество ветвей цепи
	int nodesCount;								// количество узлов цепи
	std::vector<GraphBranch> circuitBranches;	// ориентированный граф ветвей
	std::vector<double> RValues;					// массив значений сопротивлений
	std::vector<double> EValues;					// массив значений напряжений
	std::vector<double> JValues;					// массив значений токов

public:
	// конструктор
	explicit CircuitDC(const std::string&);

	// деструктор по умолчанию
	~CircuitDC() = default;

	// методы получение матриц для исходной цепи
	MatrixF GetNodesMatrix() const;
	MatrixF GetCurrentSourcesMatrix() const;
	MatrixF GetVoltageSourcesMatrix() const;
	MatrixF GetResistorsMatrix() const;
};

}


namespace cc
{

	// конструктор
	// считывает исходные данные цепи из файла и на их основе формирует граф электрической цепи
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

	// создает топологическую узловую матрицу по графу исходной цепи
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

	// создает матрицу источников тока для исходной цепи 
	MatrixF CircuitDC::GetCurrentSourcesMatrix() const
	{
		MatrixF J(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			J.at(i, 0) = JValues[i];
		}

		return J;
	}

	// создает матрицу источников напряжения для исходной цепи
	MatrixF CircuitDC::GetVoltageSourcesMatrix() const
	{
		MatrixF E(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			E.at(i, 0) = EValues[i];
		}

		return E;
	}

	// создает матрицу сопротивлений для исходной цепи
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