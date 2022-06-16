// ���� "matrix.h"
// � ������ ����� ������ ����� Matrix ��� ������ � ���������


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <vector>
#include <exception>

#define null 0

namespace cc
{


	// ����� �������
	template <typename T>
	class Matrix
	{
	private:

		size_t _rows;						// ������ ������� (���������� �����)
		size_t _columns;					// ������ ������� (���������� ��������)
		std::vector<std::vector<T>> _data;	// ����� �������� ������

		// ����� ���������� -1 � �������
		static inline int _MinusOnePower(int power)
		{
			return power % 2 ? -1 : 1;
		}

	public:
		bool IsSquare() const
		{
			return _rows == _columns;
		}

		size_t GetRows() const
		{
			return _rows;
		}

		size_t GetColumns() const
		{
			return _columns;
		}

		// ������� ������� (������ ��� ���������� ������, ����� 0)
		size_t GetOrder() const
		{
			return _rows == _columns ? _rows : 0;
		}

		// ������� ��� ������� ���������
		bool IsIdentity() const
		{
			if (!IsSquare()) return false;

			for (size_t i = 0; i < _rows; ++i)
			{
				for (size_t j = 0; j < _columns; ++j)
				{
					if (i == j)
					{
						if (_data[i][j] != 1) return false;
					}
					else
					{
						if (_data[i][j] != 0) return false;
					}
				}
			}

			return true;
		}

		// �������� �� ������� ������������
		bool IsDiagonal() const
		{
			if (!IsSquare()) return false;

			for (size_t i = 0; i < _rows; ++i)
			{
				for (size_t j = 0; j < _columns; ++j)
				{
					if (i != j && _data[i][j] != 0) return false;
				}
			}

			return true;
		}

		// ����������� ����������� ������� ��������
		explicit Matrix(std::vector<std::vector<T>>&& data2dVector)
			: _data(std::move(data2dVector))
			, _rows(data2dVector.size())
			, _columns(data2dVector.front().size())
		{
			this->Validate();
		}

		// ����������� ����������� ������� ��������
		explicit Matrix(const std::vector<std::vector<T>>& data2dVector)
			: Matrix(std::vector<std::vector<T>>(data2dVector)) // ����������� ������� ������� r-value ������, ������� �� ����������
		{
			this->Validate();
		}

		// ����������� ����������� �������
		explicit Matrix(const std::vector<T>& data1dVector)
			: _data(std::vector<std::vector<T>>(data1dVector.size()))
			, _rows(data1dVector.size())
			, _columns(1u)
		{
			auto it = data1dVector.cbegin();

			for (auto& row : _data)
			{
				row.reserve(1);
				row.emplace_back(*it);
				++it;
			}

			this->Validate();
		}

		// ����������� ����������� �������
		Matrix(const Matrix& matrix)
			: Matrix(std::vector<std::vector<T>>(matrix._data)) // ����������� ������� ������� r-value ������, ������� �� ����������
		{
			this->Validate();
		}

		// ����������� ����������� �������
		Matrix(Matrix&& matrix)
			: Matrix(std::move(matrix._data)) // �������� ���������� ����������� ������� ��������
		{
			this->Validate();
		}


		// ����������� ���������� ������� ������������ ��������
		explicit Matrix(size_t rows, size_t columns, const T& value)
			: Matrix(std::vector<std::vector<T>>(rows, std::vector<T>(columns, value))) // ����������� ������� ������� r-value ������, ������� �� ����������
		{
			this->Validate();
		}
		
		// ����������� ���������� ������������ ������� ������������� �������
		explicit Matrix(size_t order, const T& value)
			: Matrix(std::vector<std::vector<T>>(order, std::vector<T>(order, 0)))
		{
			for (size_t i = 0; i < order; ++i)
			{
				_data[i][i] = value;
			}

			this->Validate();
		}
		
