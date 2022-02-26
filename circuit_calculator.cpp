#include <iostream>

// ���������� ���� "math.h" ��� ������ � ���������
#include "circuit.h"

// ���������� ���� "circuit.h" ��� ������ � ������ ������������� ����
#include "circuit.h"


using namespace std;
using namespace matr;
using namespace circuit;


int main()
{
	// ������������� ����� �������
	setlocale(LC_ALL, "ru");
	cout.precision(3);
	cout.setf(ios::fixed);


	// ������ �������� ������ ������������� ���� �� �����
	Circuit circuit("circuit_data.txt");

	// ����������� ������������� ����
	Matr IR = calculateCircuit(circuit);
	
	// ������� ��������� � �������
	cout << "���� � �������������� ������, ��" << endl;
	cout << IR.transp() * 1000 << endl;


	return 0;
}