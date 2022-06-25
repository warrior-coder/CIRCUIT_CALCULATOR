#include <iostream>
#include "MATRIX/Matrix.hpp"
#include "COMPLEX/Complex.hpp"

using namespace std;


int main()
{
	mtx::Matrix<double> m1({
		{ 1, 2 },
		{ 3, 4 }
	});

	mtx::Matrix<cpx::Complex> m2({
		{ {1,1}, {2,2} },
		{ {3,3}, {4,4} },
	});

	cpx::Complex::SetOutForm(cpx::OutForm::OUT_EXP);

	//cout << m1.GetDeterminant() << endl;
	//cout << m2.GetDeterminant() << endl;
	cout << m2.GetReverse() << endl;
	//cout << static_cast<double>(cpx::Complex{1, 1}) << endl;

	return 0;
}