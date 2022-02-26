#include <iostream>

// подключаем файл "math.h" для работы с матрицами
#include "circuit.h"

// подключаем файл "circuit.h" для работы с графом электрической цепи
#include "circuit.h"


using namespace std;
using namespace matr;
using namespace circuit;


int main()
{
	// устанавливаем режим консоли
	setlocale(LC_ALL, "ru");
	cout.precision(3);
	cout.setf(ios::fixed);


	// читаем исходные данные электрической цепи из файла
	Circuit circuit("circuit_data.txt");

	// расчитываем электрическую цепь
	Matr IR = calculateCircuit(circuit);
	
	// выводим результат в консоль
	cout << "Токи в сопротивлениях ветвей, мА" << endl;
	cout << IR.transp() * 1000 << endl;


	return 0;
}