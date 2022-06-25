// файл "Matrix.inl"
// в данном файле реализован шаблон класса Matrix для работы с матрицами

namespace mtx
{

// конструктор перемещения вектора векторов
template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& data)
	: _data(std::move(data))
	, _rows(_data.size())
	, _columns(_data.front().size())
	, _order(_rows == _columns ? _rows : 0u)
{
	this->Validate();
}

// конструктор копирования вектора векторов
template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& data)
	: Matrix(std::vector<std::vector<T>>(data)) // конструктор вектора создает r-value объект, который мы перемещаем
{
	this->Validate();
}

// конструктор заполнения матрицы произвольных размеров
template<typename T>
Matrix<T>::Matrix(size_t rows, size_t columns, const T& value)
	: Matrix(std::vector<std::vector<T>>(rows, std::vector<T>(columns, value))) // конструктор вектора создает r-value объект, который мы перемещаем
{
	this->Validate();
}

// конструктор заполнения диагональной матрицы определенного порядка
template<typename T>
Matrix<T>::Matrix(size_t order, const T& value)
	: Matrix(std::vector<std::vector<T>>(order, std::vector<T>(order, 0u)))
{
	for (size_t i = 0; i < order; ++i)
	{
		_data[i][i] = value;
	}

	this->Validate();
}

// конструктор копирования вектора
template<typename T>
Matrix<T>::Matrix(const std::vector<T>& data)
	: _data(std::vector<std::vector<T>>(data.size()))
	, _rows(_data.size())
	, _columns(1u)
	, _order(_rows == _columns ? _rows : 0u)
{
	auto it = data.cbegin();

	for (auto& row : _data)
	{
		row.reserve(1);
		row.emplace_back(*it);
		++it;
	}

	this->Validate();
}

// конструктор копирования матрицы
template<typename T>
Matrix<T>::Matrix(const Matrix& matrix)
	: Matrix(std::vector<std::vector<T>>(matrix._data)) // конструктор вектора создает r-value объект, который мы перемещаем
{
	this->Validate();
}

// конструктор перемещения матрицы
template<typename T>
Matrix<T>::Matrix(Matrix&& matrix)
	: Matrix(std::move(matrix._data)) // вызываем контруктор перемещения вектора векторов
{
	this->Validate();
}

// метод валидации матрицы
template<typename T>
inline void Matrix<T>::Validate() const
{
	if (_rows == 0)
	{
		throw std::invalid_argument{ "rows_cant_be_0" };
	}

	if (_columns == 0)
	{
		throw std::invalid_argument("columns_cant_be_0");
	}

	for (const auto& row : _data)
	{
		if (row.size() != _columns)
		{
			throw std::length_error("rows_have_different_sizes");
		}
	}
}

// операция отрицания матрицы
template<typename T>
Matrix<T> Matrix<T>::operator-() const
{
	std::vector<std::vector<T>> resultData(_rows);

	for (size_t i = 0; i < _rows; ++i)
	{
		resultData[i].reserve(_columns);

		for (size_t j = 0; j < _columns; ++j)
		{
			resultData[i].emplace_back(-_data[i][j]);
		}
	}

	return Matrix(std::move(resultData));
}

// операция умножения матрицы на число
template<typename T>
Matrix<T> Matrix<T>::operator*(const T& value) const
{
	std::vector<std::vector<T>> resultData(_data);

	for (auto& row : resultData)
	{
		for (auto& element : row)
		{
			element *= value;
		}
	}

	return Matrix(std::move(resultData));
}

// операция сложения матриц
template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& matrix) const
{
	if (!IsEqualSizeTo(matrix))
	{
		throw std::length_error("matrices_have_different_sizes");
	}

	std::vector<std::vector<T>> resultData(_rows);

	for (size_t i = 0; i < _rows; ++i)
	{
		resultData[i].reserve(_columns);

		for (size_t j = 0; j < _columns; ++j)
		{
			resultData[i].emplace_back(_data[i][j] + matrix[i][j]);
		}
	}

	return Matrix(std::move(resultData));
}

// операция вычитания матриц
template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& matrix) const
{
	if (!IsEqualSizeTo(matrix))
	{
		throw std::length_error("matrices_have_different_sizes");
	}

	std::vector<std::vector<T>> resultData(_rows);

	for (size_t i = 0; i < _rows; ++i)
	{
		resultData[i].reserve(_columns);

		for (size_t j = 0; j < _columns; ++j)
		{
			resultData[i].emplace_back(_data[i][j] - matrix[i][j]);
		}
	}

	return Matrix(std::move(resultData));
}

