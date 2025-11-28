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
