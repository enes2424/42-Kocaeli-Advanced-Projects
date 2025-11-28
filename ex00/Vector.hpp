#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

template <typename K>
class Vector;

template <typename K>
std::ostream &operator<<(std::ostream &os, const Vector<K> &v);

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

  friend std::ostream &operator<< <>(std::ostream &os, const Vector<K> &v);
};

#include "Vector.tpp"

#endif