// операция умнлжения матриц
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& matrix) const
{
	if (!IsCompatibleTo(matrix))
	{
		throw std::length_error("matrices_have_incompatible_sizes");
	}

	std::vector<std::vector<T>> resultData(_rows);

	for (size_t i = 0; i < _rows; ++i)
	{
		resultData[i].reserve(matrix._columns);

		for (size_t j = 0; j < matrix._columns; ++j)
		{
			resultData[i].emplace_back(0);

			for (size_t k = 0; k < _columns; ++k)
			{
				resultData[i][j] += _data[i][k] * matrix[k][j];
			}
		}
	}

	return Matrix(std::move(resultData));
}

// сравнение матриц
template<typename T>
bool Matrix<T>::operator==(const Matrix& matrix2) const
{
	if (!IsEqualSizeTo(matrix2))
	{
		return false;
	}

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _columns; ++j)
		{
			if (!_IsZero(_data[i][j] - matrix2._data[i][j])) return false;
		}
	}

	return true;
}

template<typename T>
bool Matrix<T>::operator!=(const Matrix& matrix2) const
{
	if (!IsEqualSizeTo(matrix2))
	{
		return true;
	}
	
	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _columns; ++j)
		{
			if (!_IsZero(_data[i][j] - matrix2._data[i][j])) return true;
		}
	}

	return false;
}

// константная операция индексации
template<typename T>
const std::vector<T>& Matrix<T>::operator[](const size_t& i) const
{
	return _data.operator[](i);
}

// метод индексации
template<typename T>
T& Matrix<T>::at(const size_t& i, const size_t& j)
{
	return _data.at(i).at(j);
}

// составить минорную матрицу
template<typename T>
Matrix<T> Matrix<T>::GetMinorMatrix(const size_t& minorRow, const size_t& minorColumn) const
{
	if (!_order)
	{
		throw std::logic_error("matrix_is_not_square");
	}

	if (minorRow >= _order)
	{
		throw std::out_of_range("minor_row_is_out_of_rows");
	}

	if (minorColumn >= _order)
	{
		throw std::out_of_range("minor_column_is_out_of_columns");
	}

	std::vector<std::vector<T>> resultData(_order - 1, std::vector<T>(_order - 1));
	size_t im = 0, jm = 0;

	for (size_t i = 0; i < _order; ++i)
	{
		for (size_t j = 0; j < _order; ++j)
		{
			if (i != minorRow && j != minorColumn) // исключаем из матрицы minorRow строку и minorColumn столбец
			{
				resultData[im][jm] = _data[i][j];
				++jm;

				if (jm == _order - 1)
				{
					jm = 0;
					++im;
				}
			}
		}
	}

	return Matrix(std::move(resultData));
}

// найти минор матрицы
template<typename T>
T Matrix<T>::GetMinor(const size_t& minorRow, const size_t& minorColumn) const
{
	return GetMinorMatrix(minorRow, minorColumn).GetDeterminant();
}

// найти определитель матрицы
template<typename T>
T Matrix<T>::GetDeterminant() const
{
	if (!_order)
	{
		throw std::logic_error("matrix_is_not_square"); // если матрица не квадратная, то генерируем ошибку
	}

	// для малых размеров вычисляем по формулам
	if (_order == 1)
	{
		return _data[0][0];
	}

	if (_order == 2)
	{
		return _data[0][0] * _data[1][1]
			 - _data[0][1] * _data[1][0];
	}

	if (_order == 3)
	{
		return _data[0][0] * _data[1][1] * _data[2][2]
			 + _data[2][0] * _data[0][1] * _data[1][2]
			 + _data[1][0] * _data[2][1] * _data[0][2]
			 - _data[2][0] * _data[1][1] * _data[0][2]
			 - _data[0][0] * _data[2][1] * _data[1][2]
			 - _data[1][0] * _data[0][1] * _data[2][2];
	}

	// находим определитель матрицы по теореме Лапласа
	T determinant(0);

	for (size_t i = 0; i < _order; ++i)
	{
		determinant += _data[i][0] * T(_MinusOnePower(i)) * GetMinor(i, 0u);
	}

	return determinant;
}

