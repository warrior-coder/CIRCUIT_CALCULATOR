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
	try
	{
		// ��������� ���� � ������� ������������� ����
		ReaderTXT reader("Resources/CircuitData1.txt");
		
		// ������ �������� ������ � ������� �� � ������������� ����
		CircuitDC circuitDC(reader.Read());
		
		// ����������� ������������� ����
		MatrixD IR = CircuitCalculator::CalculateCircuit(circuitDC);

		// ������� ��������� � �������
		cout << "Currents in branches:" << endl;
		cout << IR << endl;
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}

	return 0;
}