# 42 Kocaeli ComputorV1

## Polynomial Equation Solver (Polinom Denklem Çözücü)

Bu proje, 42 Okulu algoritma dalının bir parçasıdır. Birinci ve ikinci dereceden polinom denklemlerini çözen, matematiksel parsing, diskriminant hesaplama ve karmaşık sayı desteği ile donatılmış bir C++ programıdır.

## 📚 Özellikler

### Denklem Parsing (Ayrıştırma)

**Dosya:** `parser.cpp`

Polinom denklemlerini ayrıştırıp matematiksel yapıya dönüştürür.

- **Fonksiyon:** `parseExpression(char *expression)`
- **Desteklenen Format:** `a * X^2 + b * X + c = 0`
- **Özellikler:**
  - Boşluk karakterlerini otomatik atlar
  - Pozitif ve negatif katsayıları destekler
  - Katsayı olmayan terimleri (implicit 1) tanır
  - Eşitlik işaretinin her iki tarafındaki terimleri işler
  - Hata kontrolü ve detaylı hata mesajları
- **Örnek:**
  ```cpp
  "5 * X^2 + 3 * X - 2 = 0"
  "X^2 - 4 = 0"
  "2 * X = 4"
  ```

### İndirgenmiş Form Yazdırma

**Dosya:** `solver.cpp` - `printReducedForm()`

Denklemi indirgenmiş biçimde gösterir.

- **Açıklama:** Tüm terimleri bir tarafa toplar ve standart formda yazdırır
- **Çıktı Formatı:** `a * X^n + b * X^m + ... = 0`
- **Örnek:**
  ```bash
  Reduced form: 5 * X^2 + 3 * X - 2 = 0
  ```

### Polinom Derecesi Belirleme

**Dosya:** `solver.cpp` - `getPolynomialDegree()`

Denklemin derecesini hesaplar.

- **Açıklama:** En yüksek katsayısı sıfırdan farklı olan terimin üssünü bulur
- **Özel Durumlar:**
  - Derece -1: Tüm katsayılar sıfır (her sayı çözümdür)
  - Derece 0: Sadece sabit terim var (çözüm yok)
  - Derece 1: Doğrusal denklem
  - Derece 2: İkinci derece denklem
  - Derece > 2: Çözüm desteklenmiyor

### Doğrusal Denklem Çözücü

**Dosya:** `solver.cpp` - `solveLinear()`

Birinci dereceden denklemleri çözümler (ax + b = 0).

- **Formül:** `X = -b / a`
- **Çıktı:** Ondalık sayı ve kesir formu
- **Örnek:**
  ```bash
  Input: "2 * X + 4 = 0"
  The solution is:
  X = -b / a
  X = -(4) / 2
  X = -2 => -2
  ```

### İkinci Dereceden Denklem Çözücü

**Dosya:** `solver.cpp` - `solveQuadratic()`

İkinci dereceden denklemleri diskriminant ile çözümler (ax² + bx + c = 0).

- **Diskriminant:** `Δ = b² - 4ac`
- **Durum 1 - Δ > 0:** İki gerçek kök
  - Formül: `X = (-b ± √Δ) / 2a`
  - Her iki çözüm için ondalık ve kesir formu
- **Durum 2 - Δ = 0:** Tek (çift katlı) kök
  - Formül: `X = -b / 2a`
- **Durum 3 - Δ < 0:** İki karmaşık kök
  - Formül: `X = (-b ± i√|Δ|) / 2a`
  - Gerçek ve sanal kısımlar ayrı gösterilir
- **Örnek:**

  ```bash
  Input: "X^2 - 4 = 0"
  Discriminant Δ = b² - 4ac
  Δ = 0² - 4 * 1 * -4
  Δ = 16

  Discriminant is strictly positive, the two solutions are:
  X₁ = -2
  X₂ = 2
  ```

### Yardımcı Fonksiyonlar

**Dosya:** `utils.cpp`

Matematiksel ve yardımcı işlevler.

- **sqrt():** Newton-Raphson yöntemi ile karekök hesaplama
  - Standart kütüphane kullanmadan implement edilmiş
  - 1e-15 hassasiyetinde sonuç
