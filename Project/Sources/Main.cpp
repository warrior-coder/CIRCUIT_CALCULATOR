#include <iostream>

// ���������� ���� "math.h" ��� ������ � ���������
#include "Matrix.hpp"

// ���������� ���� "circuit.h" ��� ������ � ������ ������������� ����
//#include "CircuitDC.hpp"
//#include "CircuitCalculator.hpp"


using namespace std;
using namespace cc;


int main()
{
	/*	// ������������� ����� �������
	setlocale(LC_ALL, "ru");
	cout.precision(3);
	cout.setf(ios::fixed);


	// ������ �������� ������ ������������� ���� �� �����
	CircuitDC circuit("Resources/circuit_data.txt");

	// ����������� ������������� ����
	MatrixF IR = CircuitCalculator::CalculateCircuit(circuit);

	// ������� ��������� � �������
	cout << "���� � �������������� ������, �" << endl;
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