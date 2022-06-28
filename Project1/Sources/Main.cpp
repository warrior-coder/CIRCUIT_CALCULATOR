#include <iostream>
#include "COMPLEX/Complex.hpp" // подключаем библиотеку "Complex.hpp" для работы с комплексными числами
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами
#include "ReaderTXT.hpp"
#include "CircuitAC.hpp"
#include "CircuitCalculator.hpp"

using namespace std;
using namespace mtx;
using namespace cpx;
using namespace cc;


// пример программы расчета цепи постоянного тока
void ProgramDC()
{
	ReaderTXT reader("Resources/CircuitData1.txt"); // указываем файл с данными электрической цепи

	CircuitDC circuitDC(reader.Read()); // читаем исходные данные и заносим их в электрическую цепь

	CircuitCalculator::SetCalculateLog(LOG_R_MATRIX | LOG_E_MATRIX | LOG_J_MATRIX);
	MatrixD IR = CircuitCalculator::CalculateCircuit(circuitDC); // расчитываем электрическую цепь

	cout << "Currents in branches:\n" << IR << endl; // выводим результат в консоль
}

// пример программы расчета цепи переменного тока
void ProgramAC()
{
	ReaderTXT reader("Resources/CircuitData3.txt"); // указываем файл с данными электрической цепи
	
	CircuitAC circuitAC(reader.Read()); // читаем исходные данные и заносим их в электрическую цепь

	MatrixC IR = CircuitCalculator::CalculateCircuit(circuitAC); // расчитываем электрическую цепь

	Complex::SetOutForm(OutForm::OUT_EXP);
	cout << "Currents in branches:\n" << IR << endl; // выводим результат в консоль
}

int main()
{
	ProgramDC();

	return 0;
}