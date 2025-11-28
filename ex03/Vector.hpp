#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <vector>

template <typename K>
class Vector;

template <typename K>
std::ostream &operator<<(std::ostream &os, const Vector<K> &v);

template <typename K>
Vector<K> linear_combination(const std::vector<Vector<K>> &u,
                             const std::vector<K> &coefs);

template <typename K>
class Vector
{
private:
  K *data;

  size_t size;

public:
  Vector();
  Vector(std::initializer_list<K> init);
  Vector(const Vector<K> &other);
  Vector &operator=(std::initializer_list<K> init);
  Vector &operator=(const Vector<K> &other);
  ~Vector();

  void add(const Vector<K> &v);
  void sub(const Vector<K> &v);
  void scl(K a);

  K dot(const Vector<K> &v) const;

  Vector<K> operator+(const Vector<K> &v) const;
  Vector<K> &operator+=(const Vector<K> &v);

  Vector<K> operator-(const Vector<K> &v) const;
  Vector<K> &operator-=(const Vector<K> &v);

  Vector<K> operator*(K a) const;
  Vector<K> &operator*=(K a);

  friend std::ostream &operator<< <>(std::ostream &os, const Vector<K> &v);

  friend Vector<K> linear_combination<>(const std::vector<Vector<K>> &u,
                                        const std::vector<K> &coefs);
};

#include "Vector.tpp"

#endif
