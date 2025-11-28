template <typename K>
Matrix<K>::Matrix()
{
	rows = 0;
	cols = 0;

	data = new K[0];
}

template <typename K>
Matrix<K>::Matrix(std::initializer_list<std::initializer_list<K>> init)
{
	rows = init.size();
	cols = rows == 0 ? 0 : init.begin()->size();

	for (const auto &row : init)
	{
		if (row.size() != cols)
			throw std::runtime_error("All rows must have the same number of columns.");
	}

	data = new K[rows * cols];

	size_t i = 0;
	size_t j;

	for (const auto &row : init)
	{
		j = 0;

		for (const K &value : row)
		{
			data[j * rows + i] = value;

			j++;
		}

		i++;
	}
}

template <typename K>
Matrix<K>::Matrix(const std::vector<std::vector<K>> &v)
{
	rows = v.size();
	cols = rows == 0 ? 0 : v.begin()->size();

	for (const auto &row : v)
	{
		if (row.size() != cols)
			throw std::runtime_error("All rows must have the same number of columns.");
	}

	data = new K[rows * cols];

	size_t i = 0;
	size_t j;

	for (const auto &row : v)
	{
		j = 0;

		for (const K &value : row)
		{
			data[j * rows + i] = value;

			j++;
		}

		i++;
	}
}

template <typename K>
Matrix<K>::Matrix(const Matrix<K> &other)
{
	rows = other.rows;
	cols = other.cols;

	size_t size = rows * cols;

	data = new K[size];

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];
}

template <typename K>
Matrix<K> &Matrix<K>::operator=(std::initializer_list<std::initializer_list<K>> init)
{
	size_t rows = init.size();
	size_t cols = rows == 0 ? 0 : init.begin()->size();

	for (const auto &row : init)
	{
		if (row.size() != cols)
			throw std::runtime_error("All rows must have the same number of columns.");
	}

	if (this->rows * this->cols != rows * cols)
	{
		delete[] data;

		data = new K[rows * cols];
	}

	this->rows = rows;
	this->cols = cols;

	size_t i = 0;
	size_t j;

	for (const auto &row : init)
	{
		j = 0;

		for (const K &value : row)
		{
			data[j * rows + i] = value;

			j++;
		}

		i++;
	}

	return *this;
}

template <typename K>
Matrix<K> &Matrix<K>::operator=(const std::vector<std::vector<K>> &v)
{
	size_t rows = v.size();
	size_t cols = rows == 0 ? 0 : v.begin()->size();

	for (const auto &row : v)
	{
		if (row.size() != cols)
			throw std::runtime_error("All rows must have the same number of columns.");
	}

	if (this->rows * this->cols != rows * cols)
	{
		delete[] data;

		data = new K[rows * cols];
	}

	this->rows = rows;
	this->cols = cols;

	size_t i = 0;
	size_t j;

	for (const auto &row : v)
	{
		j = 0;

		for (const K &value : row)
		{
			data[j * rows + i] = value;

			j++;
		}

		i++;
	}

	return *this;
}

template <typename K>
Matrix<K> &Matrix<K>::operator=(const Matrix<K> &other)
{
	size_t size = other.rows * other.cols;

	if (rows * cols != size)
	{
		delete[] data;

		data = new K[size];
	}

	rows = other.rows;
	cols = other.cols;

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];

	return *this;
}

template <typename K>
Matrix<K>::~Matrix()
{
	delete[] data;
}

template <typename K>
void Matrix<K>::add(const Matrix<K> &v)
{
	if (rows != v.rows || cols != v.cols)
		throw std::runtime_error("Matrix dimensions must match for addition.");

	size_t size = rows * cols;

	for (size_t i = 0; i < size; i++)
		data[i] += v.data[i];
}

template <typename K>
void Matrix<K>::sub(const Matrix<K> &v)
{
	if (rows != v.rows || cols != v.cols)
		throw std::runtime_error("Matrix dimensions must match for subtraction.");

	size_t size = rows * cols;

	for (size_t i = 0; i < size; i++)
		data[i] -= v.data[i];
}

template <typename K>
void Matrix<K>::scl(K a)
{
	size_t size = rows * cols;

	for (size_t i = 0; i < size; i++)
		data[i] *= a;
}

template <typename K>
Vector<K> Matrix<K>::mul_vec(const Vector<K> &vec) const
{
	if (cols != vec.size)
		throw std::runtime_error("Matrix columns must match vector size for multiplication.");

	std::vector<K> result(rows);

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			result[i] += data[j * rows + i] * vec.data[j];

	return result;
}

template <typename K>
Matrix<K> Matrix<K>::mul_mat(const Matrix<K> &mat) const
{
	if (cols != mat.rows)
		throw std::runtime_error("Matrix A columns must match Matrix B rows for multiplication.");

	std::vector<std::vector<K>> result(rows, std::vector<K>(mat.cols));

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < mat.cols; j++)
			for (size_t k = 0; k < cols; k++)
				result[i][j] += data[k * rows + i] * mat.data[j * mat.rows + k];

	return result;
}

template <typename K>
K Matrix<K>::trace() const
{
	if (rows != cols)
		throw std::runtime_error("Trace is only defined for square matrices.");

	size_t size = rows;

	K result = K{};

	for (size_t i = 0; i < size; i++)
		result += data[i * size + i];

	return result;
}

template <typename K>
Matrix<K> Matrix<K>::transpose() const
{
	std::vector<std::vector<K>> result(cols, std::vector<K>(rows));

	size_t k = 0;

	for (size_t i = 0; i < cols; i++)
	{
		for (size_t j = 0; j < rows; j++)
		{
			result[i][j] = data[k];

			k++;
		}
	}

	return result;
}

