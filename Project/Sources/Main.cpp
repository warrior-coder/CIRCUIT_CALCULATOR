#include <iostream>

// подключаем файл "math.h" для работы с матрицами
#include "Matrix.hpp"

// подключаем файл "circuit.h" для работы с графом электрической цепи
//#include "CircuitDC.hpp"
//#include "CircuitCalculator.hpp"


using namespace std;
using namespace cc;


int main()
{
	/*	// устанавливаем режим консоли
	setlocale(LC_ALL, "ru");
	cout.precision(3);
	cout.setf(ios::fixed);


	// читаем исходные данные электрической цепи из файла
	CircuitDC circuit("Resources/circuit_data.txt");

	// расчитываем электрическую цепь
	MatrixF IR = CircuitCalculator::CalculateCircuit(circuit);

	// выводим результат в консоль
	cout << "Токи в сопротивлениях ветвей, А" << endl;
	cout << IR.transp() << endl;
	*/
	try
	{
		/*MatrixF m1({
			{ 2, 2 },
			{ 1, 1 }
		});

		MatrixF m2({
			{ 4, 4 },
			{ 3, 3 }
		});

		cout << m1 + m2 << endl;
		cout << m1 - m2 << endl;
		cout << m1 * m2 << endl;
		cout << -m2 << endl;
		cout << m2 * 10 << endl;
		cout << -m2 * 10 << endl;*/
		
		MatrixF m1({
			1,
			2,
			3,
			4,
			5,
			6,
			7,
			8,
			9,
			100
		});

		cout << m1.Transpose().Clear(999) << endl;


	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}

	

	return 0;
}