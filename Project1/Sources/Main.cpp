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


int main()
{
	// указываем файл с данными электрической цепи
	ReaderTXT reader("Resources/CircuitData3.txt");
		
	// читаем исходные данные и заносим их в электрическую цепь
	CircuitAC circuitAC(reader.Read());
		
	// расчитываем электрическую цепь
	MatrixC IR = CircuitCalculator::CalculateCircuit(circuitAC);

	// выводим результат в консоль
	Complex::SetOutForm(OutForm::OUT_EXP);
	cout << "Currents in branches:" << endl;
	cout << IR << endl;


	return 0;
}