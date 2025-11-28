# 42 Kocaeli Matrix Projesi

## Linear Algebra for Machine Learning (Makine Öğrenmesi için Lineer Cebir)

Bu proje, 42 Okulu'nun Matrix modülüdür. C++ ile template programlama kullanarak vektör ve matris işlemleri, lineer cebir algoritmaları ve makine öğrenmesi için temel matematiksel operasyonları içerir. Generic programlama, operator overloading ve ileri seviye C++ konseptlerini öğretmeyi amaçlar.

## 📚 Egzersizler

### ex00 - Add, Subtract and Scale

**Dosyalar:** `ex00/Vector.hpp`, `ex00/Vector.tpp`, `ex00/Matrix.hpp`, `ex00/Matrix.tpp`

Vektör ve matrislerde temel aritmetik işlemler.

- **Fonksiyonlar:**
  - `void add(const Vector<K> &v)` - Vektör toplama
  - `void sub(const Vector<K> &v)` - Vektör çıkarma
  - `void scl(K a)` - Skaler çarpma
  - `void add(const Matrix<K> &m)` - Matris toplama
  - `void sub(const Matrix<K> &m)` - Matris çıkarma
  - `void scl(K a)` - Matris skaler çarpma
- **Açıklama:** Element-wise operasyonlar ile temel vektör ve matris aritmetiği
- **Örnek:**

  ```cpp
  Vector<float> u = {2.f, 3.f};
  Vector<float> v = {5.f, 7.f};
  u.add(v);  // [7.0, 10.0]

  Matrix<float> m = {{1.f, 2.f}, {3.f, 4.f}};
  m.scl(2.f);  // [[2.0, 4.0], [6.0, 8.0]]
  ```

### ex01 - Linear Combination

**Dosyalar:** `ex01/Vector.hpp`, `ex01/Vector.tpp`

Vektörlerin lineer kombinasyonu.

- **Fonksiyon:** `Vector<K> linear_combination(const std::vector<Vector<K>> &u, const std::vector<K> &coefs)`
- **Açıklama:** Verilen vektörlerin ağırlıklı toplamını hesaplar: `c1*v1 + c2*v2 + ... + cn*vn`
- **Kullanım:** Vektör uzayında temel vektörlerin kombinasyonu
- **Örnek:**

  ```cpp
  Vector<float> e1 = {1.f, 0.f, 0.f};
  Vector<float> e2 = {0.f, 1.f, 0.f};
  Vector<float> e3 = {0.f, 0.f, 1.f};

  auto result = linear_combination<float>({e1, e2, e3}, {10.f, -2.f, 0.5f});
  // [10.0, -2.0, 0.5]
  ```

### ex02 - Linear Interpolation

**Dosyalar:** `ex02/Common.hpp`, `ex02/Common.tpp`

Lineer interpolasyon (lerp) fonksiyonu.

- **Fonksiyon:** `V lerp(V u, V v, float t)`
- **Açıklama:** İki değer arasında lineer interpolasyon yapar: `u + (v - u) * t`
- **Parametre:** `t ∈ [0, 1]` - Interpolasyon faktörü
- **Destekler:** Skaler değerler, vektörler ve matrisler
- **Örnek:**

  ```cpp
  lerp(0.f, 1.f, 0.5f);  // 0.5
  lerp(21.f, 42.f, 0.3f);  // 27.3

  lerp(Vector<float>({2.f, 1.f}), Vector<float>({4.f, 2.f}), 0.3f);
  // [2.6, 1.3]
  ```

### ex03 - Dot Product

**Dosyalar:** `ex03/Vector.hpp`, `ex03/Vector.tpp`

Vektörlerin nokta çarpımı (dot product).

- **Fonksiyon:** `K dot(const Vector<K> &v) const`
- **Açıklama:** İki vektörün iç çarpımını hesaplar: `u·v = u1*v1 + u2*v2 + ... + un*vn`
- **Kullanım:** Vektörler arası açı hesaplama, projeksiyon
- **Örnek:**

  ```cpp
  Vector<float> u = {0.f, 0.f};
  Vector<float> v = {1.f, 1.f};
  u.dot(v);  // 0.0

  Vector<float> u1 = {1.f, 1.f};
  Vector<float> v1 = {1.f, 1.f};
  u1.dot(v1);  // 2.0

  Vector<float> u2 = {-1.f, 6.f};
  Vector<float> v2 = {3.f, 2.f};
  u2.dot(v2);  // 9.0
  ```

