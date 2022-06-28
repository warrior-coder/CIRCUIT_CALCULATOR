// файл "CircuitDC.hpp"
// в данном файле описан класс CircuitDC характеризующий электрическую цепь постоянного тока

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами


namespace cc
{

// обобщенная ветвь может быть представлена последовательным соединением сопротивления (R)
// и источника ЭДС (E) с включенным параллельно им источником тока (J)
//
//    +------(J)--------+
//    |                 |   I,U
// ---+---(E)---[ R ]---+---->---
//
typedef struct _CircuitDCBranch // структура обобщенной ветви электрической цепи
{
	size_t beginNode;	// индекс начального узла
	size_t endNode;		// индекс конечного узла
	double RValue;		// значение сопротивления ветви
	double EValue;		// значение источника напряжения ветви
	double JValue;		// значение источника тока ветви
}CircuitDCBranch;


class CircuitDC // класс электрическая цепь постоянного тока
{
private:
	size_t _branchesCount;							// количество ветвей цепи
	size_t _nodesCount;								// количество узлов цепи
	std::vector<CircuitDCBranch> _circuitBranches;	// ориентированный граф ветвей

public:
	CircuitDC() = delete; // конструктор по умолчанию отсутствует
	explicit CircuitDC(const std::vector<std::string>& data); // конструктор от массива строк данных (данные в строках должны быть разделены пробелами)

	CircuitDC(const CircuitDC&) = default; // конструктор копирования по умолчанию
	CircuitDC(CircuitDC&&) noexcept = default; // конструктор перемещения по умолчанию

	~CircuitDC() = default; // деструктор по умолчанию

	size_t GetBranchesCount() const noexcept; // методы получения свойств
	size_t GetNodesCount() const noexcept; 
	
	// в матрице соединений A высота равна числу узлов без одного (базисного), а ширина – числу ветвей графа
	// элемент матрицы Aij равен
	//  0, если ветвь j не принадлежит узлу i
	//  1, если ветвь j выходит из узла i
	// -1, если ветвь j входит в узел i
	mtx::MatrixD GetNodalMatrix() const; // создает топологическую узловую матрицу по графу исходной цепи

	mtx::MatrixD GetCurrentSourcesMatrix() const; // создает матрицу источников тока для исходной цепи 
	mtx::MatrixD GetVoltageSourcesMatrix() const; // создает матрицу источников напряжения для исходной цепи
	mtx::MatrixD GetResistorsMatrix() const; // создает матрицу сопротивлений для исходной цепи
};

}


// реализован класс CircuitDC характеризующий электрическую цепь постоянного тока
namespace cc
{

// конструктор от массива строк данных (данные в строках должны быть разделены пробелами)
CircuitDC::CircuitDC(const std::vector<std::string>& data)
	: _branchesCount(0u)
	, _nodesCount(0u)
{

	CircuitDCBranch circuitBranch{};

	for (const auto& line : data)
	{
		std::istrstream iss(line.c_str());

		iss >> _branchesCount
			>> circuitBranch.beginNode >> circuitBranch.endNode
			>> circuitBranch.RValue
			>> circuitBranch.EValue
			>> circuitBranch.JValue;

		_nodesCount = std::max(_nodesCount, circuitBranch.beginNode);
		_nodesCount = std::max(_nodesCount, circuitBranch.endNode);
		circuitBranch.beginNode -= 1;
		circuitBranch.endNode -= 1;

		_circuitBranches.push_back(circuitBranch);
	}
}

// методы получения свойств
size_t CircuitDC::GetBranchesCount() const noexcept
{
	return _branchesCount;
}

size_t CircuitDC::GetNodesCount() const noexcept
{
	return _nodesCount;
}

// создает топологическую узловую матрицу по графу исходной цепи
mtx::MatrixD CircuitDC::GetNodalMatrix() const
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

// создает матрицу источников тока для исходной цепи 
mtx::MatrixD CircuitDC::GetCurrentSourcesMatrix() const
{
	mtx::MatrixD J(_branchesCount, 1, 0.0);

	for (size_t i = 0; i < J.GetRows(); ++i)
	{
		J.at(i, 0) = _circuitBranches[i].JValue;
	}

	return J;
}

// создает матрицу источников напряжения для исходной цепи
mtx::MatrixD CircuitDC::GetVoltageSourcesMatrix() const
{
	mtx::MatrixD E(_branchesCount, 1, 0.0);

	for (size_t i = 0; i < E.GetRows(); ++i)
	{
		E.at(i, 0) = _circuitBranches[i].EValue;
	}

	return E;
}

// создает матрицу сопротивлений для исходной цепи
mtx::MatrixD CircuitDC::GetResistorsMatrix() const
{
	mtx::MatrixD R(_branchesCount, 1, 0.0);

	for (size_t i = 0; i < R.GetRows(); ++i)
	{
		R.at(i, 0) = _circuitBranches[i].RValue;
	}

	return R;
}

}