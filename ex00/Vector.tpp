template <typename K>
Vector<K>::Vector()
{
	size = 0;

	data = new K[0];
}

template <typename K>
Vector<K>::Vector(std::initializer_list<K> init)
{
	size = init.size();

	data = new K[size];

	size_t i = 0;

	for (const K &value : init)
	{
		data[i] = value;

		i++;
	}
}

template <typename K>
Vector<K>::Vector(const Vector<K> &other)
{
	size = other.size;

	data = new K[size];

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];
}

template <typename K>
Vector<K> &Vector<K>::operator=(std::initializer_list<K> init)
{
	if (size != init.size())
	{
		delete[] data;

		size = init.size();

		data = new K[size];
	}

	size_t i = 0;

	for (const K &value : init)
	{
		data[i] = value;

		i++;
	}

	return *this;
}

template <typename K>
Vector<K> &Vector<K>::operator=(const Vector<K> &other)
{
	if (size != other.size)
	{
		delete[] data;

		size = other.size;

		data = new K[size];
	}

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];

	return *this;
}

template <typename K>
Vector<K>::~Vector()
{
	delete[] data;
}

template <typename K>
void Vector<K>::add(const Vector<K> &v)
{
	if (size != v.size)
		throw std::runtime_error("Vector sizes do not match for addition.");

	for (size_t i = 0; i < size; i++)
		data[i] += v.data[i];
}

template <typename K>
void Vector<K>::sub(const Vector<K> &v)
{
	if (size != v.size)
		throw std::runtime_error("Vector sizes do not match for subtraction.");

	for (size_t i = 0; i < size; i++)
		data[i] -= v.data[i];
}

template <typename K>
void Vector<K>::scl(K a)
{
	for (size_t i = 0; i < size; i++)
		data[i] *= a;
}

template <typename K>
std::ostream &operator<<(std::ostream &os, const Vector<K> &v)
{
	if (v.size == 0)
	{
		os << "[]" << std::endl;
		return os;
	}

	for (size_t i = 0; i < v.size; i++)
		os << "[" << v.data[i] << "]" << std::endl;

	return os;
}