### ex04 - Norm

**Dosyalar:** `ex04/Vector.hpp`, `ex04/Vector.tpp`

Vektör normları (uzunluk hesaplamaları).

- **Fonksiyonlar:**
  - `float norm_1() const` - Manhattan norm (L1): `|x1| + |x2| + ... + |xn|`
  - `float norm() const` - Euclidean norm (L2): `√(x1² + x2² + ... + xn²)`
  - `float norm_inf() const` - Supremum norm (L∞): `max(|x1|, |x2|, ..., |xn|)`
- **Açıklama:** Vektörün büyüklüğünü farklı metriklerle ölçer
- **Örnek:**

  ```cpp
  Vector<float> u = {0.f, 0.f, 0.f};
  u.norm_1();    // 0.0
  u.norm();      // 0.0
  u.norm_inf();  // 0.0

  Vector<float> u1 = {1.f, 2.f, 3.f};
  u1.norm_1();    // 6.0
  u1.norm();      // 3.74166
  u1.norm_inf();  // 3.0
  ```

### ex05 - Cosine

**Dosyalar:** `ex05/Vector.hpp`, `ex05/Vector.tpp`

İki vektör arasındaki açının kosinüsü.

- **Fonksiyon:** `float angle_cos(const Vector<K> &u, const Vector<K> &v)`
- **Açıklama:** `cos(θ) = (u·v) / (||u|| * ||v||)`
- **Kullanım:** Vektörler arası benzerlik ölçümü
- **Değer Aralığı:** `[-1, 1]` (0°=1, 90°=0, 180°=-1)
- **Örnek:**

  ```cpp
  Vector<float> u = {1.f, 0.f};
  Vector<float> v = {1.f, 0.f};
  angle_cos(u, v);  // 1.0 (0°)

  Vector<float> u1 = {1.f, 0.f};
  Vector<float> v1 = {0.f, 1.f};
  angle_cos(u1, v1);  // 0.0 (90°)

  Vector<float> u2 = {-1.f, 1.f};
  Vector<float> v2 = {1.f, -1.f};
  angle_cos(u2, v2);  // -1.0 (180°)
  ```

### ex06 - Cross Product

**Dosyalar:** `ex06/Vector.hpp`, `ex06/Vector.tpp`

3 boyutlu vektörlerin çapraz çarpımı.

- **Fonksiyon:** `Vector<K> cross_product(const Vector<K> &u, const Vector<K> &v)`
- **Açıklama:** İki vektöre dik üçüncü bir vektör üretir
- **Formül:** `u × v = [u2v3 - u3v2, u3v1 - u1v3, u1v2 - u2v1]`
- **Özellik:** Sadece 3D vektörler için tanımlı
- **Örnek:**

  ```cpp
  Vector<float> u = {0.f, 0.f, 1.f};
  Vector<float> v = {1.f, 0.f, 0.f};
  cross_product(u, v);  // [0.0, 1.0, 0.0]

  Vector<float> u1 = {1.f, 2.f, 3.f};
  Vector<float> v1 = {4.f, 5.f, 6.f};
  cross_product(u1, v1);  // [-3.0, 6.0, -3.0]
  ```

### ex07 - Linear Map, Matrix Multiplication

**Dosyalar:** `ex07/Matrix.hpp`, `ex07/Matrix.tpp`

Matris-vektör ve matris-matris çarpımı.

- **Fonksiyonlar:**
  - `Vector<K> mul_vec(const Vector<K> &vec) const` - Matris × Vektör
  - `Matrix<K> mul_mat(const Matrix<K> &mat) const` - Matris × Matris
