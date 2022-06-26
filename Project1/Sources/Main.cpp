#include <iostream>
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������
#include "ReaderTXT.hpp"
#include "CircuitDC.hpp"
#include "CircuitCalculator.hpp"

using namespace std;
using namespace mtx;
using namespace cc;


int main()
{
	// ������������� ����� �������
	setlocale(LC_ALL, "ru");
	cout.precision(4);
	cout.setf(ios::fixed);

	try
	{
		ReaderTXT reader{ "Resources/CircuitData1.txt" };

		// ������ �������� ������ ������������� ���� �� ����� 
		CircuitDC circuit{ reader.Read() };

		// ����������� ������������� ����
		MatrixD matrix{ CircuitCalculator::CalculateCircuit(circuit) };

		// ������� ��������� � �������
		cout << "���� � �������������� ������, �" << endl;
		cout << matrix << endl;
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}


	return 0;
}