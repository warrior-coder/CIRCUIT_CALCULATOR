#include <iostream>
#include "COMPLEX/Complex.hpp" // ?????????? ?????????? "Complex.hpp" ??? ?????? ? ???????????? ???????
#include "MATRIX/Matrix.hpp" // ?????????? ?????????? "Matrix.hpp" ??? ?????? ? ?????????
#include "CircuitCalculator.hpp" // ?????????? ?????????? "CircuitCalculator.hpp" ??? ??????? ????????????? ?????

using namespace std;
using namespace mtx;
using namespace cpx;
using namespace cc;


// ?????? ????????? ??????? ???? ??????????? ????
void ProgramDC()
{
	ReaderTXT reader("Resources/CircuitData1.txt"); // ????????? ???? ? ??????? ????????????? ????

	CircuitDC circuitDC(reader.Read()); // ?????? ???????? ?????? ? ??????? ?? ? ????????????? ????

	CircuitCalculator::SetCalculateLog(LOG_E_MATRIX | LOG_J_MATRIX | LOG_RD_MATRIX | LOG_A_MATRIX);  // ???????? ??????????? ????????????? ??????????
	MatrixD IR = CircuitCalculator::CalculateCircuit(circuitDC); // ??????????? ????????????? ????

	cout << "Currents in branches:\n" << IR << endl; // ??????? ????????? ? ???????
}

// ?????? ????????? ??????? ???? ??????????? ????
void ProgramAC()
{
	ReaderTXT reader("Resources/CircuitData3.txt");
	
	CircuitAC circuitAC(reader.Read());

	CircuitCalculator::SetCalculateLog(LOG_NO); // ????????? ??????????? ????????????? ??????????
	MatrixC IR = CircuitCalculator::CalculateCircuit(circuitAC);

	Complex::SetOutForm(OutForm::OUT_EXP); // ????????????? ???????????????? ????? ?????? ??????????? ?????
	cout << "Currents in branches:\n" << IR << endl;
}


int main()
{
	ProgramDC();


	return 0;
}