- **Açıklama:** Lineer dönüşümler ve matris çarpımı
- **Kullanım:** Koordinat dönüşümleri, lineer sistemler
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 0.f}, {0.f, 1.f}};
  Vector<float> v = {4.f, 2.f};
  u.mul_vec(v);  // [4.0, 2.0]

  Matrix<float> m1 = {{1.f, 0.f}, {0.f, 1.f}};
  Matrix<float> m2 = {{2.f, 1.f}, {4.f, 2.f}};
  m1.mul_mat(m2);  // [[2.0, 1.0], [4.0, 2.0]]
  ```

### ex08 - Trace

**Dosyalar:** `ex08/Matrix.hpp`, `ex08/Matrix.tpp`

Matrisin izi (trace).

- **Fonksiyon:** `K trace() const`
- **Açıklama:** Köşegen elemanların toplamı: `tr(A) = a11 + a22 + ... + ann`
- **Özellik:** Sadece kare matrisler için tanımlı
- **Kullanım:** Özdeğerlerin toplamı, matris karakterizasyonu
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 0.f}, {0.f, 1.f}};
  u.trace();  // 2.0

  Matrix<float> u1 = {{2.f, -5.f, 0.f}, {4.f, 3.f, 7.f}, {-2.f, 3.f, 4.f}};
  u1.trace();  // 9.0

  Matrix<float> u2 = {{-2.f, -8.f, 4.f}, {1.f, -23.f, 4.f}, {0.f, 6.f, 4.f}};
  u2.trace();  // -21.0
  ```

### ex09 - Transpose

**Dosyalar:** `ex09/Matrix.hpp`, `ex09/Matrix.tpp`

Matris transpozu.

- **Fonksiyon:** `Matrix<K> transpose() const`
- **Açıklama:** Satır ve sütunları değiştirir: `AT[i][j] = A[j][i]`
- **Özellik:** `m×n` matris → `n×m` matris
- **Kullanım:** Simetrik matrisler, least squares
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 0.f}, {0.f, 1.f}};
  u.transpose();  // [[1.0, 0.0], [0.0, 1.0]]

  Matrix<float> u1 = {{2.f, -5.f, 0.f}, {4.f, 3.f, 7.f}};
  u1.transpose();  // [[2.0, 4.0], [-5.0, 3.0], [0.0, 7.0]]
  ```

### ex10 - Row Echelon Form

**Dosyalar:** `ex10/Matrix.hpp`, `ex10/Matrix.tpp`

Matrisin satır basamak formu.

- **Fonksiyon:** `Matrix<K> row_echelon() const`
- **Açıklama:** Gaussian elimination ile matris indirgemesi
- **Özellik:** Pivot elemanlar, sıfır satırlar alta
- **Kullanım:** Lineer denklem sistemleri, rank hesaplama
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 2.f}, {3.f, 4.f}};
  u.row_echelon();
  // [[1.0, 2.0], [0.0, -2.0]]

  Matrix<float> u1 = {{1.f, 2.f}, {2.f, 4.f}};
  u1.row_echelon();
  // [[1.0, 2.0], [0.0, 0.0]]
  ```

### ex11 - Determinant

**Dosyalar:** `ex11/Matrix.hpp`, `ex11/Matrix.tpp`

Matrisin determinantı.

- **Fonksiyon:** `K determinant() const`
- **Açıklama:** Kare matrisin determinantını hesaplar (row reduction kullanarak)
- **Özellik:** `det(A) ≠ 0` ⟺ Matris terslenebilir
- **Kullanım:** Matris tersi, lineer bağımsızlık
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, -1.f}, {-1.f, 1.f}};
  u.determinant();  // 0.0

  Matrix<float> u1 = {{2.f, 0.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 0.f, 2.f}};
  u1.determinant();  // 8.0

  Matrix<float> u2 = {{8.f, 5.f, -2.f}, {4.f, 7.f, 20.f}, {7.f, 6.f, 1.f}};
  u2.determinant();  // -174.0
  ```

### ex12 - Inverse

**Dosyalar:** `ex12/Matrix.hpp`, `ex12/Matrix.tpp`

Matrisin tersi.

- **Fonksiyon:** `Matrix<K> inverse() const`
- **Açıklama:** `A * A⁻¹ = I` özelliğini sağlayan matris
- **Yöntem:** Gauss-Jordan eliminasyonu
- **Özellik:** Sadece kare, terslenebilir matrisler için
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
  u.inverse();
  // [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]

  Matrix<float> u1 = {{2.f, 0.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 0.f, 2.f}};
  u1.inverse();
  // [[0.5, 0.0, 0.0], [0.0, 0.5, 0.0], [0.0, 0.0, 0.5]]
  ```

