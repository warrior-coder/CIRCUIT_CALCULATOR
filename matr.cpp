// файл "matr.cpp"
// в данном файле реализован класс Matr для работы с матрицами


#include "matr.h"


namespace matr
{

// в классе актуальна генерация исключений, т.к. некоторые операции над матрицами невозможны при их несоответствии
// класс исключения Ex (сокращенно от Exception) содержит строку с информацией об ошибке
class Matr::Ex
{
	const char* info;

public:
	Ex(const char* info) : info(info) {}
	~Ex() = default;

	const char* what() const { return info; }
};


// конструкторы

// создает матрицу из двумерного списка инициализации
// реализует инициализацию, аналогичную двумерному массиву
// автоматически выводит размеры матрицы исходя из размеров списков, что позволяет избавиться от явного задания размеров матрицы
Matr::Matr(std::initializer_list<std::initializer_list<float>> il)
	: h(il.size())				// высота матрицы равна размеру списка списков
	, w(il.begin()->size())		// ширина матрицы равна размеру подсписка
	, n(h == w ? h : 0)			// порядок матрицы вычисляется, исходя из размеров матрицы
{
	auto it = il.begin(); // получаем итератор на начало списка

	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		if (it[i].size() != w) throw Ex("invalid size"); // если в матрице строки разной длинны, то генерируем ошибку

		auto jt = it[i].begin(); // получаем итератор на начало i-го подсписка

		buffer[i] = new float[w];

		for (int j = 0; j < w; j++)
		{
			buffer[i][j] = jt[j]; // копируем элементы подсписка
		}
	}
}

// создает матрицу-столбец из одномерного списка инициализации
Matr::Matr(std::initializer_list<float> il)
	: h(il.size())			// высота матрицы равна размеру списка
	, w(1)					// ширина матрицы равна единице
	, n(h == 1 ? 1 : 0)		// у матрицы-столбца может быть только первый порядок или отсутствовать
{
	auto it = il.begin();

	buffer = new float* [h];

	for (int i = 0; i < h; i++)
	{
		buffer[i] = new float(it[i]);
	}
}

// создает матрицу указанных размеров и заполняет значениями val (по умолчанию нулями)
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

// конструктор копирования
// из-за наличия динамических полей в классе следует использовать глубокое копирование
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

// деструктор
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


// метод индексации возвращает ссылку на элемент матрицы
float& Matr::at(int i, int j)
{
	return *(*(buffer + i) + j);
}


// следующие перегруженные операции полностью отражают логику математических операций над матрицами

// операция присваивания матрицы
Matr& Matr::operator=(const Matr& m)
{
	// присваиваемая матрица может быть другого размера, поэтому старую память следует освободить и выделить новую
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

// операция помещения в поток вывода
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

// операция умножения матриц
Matr operator*(const Matr& m1, const Matr& m2)
{
	// если матрицы не согласованны, то генерируем ошибку
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

// операция умножения матрицы на число
Matr operator*(const Matr& m, float x)
{
	Matr res = m;

	for (int i = 0; i < res.h; i++)
	{
		for (int j = 0; j < res.w; j++) res.buffer[i][j] *= x;
	}

	return res;
}

// операция сложения матриц
Matr operator+(const Matr& m1, const Matr& m2)
{
	// если матрицы не одного размера, то генерируем ошибку
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

// операция вычитания матриц
Matr operator-(const Matr& m1, const Matr& m2)
{
	// если матрицы не одного размера, то генерируем ошибку
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

// операция отрициния матрицы
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

// рекурсивная функция вычисления минора матрицы (только для квадратных матриц)
float minor_r(const Matr& m, int mi, int mj)
{
	if (m.n == 0) throw Matr::Ex("minor_r()");	// если матрица не квадратная, то генерируем ошибку

	Matr M(m.n - 1, m.n - 1);
	int it = 0, jt = 0;

	for (int i = 0; i < m.n; i++)
	{
		for (int j = 0; j < m.n; j++)
		{
			if (i != mi && j != mj) // исключаем из матрицы mi строку и mj столбец
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

	// находим определитель полученной матрицы по теореме Лапласа
	if (M.n > 1)
	{
		float res = 0;

		for (int i = 0; i < m.n - 1; i++)
		{
			res += powf(-1, i) * M.buffer[i][0] * minor_r(M, i, 0);
		}

		return res;
	}
	// определитель матрицы из одного элемента равен ее элементу
	else
	{
		return M.buffer[0][0];
	}
}

// вычисление определителя матрицы (только для квадратных матриц)
float Matr::det()
{
	if (n == 0) throw Ex("det()");	// если матрица не квадратная, то генерируем ошибку
	if (n == 1) return buffer[0][0];	// определитель матрицы из одного элемента равен ее элементу

	// находим определитель матрицы по теореме Лапласа
	float res = 0;

	for (int i = 0; i < n; i++)
	{
		res += buffer[i][0] * powf(-1, i) * minor_r(*this, i, 0);
	}

	return res;
}

// вычисление обратной матрицы (только для квадратных матриц)
Matr Matr::rev()
{
	if (n == 0) throw Ex("rev()"); // если матрица не квадратная, то генерируем ошибку
	if (n == 1) return Matr(1, 1, 1 / buffer[0][0]); // обратная матрица из одного элемента равна ее обратному элементу

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

// диагонализация матрицы (только для матриц-столбцов)
Matr diag(const Matr& m)
{
	if (m.w != 1) throw Matr::Ex("diag()"); // если матрица не столбец, то генерируем ошибку

	Matr res(m.h, m.h, 0);

	for (int i = 0; i < m.h; i++)
	{
		res.buffer[i][i] = m.buffer[i][0];
	}

	return res;
}

// транспонирование матрицы
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


// освобождение памяти, выделенной под буфер матрицы
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


// методы получения свойств матрицы
int Matr::getH() const
{
	return h;
}

int Matr::getW() const
{
	return w;
}

}