// найти обратную матрицу
template<typename T>
Matrix<T> Matrix<T>::GetReverse() const
{
	if (!_order)
	{
		throw std::logic_error("matrix_is_not_square");	// если матрица не квадратная, то генерируем ошибку
	}

	T determinant(GetDeterminant());

	if (_IsZero(determinant))
	{
		throw std::logic_error("zero_determinant"); // если определитель 0, то обратная матрица отсуствует
	}

	if (_order == 1)
	{
		return Matrix( 1u, 1u, T(1) / _data[0][0]); // обратная матрица из одного элемента равна ее обратному элементу
	}

	std::vector<std::vector<T>> resultData(_order);

	for (size_t i = 0; i < _order; ++i)
	{
		resultData[i].reserve(_order);

		for (size_t j = 0; j < _order; ++j)
		{
			resultData[i].emplace_back(T(_MinusOnePower(i + j)) * GetMinor(i, j) / determinant); // используем emplace_back() вместо push_back(), чтобы вычесленное временное значение не копировалось, а перемещалось. Для этого вначале резервиркем место reserve()
		}
	}

	return Matrix(std::move(resultData)).GetTransposed();
}

// найти диагональную матрицу
template<typename T>
Matrix<T> Matrix<T>::GetDiagonal() const
{
	if (_columns != 1)
	{
		throw std::out_of_range("matrix_is_not_single_column");
	}

	std::vector<std::vector<T>> resultData(_rows, std::vector<T>(_rows, 0u));

	for (size_t i = 0; i < _rows; ++i)
	{
		resultData[i][i] = _data[i][0];
	}

	return Matrix(std::move(resultData));
}

// найти транспонированную матрицу
template<typename T>
Matrix<T> Matrix<T>::GetTransposed() const
{
	std::vector<std::vector<T>> resultData(_columns, std::vector<T>(_rows)); // _rows и _columns меняем местамин

	for (size_t i = 0; i < _columns; ++i)
	{
		for (size_t j = 0; j < _rows; ++j)
		{
			resultData[i][j] = _data[j][i];
		}
	}

	return Matrix(std::move(resultData));
}

// очистка матрицы значением
template<typename T>
Matrix<T>& Matrix<T>::Clear(const T& value)
{
	for (auto& row : _data)
	{
		for (auto& element : row)
		{
			element = value;
		}
	}

	return *this;
}

// является ли матрица столбцом
template<typename T>
inline bool Matrix<T>::IsSingleColumn() const noexcept
{
	return _columns == 1;
}

// совместимы ли матрицы
template<typename T>
inline bool Matrix<T>::IsCompatibleTo(const Matrix& matrix2) const noexcept
{
	return _columns == matrix2._rows;
}

// одинаковые ли размеры матриц
template<typename T>
inline bool Matrix<T>::IsEqualSizeTo(const Matrix& matrix2) const noexcept
{
	return _columns == matrix2._columns && _rows == matrix2._rows;
}

// является ли матрица квадратной
template<typename T>
inline bool Matrix<T>::IsSquare() const noexcept
{
	return static_cast<bool>(_order);
}

// является ли матрица единичной
template<typename T>
bool Matrix<T>::IsIdentity() const
{
	if (!_order)
	{
		return false;
	}

	for (size_t i = 0; i < _order; ++i)
	{
		for (size_t j = 0; j < _order; ++j)
		{
			if (i == j)
			{
				if (!_IsZero(_data[i][j] - 1)) return false;
			}
			else
			{
				if (!_IsZero(_data[i][j])) return false;
			}
		}
	}

	return true;
}

// является ли матрица диагональной
template<typename T>
bool Matrix<T>::IsDiagonal() const
{
	if (!_order)
	{
		return false;
	}

	for (size_t i = 0; i < _order; ++i)
	{
		for (size_t j = 0; j < _order; ++j)
		{
			if (i != j && !_IsZero(_data[i][j])) return false;
		}
	}

	return true;
}

// методы получения свойств матрицы
template<typename T>
inline size_t Matrix<T>::GetRows() const noexcept
{
	return _rows;
}

template<typename T>
inline size_t Matrix<T>::GetColumns() const noexcept
{
	return _columns;
}

template<typename T>
inline size_t Matrix<T>::GetOrder() const noexcept
{
	return _order; // порядок матрицы (только для квадратных матриц, иначе 0)
}

}