### ex13 - Rank

**Dosyalar:** `ex13/Matrix.hpp`, `ex13/Matrix.tpp`

Matrisin rankı.

- **Fonksiyon:** `size_t rank() const`
- **Açıklama:** Lineer bağımsız satır/sütun sayısı
- **Yöntem:** Row echelon form'daki sıfır olmayan satır sayısı
- **Kullanım:** Lineer sistemlerin çözülebilirliği
- **Örnek:**

  ```cpp
  Matrix<float> u = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
  u.rank();  // 3

  Matrix<float> u1 = {{1.f, 2.f, 0.f, 0.f}, {2.f, 4.f, 0.f, 0.f}, {-1.f, 2.f, 1.f, 1.f}};
  u1.rank();  // 2

  Matrix<float> u2 = {{8.f, 5.f, -2.f}, {4.f, 7.f, 20.f}, {7.f, 6.f, 1.f}, {21.f, 18.f, 7.f}};
  u2.rank();  // 3
  ```

## 🔧 Kullanım

### Gereksinimler

```bash
# C++ derleyici (C++11 veya üstü)
g++ --version  # veya clang++ --version

# Make
make --version
```

### Derleme

Her egzersiz kendi Makefile'ına sahiptir:

```bash
# Belirli bir egzersizi derleme
cd ex00
make

# Programı çalıştırma
./matrix

# Temizleme
make clean    # Object dosyalarını sil
make fclean   # Tüm derlenmiş dosyaları sil
make re       # Yeniden derle
```

### Tüm Egzersizleri Test Etme

```bash
# Her dizinde derleme ve çalıştırma
for i in ex*/; do
    cd "$i"
    make && ./matrix
    cd ..
done
```

## 🎯 Öğrenilen Kavramlar

1. **Template Programming**: Generic sınıflar ve fonksiyonlar, tip parametreleri
2. **Operator Overloading**: `+`, `-`, `*`, `<<` operatörlerinin yeniden tanımlanması
3. **RAII Pattern**: Resource Acquisition Is Initialization, bellek yönetimi
4. **Linear Algebra**: Vektör ve matris işlemleri, lineer dönüşümler
5. **Numerical Methods**: Gaussian elimination, matrix decomposition
6. **STL Integration**: `std::vector`, `std::initializer_list` kullanımı
7. **Friend Functions**: Template friend fonksiyonlar, non-member operatörler
8. **Const Correctness**: Const member functions, const parameters
9. **Mathematical Algorithms**: Determinant, inverse, rank hesaplamaları
10. **Memory Management**: Dynamic allocation, copy constructor, assignment operator

## 🏗️ Proje Yapısı

Her egzersiz şu dosyaları içerir:

- **Vector.hpp**: Vektör sınıfı tanımı ve deklarasyonları
- **Vector.tpp**: Vektör sınıfı template implementasyonları
- **Matrix.hpp**: Matris sınıfı tanımı ve deklarasyonları
- **Matrix.tpp**: Matris sınıfı template implementasyonları
- **Common.hpp**: Yardımcı fonksiyonlar (lerp, vb.)
- **Common.tpp**: Yardımcı fonksiyon implementasyonları
- **main.cpp**: Test programı
- **Makefile**: Derleme dosyası

## 📋 Notlar

- **Template Separation Model**: Header ve implementation dosyaları ayrılmıştır (.hpp ve .tpp)
- **Type Safety**: Template parametresi `K` sayısal tipler için tasarlanmıştır (float, double, int, complex)
- **Exception Handling**: Boyut uyumsuzlukları ve geçersiz işlemler için hata kontrolü
- **Performance**: Efficient memory layout, minimal copying, move semantics
- **Mathematical Correctness**: IEEE floating-point standardına uygun hesaplamalar
- **Code Reusability**: Daha ileri egzersizler önceki egzersizlerin kodlarını genişletir

## 🚀 İleri Seviye Konular

- **Eigenvalues & Eigenvectors**: Özdeğer ve özvektör hesaplamaları
- **SVD**: Singular Value Decomposition
- **QR Decomposition**: QR ayrıştırması
- **Least Squares**: En küçük kareler yöntemi
- **Matrix Projections**: Ortogonal ve diğer projeksiyonlar
