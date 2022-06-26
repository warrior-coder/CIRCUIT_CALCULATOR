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
	// устанавливаем режим консоли
	setlocale(LC_ALL, "ru");
	cout.precision(4);
	cout.setf(ios::fixed);

	try
	{
		ReaderTXT reader{ "Resources/CircuitData1.txt" };

		// читаем исходные данные электрической цепи из файла 
		CircuitDC circuit{ reader.Read() };

		// расчитываем электрическую цепь
		MatrixD matrix{ CircuitCalculator::CalculateCircuit(circuit) };

		// выводим результат в консоль
		cout << "Токи в сопротивлениях ветвей, А" << endl;
		cout << matrix << endl;
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}


	return 0;
}