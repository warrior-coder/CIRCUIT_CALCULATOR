// ���� "matr.cpp"
// � ������ ����� ���������� ����� Matr ��� ������ � ���������


#include "matr.h"


namespace matr
{

// � ������ ��������� ��������� ����������, �.�. ��������� �������� ��� ��������� ���������� ��� �� ��������������
// ����� ���������� Ex (���������� �� Exception) �������� ������ � ����������� �� ������
class Matr::Ex
{
	const char* info;

public:
	Ex(const char* info) : info(info) {}
	~Ex() = default;

	const char* what() const { return info; }
};


// ������������

// ������� ������� �� ���������� ������ �������������
// ��������� �������������, ����������� ���������� �������
// ������������� ������� ������� ������� ������ �� �������� �������, ��� ��������� ���������� �� ������ ������� �������� �������
Matr::Matr(std::initializer_list<std::initializer_list<float>> il)
	: h(il.size())				// ������ ������� ����� ������� ������ �������
	, w(il.begin()->size())		// ������ ������� ����� ������� ���������
	, n(h == w ? h : 0)			// ������� ������� �����������, ������ �� �������� �������
{
	auto it = il.begin(); // �������� �������� �� ������ ������

	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		if (it[i].size() != w) throw Ex("invalid size"); // ���� � ������� ������ ������ ������, �� ���������� ������

		auto jt = it[i].begin(); // �������� �������� �� ������ i-�� ���������

		buffer[i] = new float[w];

		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = jt[j]; // �������� �������� ���������
		}
	}
}

// ������� �������-������� �� ����������� ������ �������������
Matr::Matr(std::initializer_list<float> il)
	: h(il.size())			// ������ ������� ����� ������� ������
	, w(1)					// ������ ������� ����� �������
	, n(h == 1 ? 1 : 0)		// � �������-������� ����� ���� ������ ������ ������� ��� �������������
{
	auto it = il.begin();

	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		buffer[i] = new float(it[i]);
	}
}

// ������� ������� ��������� �������� � ��������� ���������� val (�� ��������� ������)
Matr::Matr(int h, int w, float val)
	: h(h)
	, w(w)
	, n(h == w ? h : 0)
{
	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		buffer[i] = new float[w];

		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = val;
		}
	}
}

// ����������� �����������
// ��-�� ������� ������������ ����� � ������ ������� ������������ �������� �����������
Matr::Matr(const Matr& m) : h(m.h), w(m.w), n(m.n)
{
	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		buffer[i] = new float[w];

		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = m.buffer[i][j];
		}
	}
}

// ����������
Matr::~Matr()
{
	for (int i = 0; i < h; i++)
	{
		delete[] buffer[i];
		buffer[i] = nullptr;
	}
	delete[] buffer;
	buffer = nullptr;
}


// ����� ���������� ���������� ������ �� ������� �������
float& Matr::at(int i, int j)
{
	return *(*(buffer + i) + j);
}


// ��������� ������������� �������� ��������� �������� ������ �������������� �������� ��� ���������

// �������� ������������ �������
Matr& Matr::operator=(const Matr& m)
{
	// ������������� ������� ����� ���� ������� �������, ������� ������ ������ ������� ���������� � �������� �����
	clear();

	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		buffer[i] = new float[w];

		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = m.buffer[i][j];
		}
	}

	return *this;
}

// �������� ��������� � ����� ������
std::ostream& operator<<(std::ostream& os, const Matr& m)
{
	for (int i = 0; i < m.h; i++)
	{
		for (int j = 0; j < m.w; j++)
		{
			os << std::setw(10) << m.buffer[i][j];
		}
		os << std::endl;
	}

	return os;
}

// �������� ��������� ������
Matr operator*(const Matr& m1, const Matr& m2)
{
	// ���� ������� �� ������������, �� ���������� ������
	if (m1.w != m2.h) throw Matr::Ex("operator*");

	Matr res(m1.h, m2.w);
	int d = m1.w;

	for (int i = 0; i < res.h; i++)
	{
		for (int j = 0; j < res.w; j++)
		{
			res.buffer[i][j] = 0;

			for (int k = 0; k < d; k++)
			{
				res.buffer[i][j] += m1.buffer[i][k] * m2.buffer[k][j];
			}
		}
	}

	return res;
}

// �������� ��������� ������� �� �����
Matr operator*(const Matr& m, float x)
{
	Matr res = m;

	for (int i = 0; i < res.h; i++)
	{
		for (int j = 0; j < res.w; j++) res.buffer[i][j] *= x;
	}

	return res;
}

