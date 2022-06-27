#include <iostream>
#include "MATRIX/Matrix.hpp" // подключаем библиотеку "Matrix.hpp" для работы с матрицами
#include "ReaderTXT.hpp"
#include "CircuitDC.hpp"
#include "CircuitCalculator.hpp"

using namespace std;
using namespace mtx;
using namespace cc;


int main()
{
	try
	{
		// указываем файл с данными электрической цепи
		ReaderTXT reader("Resources/CircuitData1.txt");
		
		// читаем исходные данные и заносим их в электрическую цепь
		CircuitDC circuitDC(reader.Read());
		
		// расчитываем электрическую цепь
		MatrixD IR = CircuitCalculator::CalculateCircuit(circuitDC);

		// выводим результат в консоль
		cout << "Currents in branches:" << endl;
		cout << IR << endl;
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}

	return 0;
}