		// ����� ��������� �������
		void Validate() const
		{
			if (_rows == 0)
			{
				throw std::out_of_range{ "rows_cannot_be_0" };
			}

			if (_columns == 0)
			{
				throw std::out_of_range("columns_cannot_be_0");
			}

			for (const auto& row : _data)
			{
				if (row.size() != _columns)
				{
					throw std::length_error("rows_have_different_sizes");
				}
			}
		}

		
		// ����������
		~Matrix() = default;

		// ����� ����������
		T& at(size_t i, size_t j)
		{
			return _data.at(i).at(j);
		}

		// ����������� ����� ����������
		const std::vector<T>& operator[](size_t index) const
		{
			return _data[index];
		}


		// �������������� ��������
		Matrix operator-() const
		{
			std::vector<std::vector<T>> resultData(_rows);

			for (std::size_t i = 0; i < _rows; ++i)
			{
				resultData[i].reserve(_columns);

				for (std::size_t j = 0; j < _columns; ++j)
				{
					resultData[i].emplace_back(-_data[i][j]);
				}
			}

			return Matrix(std::move(resultData));
		}

		Matrix& operator=(const Matrix&) = default;
		Matrix& operator=(Matrix&&) noexcept = default;
		
		// �������� �������� ������
		friend Matrix operator+(const Matrix& matrix1, const Matrix& matrix2)
		{
			if (!matrix1.IsEqualSizeTo(matrix2))
			{
				throw std::length_error("matrices_have_different_sizes");
			}

			std::vector<std::vector<T>> resultData(matrix1._rows);

			for (std::size_t i = 0; i < matrix1._rows; ++i)
			{
				resultData[i].reserve(matrix1._columns);

				for (std::size_t j = 0; j < matrix1._columns; ++j)
				{
					resultData[i].emplace_back(matrix1[i][j] + matrix2[i][j]);
				}
			}

			return Matrix(std::move(resultData));
		}

		// �������� ��������� ������
		friend Matrix operator-(const Matrix& matrix1, const Matrix& matrix2)
		{
			if (!matrix1.IsEqualSizeTo(matrix2))
			{
				throw std::length_error("matrices_have_different_sizes");
			}

			std::vector<std::vector<T>> resultData(matrix1._rows);

			for (std::size_t i = 0; i < matrix1._rows; ++i)
			{
				resultData[i].reserve(matrix1._columns);

				for (std::size_t j = 0; j < matrix1._columns; ++j)
				{
					resultData[i].emplace_back(matrix1[i][j] - matrix2[i][j]);
				}
			}

			return Matrix(std::move(resultData));
		}

		// �������� ��������� ������� �� �����
		friend Matrix operator*(const Matrix& matrix, const T& value)
		{
			std::vector<std::vector<T>> resultData(matrix._data);

			for (auto& row : resultData)
			{
				for (auto& element : row)
				{
					element *= value;
				}
			}

			return Matrix(std::move(resultData));
		}
		