// �������� �������� ������
Matr operator+(const Matr& m1, const Matr& m2)
{
	// ���� ������� �� ������ �������, �� ���������� ������
	if (m1.h != m2.h || m1.w != m2.w) throw Matr::Ex("operator+");

	Matr res(m1.h, m1.w);

	for (int i = 0; i < m1.h; i++)
	{
		for (int j = 0; j < m1.w; j++)
		{
			res.buffer[i][j] = m1.buffer[i][j] + m2.buffer[i][j];
		}
	}

	return res;
}

// �������� ��������� ������
Matr operator-(const Matr& m1, const Matr& m2)
{
	// ���� ������� �� ������ �������, �� ���������� ������
	if (m1.h != m2.h || m1.w != m2.w) throw Matr::Ex("operator-");

	Matr res(m1.h, m1.w);

	for (int i = 0; i < m1.h; i++)
	{
		for (int j = 0; j < m1.w; j++)
		{
			res.buffer[i][j] = m1.buffer[i][j] - m2.buffer[i][j];
		}
	}

	return res;
}

// �������� ��������� �������
Matr Matr::operator-()
{
	Matr res = *this;

	for (int i = 0; i < res.h; i++)
	{
		for (int j = 0; j < res.w; j++)
		{
			res.buffer[i][j] *= -1;
		}
	}

	return res;
}

// ����������� ������� ���������� ������ ������� (������ ��� ���������� ������)
float minor_r(const Matr& m, int mi, int mj)
{
	if (m.n == 0) throw Matr::Ex("minor_r()");	// ���� ������� �� ����������, �� ���������� ������

	Matr M(m.n - 1, m.n - 1);
	int it = 0, jt = 0;

	for (int i = 0; i < m.n; i++)
	{
		for (int j = 0; j < m.n; j++)
		{
			if (i != mi && j != mj) // ��������� �� ������� mi ������ � mj �������
			{
				M.buffer[it][jt] = m.buffer[i][j];
				jt++;

				if (jt == m.n - 1)
				{
					jt = 0;
					it++;
				}
			}
		}
	}

	// ������� ������������ ���������� ������� �� ������� �������
	if (M.n > 1)
	{
		float res = 0;

		for (int i = 0; i < m.n - 1; i++)
		{
			res += powf(-1, i) * M.buffer[i][0] * minor_r(M, i, 0);
		}

		return res;
	}
	// ������������ ������� �� ������ �������� ����� �� ��������
	else
	{
		return M.buffer[0][0];
	}
}

// ���������� ������������ ������� (������ ��� ���������� ������)
float Matr::det()
{
	if (n == 0) throw Ex("det()");	// ���� ������� �� ����������, �� ���������� ������
	if (n == 1) return buffer[0][0];	// ������������ ������� �� ������ �������� ����� �� ��������

	// ������� ������������ ������� �� ������� �������
	float res = 0;

	for (int i = 0; i < n; i++)
	{
		res += buffer[i][0] * powf(-1, i) * minor_r(*this, i, 0);
	}

	return res;
}

// ���������� �������� ������� (������ ��� ���������� ������)
Matr Matr::rev()
{
	if (n == 0) throw Ex("rev()"); // ���� ������� �� ����������, �� ���������� ������
	if (n == 1) return Matr(1, 1, 1 / buffer[0][0]); // �������� ������� �� ������ �������� ����� �� ��������� ��������

	Matr res(n, n);
	float det = this->det();

	if (det)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				res.buffer[i][j] = powf(-1, i + j) * minor_r(*this, j, i) / det;
			}
		}
	}

	return res;
}

// �������������� ������� (������ ��� ������-��������)
Matr diag(const Matr& m)
{
	if (m.w != 1) throw Matr::Ex("diag()"); // ���� ������� �� �������, �� ���������� ������

	Matr res(m.h, m.h, 0);

	for (int i = 0; i < m.h; i++)
	{
		res.buffer[i][i] = m.buffer[i][0];
	}

	return res;
}

// ���������������� �������
Matr Matr::transp()
{
	Matr res(w, h);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			res.buffer[j][i] = buffer[i][j];
		}
	}

	return res;
}


// ������������ ������, ���������� ��� ����� �������
void Matr::clear()
{
	for (int i = 0; i < h; i++)
	{
		delete[] buffer[i];
		buffer[i] = nullptr;
	}
	delete[] buffer;
	buffer = nullptr;
}


// ������ ��������� ������� �������
int Matr::getH() const
{
	return h;
}

int Matr::getW() const
{
	return w;
}

}