template <typename K>
Matrix<K> Matrix<K>::row_echelon() const
{
	std::vector<std::vector<K>> result(rows, std::vector<K>(cols));

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			result[i][j] = data[j * rows + i];

	for (size_t i = 0, j = 0; i < rows && j < cols; j++)
	{
		size_t pivot = i;

		while (pivot < rows && result[pivot][j] == 0)
			pivot++;

		if (pivot == rows)
			continue;

		if (pivot != i)
			std::swap(result[i], result[pivot]);

		if (result[i][j] != 1)
		{
			for (size_t k = cols - 1; k >= j; k--)
			{
				result[i][k] /= result[i][j];

				if (k == 0)
					break;
			}
		}

		for (size_t k = 0; k < rows; k++)
		{
			if (k != i && result[k][j] != 0)
			{
				for (size_t m = cols - 1; m >= j; m--)
				{
					result[k][m] -= result[k][j] * result[i][m];

					if (m == 0)
						break;
				}
			}
		}

		i++;
	}

	return result;
}

template <typename K>
K Matrix<K>::determinant() const
{
	if (rows != cols)
		throw std::runtime_error("Determinant only defined for square matrices");

	size_t size = rows;

	std::vector<std::vector<K>> result(size, std::vector<K>(size));

	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			result[i][j] = data[j * size + i];

	K det = 1;
	int sign = 1;

	for (size_t i = 0; i < size; i++)
	{
		size_t pivot = i;
		while (pivot < size && result[pivot][i] == 0)
			pivot++;

		if (pivot == size)
			return 0;

		if (pivot != i)
		{
			std::swap(result[i], result[pivot]);
			sign *= -1;
		}

		for (size_t j = i + 1; j < size; j++)
		{
			K factor = result[j][i] / result[i][i];

			for (size_t k = i; k < size; k++)
				result[j][k] -= factor * result[i][k];
		}
	}

	for (size_t i = 0; i < size; i++)
		det *= result[i][i];

	return det * sign;
}

template <typename K>
Matrix<K> Matrix<K>::inverse() const
{
	if (rows != cols)
		throw std::runtime_error("Inverse only defined for square matrices");

	size_t size = rows;

	std::vector<std::vector<K>> augmented(size, std::vector<K>(2 * size));

	for (size_t i = 0; i < size; i++)
	{
		augmented[i][i + size] = 1;

		for (size_t j = 0; j < size; j++)
			augmented[i][j] = data[j * size + i];
	}

	for (size_t i = 0; i < size; i++)
	{
		size_t pivot = i;
		while (pivot < size && augmented[pivot][i] == 0)
			pivot++;

		if (pivot == size)
			throw std::runtime_error("Matrix is singular and cannot be inverted.");

		if (pivot != i)
			std::swap(augmented[i], augmented[pivot]);

		if (augmented[i][i] != 1)
		{
			for (size_t k = 2 * size - 1; k >= i; k--)
			{
				augmented[i][k] /= augmented[i][i];

				if (k == 0)
					break;
			}
		}

		for (size_t k = 0; k < rows; k++)
		{
			if (k != i && augmented[k][i] != 0)
			{
				for (size_t m = 2 * size - 1; m >= i; m--)
				{
					augmented[k][m] -= augmented[k][i] * augmented[i][m];

					if (m == 0)
						break;
				}
			}
		}
	}

	std::vector<std::vector<K>> result(size, std::vector<K>(size));

	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			result[i][j] = augmented[i][j + size];

	return result;
}

template <typename K>
size_t Matrix<K>::rank() const
{
	Matrix<K> re = row_echelon();

	size_t rank = 0;

	bool havePivot;

	for (size_t i = 0; i < rows; i++)
	{
		havePivot = false;

		for (size_t j = 0; j < cols; j++)
		{
			if (re.data[j * rows + i] == 1)
			{
				havePivot = true;

				rank++;

				break;
			}
		}

		if (!havePivot)
			break;
	}

	return rank;
}

template <typename K>
Matrix<K> Matrix<K>::operator+(const Matrix<K> &v) const
{
	Matrix<K> result = *this;

	result.add(v);

	return result;
}

template <typename K>
Matrix<K> &Matrix<K>::operator+=(const Matrix<K> &v)
{
	this->add(v);

	return *this;
}

template <typename K>
Matrix<K> Matrix<K>::operator-(const Matrix<K> &v) const
{
	Matrix<K> result = *this;

	result.sub(v);

	return result;
}

template <typename K>
Matrix<K> &Matrix<K>::operator-=(const Matrix<K> &v)
{
	this->sub(v);

	return *this;
}

template <typename K>
Matrix<K> Matrix<K>::operator*(K a) const
{
	Matrix<K> result = *this;

	result.scl(a);

	return result;
}

template <typename K>
Matrix<K> &Matrix<K>::operator*=(K a)
{
	this->scl(a);

	return *this;
}

template <typename K>
std::ostream &operator<<(std::ostream &os, const Matrix<K> &v)
{
	if (v.rows == 0 && v.cols == 0)
	{
		os << "[]" << std::endl;
		return os;
	}

	for (size_t i = 0; i < v.rows; i++)
	{
		os << "[";

		for (size_t j = 0; j < v.cols; j++)
		{
			os << v.data[j * v.rows + i];

			if (j < v.cols - 1)
				os << ", ";
		}

		os << "]" << std::endl;
	}

	return os;
}