		// �������� ��������� ������
		friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2)
		{
			if (!matrix1.IsCompatibleTo(matrix2))
			{
				throw std::length_error("matrices_have_incompatible_sizes");
			}

			std::vector<std::vector<T>> resultData(matrix1._rows);

			for (size_t i = 0; i < matrix1._rows; ++i)
			{
				resultData[i].reserve(matrix2._columns);

				for (size_t j = 0; j < matrix2._columns; ++j)
				{
					resultData[i].emplace_back();

					for (size_t k = 0; k < matrix1._columns; ++k)
					{
						resultData[i][j] += matrix1[i][k] * matrix2[k][j];
					}
				}
			}

			return Matrix(std::move(resultData));
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
		{
			for (const auto& row : matrix._data)
			{
				for (const auto& element : row)
				{
					os << std::setw(10) << element;
				}
				os << '\n';
			}

			return os;
		}

		// ����������� ������� �������� ������ �������� ������� 
		/*friend T GetMinor_r(const Matrix& matrix, size_t im, size_t jm)
		{
			if (!matrix.IsSquare()) throw std::exception("matrix_is_not_square");	// ���� ������� �� ����������, �� ���������� ������

			Matrix temp(matrix._rows - 1, matrix._columns - 1);
			size_t it = 0, jt = 0;

			for (size_t i = 0; i < matrix._rows; ++i)
			{
				for (size_t j = 0; j < matrix._columns; ++j)
				{
					if (i != im && j != jm) // ��������� �� ������� mi ������ � mj �������
					{
						temp.buffer[it][jt] = matrix.buffer[i][j];
						++jt;

						if (jt == matrix._rows - 1)
						{
							jt = 0;
							++it;
						}
					}
				}
			}

			// ������� ������������ ���������� ������� �� ������� �������
			if (temp._rows > 1)
			{
				T determinant(0);

				for (size_t i = 0; i < matrix._rows - 1; ++i)
				{
					determinant += temp.buffer[i][0] * Matrix::_PowI(-1, i) * GetMinor_r(temp, i, 0);
				}

				return determinant;
			}
			// ������������ ������� �� ������ �������� ����� �� ��������
			else
			{
				return temp[0][0];
			}
		}

		T GetDeterminant()
		{
			if (!IsSquare()) throw std::exception("matrix_is_not_square");	// ���� ������� �� ����������, �� ���������� ������

			if (GetOrder() == 1) return buffer[0][0];	// ������������ ������� �� ������ �������� ����� �� ��������

			// ������� ������������ ������� �� ������� �������
			T determinant{ 0 };

			for (size_t i = 0; i < _rows; ++i)
			{
				determinant += _data[i][0] * Matrix::_PowI(-1, i) * GetMinor_r(*this, i, 0);
			}

			return determinant;
		}*/

		/*Matrix Reverse()
		{
			if (n == 0) throw std::exception("rev()"); // ���� ������� �� ����������, �� ���������� ������

			if (n == 1) return Matrix(1, 1, T(1) / buffer[0][0]); // �������� ������� �� ������ �������� ����� �� ��������� ��������

			Matrix matrRev(n, n);
			T det(this->det());

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					matrRev.buffer[i][j] = powi(-1, i + j) * minor_r(*this, j, i) / det;
				}
			}

			return matrRev;
		}*/

		Matrix Diagonalize()
		{
			if (!IsSingleColumn())
			{
				throw std::out_of_range("matrix_is_not_single_column");
			}

			std::vector<std::vector<T>> resultData(_rows, std::vector<T>(_rows, 0));

			for (size_t i = 0; i < _rows; ++i)
			{
				resultData[i][i] = _data[i][0];
			}

			return Matrix(std::move(resultData));
		}

		Matrix Transpose()
		{
			std::vector<std::vector<T>> resultData(_columns, std::vector<T>(_rows)); // _rows � _columns ������ ��������

			for (size_t i = 0; i < _columns; ++i)
			{
				for (size_t j = 0; j < _rows; ++j)
				{
					resultData[i][j] = _data[j][i];
				}
			}

			return Matrix(std::move(resultData));
		}

		// ������� ������� ���������
		Matrix& Clear(const T& value)
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

		bool operator==(const Matrix& matrix2) const
		{
			return _data == matrix2._data;
		}

		bool operator!=(const Matrix& matrix2) const
		{
			return _data != matrix2._data;
		}

		bool IsSingleColumn() const
		{
			return _columns == 1;
		}

		bool IsCompatibleTo(const Matrix& matrix2) const
		{
			return _columns == matrix2._rows;
		}

		bool IsEqualSizeTo(const Matrix& matrix2) const
		{
			return _columns == matrix2._columns && _rows == matrix2._rows;
		}
	};

	using MatrixF = Matrix<float>;
	using MatrixI = Matrix<int>;
	using MatrixD = Matrix<double>;

}