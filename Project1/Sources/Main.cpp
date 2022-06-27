#include <iostream>
#include "COMPLEX/Complex.hpp" // ���������� ���������� "Complex.hpp" ��� ������ � ������������ �������
#include "MATRIX/Matrix.hpp" // ���������� ���������� "Matrix.hpp" ��� ������ � ���������
#include "ReaderTXT.hpp"
#include "CircuitAC.hpp"
#include "CircuitCalculator.hpp"

using namespace std;
using namespace mtx;
using namespace cpx;
using namespace cc;


int main()
{
	// ��������� ���� � ������� ������������� ����
	ReaderTXT reader("Resources/CircuitData3.txt");
		
	// ������ �������� ������ � ������� �� � ������������� ����
	CircuitAC circuitAC(reader.Read());
		
	// ����������� ������������� ����
	MatrixC IR = CircuitCalculator::CalculateCircuit(circuitAC);

	// ������� ��������� � �������
	Complex::SetOutForm(OutForm::OUT_EXP);
	cout << "Currents in branches:" << endl;
	cout << IR << endl;


	return 0;
}