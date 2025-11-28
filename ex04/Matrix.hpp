#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

template <typename K>
class Matrix;

template <typename K>
std::ostream &operator<<(std::ostream &os, const Matrix<K> &v);

template <typename K>
class Matrix
{
private:
	K *data;

	size_t rows;
	size_t cols;

public:
	Matrix();
	Matrix(std::initializer_list<std::initializer_list<K>> init);
	Matrix(const Matrix<K> &other);
	Matrix &operator=(std::initializer_list<std::initializer_list<K>> init);
	Matrix &operator=(const Matrix<K> &other);
	~Matrix();

	void add(const Matrix<K> &v);
	void sub(const Matrix<K> &v);
	void scl(K a);

	Matrix<K> operator+(const Matrix<K> &v) const;
	Matrix<K> &operator+=(const Matrix<K> &v);

	Matrix<K> operator-(const Matrix<K> &v) const;
	Matrix<K> &operator-=(const Matrix<K> &v);

	Matrix<K> operator*(K a) const;
	Matrix<K> &operator*=(K a);

	friend std::ostream &operator<< <>(std::ostream &os, const Matrix<K> &v);
};

#include "Matrix.tpp"

#endif
