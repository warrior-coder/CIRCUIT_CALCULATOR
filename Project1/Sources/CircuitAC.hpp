// файл "CircuitAC.hpp"
// в данном файле описан класс CircuitAC характеризующий электрическую цепь переменного тока

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include "COMPLEX/Complex.hpp" // подключаем библиотеку "Complex.hpp" для работы с комплексными числами
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами


namespace cc
{

// обобщенная ветвь может быть представлена последовательным соединением сопротивления (R)
// и источника ЭДС (E) с включенным параллельно им источником тока (J)
//
//    +------(J)--------+
//    |                 |   I,U
// ---+---(E)---[ Z ]---+---->---
//
// где Z = R + jXL - jXC
//
typedef struct _CircuitACBranch // структура обобщенной ветви электрической цепи
{
	size_t beginNode;		// индекс начального узла
	size_t endNode;			// индекс конечного узла
	cpx::Complex ZValue;	// значение общего ветви
	cpx::Complex EValue;	// значение источника напряжения ветви
	cpx::Complex JValue;	// значение источника тока ветви
}CircuitACBranch;


class CircuitAC // класс электрическая цепь постоянного тока
{
private:
	size_t _branchesCount;							// количество ветвей цепи
	size_t _nodesCount;								// количество узлов цепи
	std::vector<CircuitACBranch> _circuitBranches;	// ориентированный граф ветвей

public:
	CircuitAC() = delete; // конструктор по умолчанию отсутствует
	explicit CircuitAC(const std::vector<std::string>& data); // конструктор от массива строк данных (данные в строках должны быть разделены пробелами)

	CircuitAC(const CircuitAC&) = default; // конструктор копирования по умолчанию
	CircuitAC(CircuitAC&&) noexcept = default; // конструктор перемещения по умолчанию

	~CircuitAC() = default; // деструктор по умолчанию

	size_t GetBranchesCount() const noexcept; // методы получения свойств
	size_t GetNodesCount() const noexcept;

	// в матрице соединений A высота равна числу узлов без одного (базисного), а ширина – числу ветвей графа
	// элемент матрицы Aij равен
	//  0, если ветвь j не принадлежит узлу i
	//  1, если ветвь j выходит из узла i
	// -1, если ветвь j входит в узел i
	mtx::MatrixC GetNodalMatrix() const; // создает топологическую узловую матрицу по графу исходной цепи
	
	mtx::MatrixC GetCurrentSourcesMatrix() const; // создает матрицу источников тока для исходной цепи 
	mtx::MatrixC GetVoltageSourcesMatrix() const; // создает матрицу источников напряжения для исходной цепи
	mtx::MatrixC GetResistorsMatrix() const; // создает матрицу сопротивлений для исходной цепи
};

}

namespace cc
{

// конструктор от массива строк данных (данные в строках должны быть разделены пробелами)
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

// методы получения свойств
size_t CircuitAC::GetBranchesCount() const noexcept
{
	return _branchesCount;
}

size_t CircuitAC::GetNodesCount() const noexcept
{
	return _nodesCount;
}

// создает топологическую узловую матрицу по графу исходной цепи
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

// создает матрицу источников тока для исходной цепи 
mtx::MatrixC CircuitAC::GetCurrentSourcesMatrix() const
{
	mtx::MatrixC J(_branchesCount, 1, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < J.GetRows(); ++i)
	{
		J.at(i, 0) = _circuitBranches[i].JValue;
	}

	return J;
}

// создает матрицу источников напряжения для исходной цепи
mtx::MatrixC CircuitAC::GetVoltageSourcesMatrix() const
{
	mtx::MatrixC E(_branchesCount, 1, cpx::Complex(0.0, 0.0));

	for (size_t i = 0; i < E.GetRows(); ++i)
	{
		E.at(i, 0) = _circuitBranches[i].EValue;
	}

	return E;
}

// создает матрицу сопротивлений для исходной цепи
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