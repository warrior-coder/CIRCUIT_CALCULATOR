#include <iostream>
#include "COMPLEX/Complex.hpp" // подключаем библиотеку "Complex.hpp" для работы с комплексными числами
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами
#include "CircuitCalculator.hpp" // подключаем библиотеку "CircuitCalculator.hpp" для расчета электрических цепей

using namespace std;
using namespace mtx;
using namespace cpx;
using namespace cc;


// пример программы расчета цепи постоянного тока
void ProgramDC()
{
	ReaderTXT reader("Resources/CircuitData1.txt"); // указываем файл с данными электрической цепи

	CircuitDC circuitDC(reader.Read()); // читаем исходные данные и заносим их в электрическую цепь

	CircuitCalculator::SetCalculateLog(LOG_E_MATRIX | LOG_J_MATRIX | LOG_RD_MATRIX | LOG_A_MATRIX);  // включаем логирование промежуточных вычислений
	MatrixD IR = CircuitCalculator::CalculateCircuit(circuitDC); // расчитываем электрическую цепь

	cout << "Currents in branches:\n" << IR << endl; // выводим результат в консоль
}

// пример программы расчета цепи переменного тока
void ProgramAC()
{
	ReaderTXT reader("Resources/CircuitData3.txt");
	
	CircuitAC circuitAC(reader.Read());

	CircuitCalculator::SetCalculateLog(LOG_NO); // отключаем логирование промежуточных вычислений
	MatrixC IR = CircuitCalculator::CalculateCircuit(circuitAC);

	Complex::SetOutForm(OutForm::OUT_EXP); // устанавливаем экспоненциальную форму вывода комплексных числе
	cout << "Currents in branches:\n" << IR << endl;
}


int main()
{
	ProgramDC();


	return 0;
}