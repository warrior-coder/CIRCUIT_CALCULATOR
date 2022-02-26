// файл "circuit.h"
// в данном файле описан класс Circuit для работы с графом электрической цепи


// защита от повторного включения заголовочного файла
#pragma once


// подключение необходимых библиотек
#include "matr.h"
#include <iostream>
#include <fstream>
#include <vector>


namespace circuit
{

// класс электрическая цепь
class Circuit
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
	std::vector<float> RValues;					// массив значений сопротивлений
	std::vector<float> EValues;					// массив значений напряжений
	std::vector<float> JValues;					// массив значений токов

public:
	// конструктор
	explicit Circuit(const std::string&);

	// деструктор по умолчанию
	~Circuit() = default;

	// методы получение матриц для исходной цепи
	matr::Matr getNodesMatrix() const;
	matr::Matr getCurrentSourcesMatrix() const;
	matr::Matr getVoltageSourcesMatrix() const;
	matr::Matr getResistorsMatrix() const;
};

// функция расчета электрической цепи методом узловых потенциалов
matr::Matr calculateCircuit(const Circuit&);

}