- **gcd():** İki sayının en büyük ortak bölenini bulur (Euclid algoritması)
- **printFraction():** Ondalık sayıları basitleştirilmiş kesir formunda gösterir
  - GCD ile sadeleştirme
  - Karmaşık sayılar için 'i' eki
- **advanceWhitespace():** Parser için boşluk atlatma
- **error():** Hata mesajları ve program sonlandırma

## 🔧 Kullanım

### Kurulum

```bash
# Derleme
make

# veya doğrudan
c++ -Wall -Wextra -Werror main.cpp parser.cpp solver.cpp utils.cpp -o computor
```

### Çalıştırma

```bash
# Komut satırı argümanı ile
./computor "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"

# Standart input'tan
./computor
5 * X^2 + 3 * X - 2 = 0

# Basit örnekler
./computor "X^2 - 4 = 0"
./computor "2 * X = 8"
./computor "X^2 + 4 = 0"
```

### Makefile Komutları

```bash
make        # Projeyi derle
make clean  # Object dosyalarını sil
make fclean # Object ve executable dosyalarını sil
make re     # Yeniden derle

# Kısa komutlar
make c      # clean
make f      # fclean
make r      # re
```

## 🎯 Öğrenilen Kavramlar

1. **Polinom Matematiği**: Birinci ve ikinci dereceden denklemlerin çözümü
2. **Diskriminant Analizi**: Köklerin doğasını belirleme (gerçek/karmaşık)
3. **Parsing Algoritmaları**: String'den matematiksel yapıya dönüşüm
4. **Hata Yönetimi**: Geçersiz girdileri yakalama ve kullanıcıya bildirme
5. **Karmaşık Sayılar**: Negatif diskriminant durumunda çözüm
6. **Sayısal Algoritmalar**: Newton-Raphson karekök, Euclid GCD
7. **STL Kullanımı**: `map<long, double>` ile terim saklama
8. **Format İyileştirme**: Kesir sadeleştirme ve okunabilir çıktı

## 📋 Teknik Detaylar

### Proje Yapısı

```
.
├── main.cpp      # Ana program akışı ve argüman işleme
├── parser.cpp    # Denklem parsing motoru
├── parser.hpp    # Parser header
├── solver.cpp    # Çözüm algoritmaları
├── solver.hpp    # Solver header
├── utils.cpp     # Yardımcı matematiksel fonksiyonlar
├── utils.hpp     # Utils header
├── Makefile      # Build sistemi
└── README.md     # Dokümantasyon
```

### Önemli Noktalar

- **Kütüphane Kısıtlaması**: `sqrt()` fonksiyonu manuel implement edilmiş (math.h kullanılmamış)
- **Hassasiyet**: Floating-point karşılaştırmalarında 1e-15 toleransı
- **Kesir Formatı**: GCD ile 1/1000000 hassasiyetinde sadeleştirme
- **Hata Kontrolü**: Parser'da 10+ farklı hata durumu kontrol edilir
- **C++ Standartları**: Wall, Wextra, Werror bayrakları ile temiz kod
- **Map Kullanımı**: Sparse polinom gösterimi için verimli veri yapısı

## 💡 Örnek Çalıştırmalar

```bash
# Basit doğrusal
$ ./computor "2 * X = 8"
Reduced form: 2 * X = 0
Polynomial degree: 1
X = 4 => 4

# İki gerçek kök
$ ./computor "X^2 - 4 = 0"
Reduced form: -4 + 1 * X^2 = 0
Polynomial degree: 2
Discriminant Δ = 16
X₁ = -2
X₂ = 2

# Karmaşık kökler
$ ./computor "X^2 + 4 = 0"
Reduced form: 4 + 1 * X^2 = 0
Polynomial degree: 2
Discriminant Δ = -16
X₁ = 0 + 2i
X₂ = 0 - 2i

# Çift katlı kök
$ ./computor "X^2 - 2 * X + 1 = 0"
Reduced form: 1 - 2 * X + 1 * X^2 = 0
Polynomial degree: 2
Discriminant Δ = 0
X = 1
```
