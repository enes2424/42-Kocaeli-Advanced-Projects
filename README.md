# 42 Kocaeli Turing Machine Simulator
## Computational Theory and Automata (Hesaplama Kuramı ve Otomata)

Bu proje, 42 Okulu Advanced Projects kapsamında geliştirilmiş bir Turing Machine simülatörüdür. Alan Turing'in teorik bilgisayar modeli olan Turing makinelerini JavaScript/Deno ile simüle eder. Proje, hesaplama kuramı, state machine tasarımı ve algoritmik problem çözme becerilerini geliştirmek için tasarlanmıştır.

**Grup Projesi:** [Ercan Deniz](https://github.com/ercdeniz) ile birlikte geliştirilmiştir.

## 📚 Bileşenler

### bin/ft_turing.js
**Ana Dosya:** `bin/ft_turing.js`

Turing makine simülatörünün giriş noktası.
- **İçe Aktarımlar:** Argument parsing, dosya okuma, validasyon, çalıştırma ve görselleştirme modülleri
- **İşlevsellik:** Komut satırı argümanlarını işler, makine tanımını yükler ve simülasyonu başlatır
- **Hata Yönetimi:** Tüm hataları yakalar ve kullanıcı dostu mesajlarla çıkış yapar
- **Akış:**
  ```javascript
  parseArgs() → readJsonFile() → validateMachine() → 
  validateInput() → executeTuring() → printTuringData()
  ```

### lib/args.js
**Dosya:** `lib/args.js`

Komut satırı argüman işleme modülü.
- **Fonksiyon:** `parseArgs() -> {jsonFile, input}`
- **Özellikler:** Help mesajı desteği, hata kontrolü ve argüman validasyonu
- **Kullanım:**
  ```bash
  ft_turing [-h] jsonfile input
  ft_turing --help  # Yardım mesajını gösterir
  ```

### lib/file.js
**Dosya:** `lib/file.js`

JSON dosya okuma ve işleme modülü.
- **Fonksiyon:** `readJsonFile(filePath) -> Promise<Object>`
- **Hata Kontrolü:** Dosya bulunamama, geçersiz JSON ve okuma hatalarını yakalar
- **Örnek:**
  ```javascript
  const machine = await readJsonFile("machines/palindrome.json");
  ```

### lib/validation.js
**Dosya:** `lib/validation.js`

Kapsamlı makine ve girdi validasyon sistemi.
- **Ana Fonksiyonlar:**
  - `validateMachine(machineData)` - Makine tanımını doğrular
  - `validateInput(input, alphabet, blank)` - Girdi string'ini kontrol eder
- **Kontroller:**
  - Gerekli alanların varlığı (name, alphabet, blank, states, initial, finals, transitions)
  - Alfabe geçerliliği ve blank karakter kontrolü
  - State listesi ve geçiş tablosu doğrulaması
  - Duplicate state ve karakter kontrolü
- **Örnek Hata:**
  ```
  Missing required fields: 'alphabet', 'states'
  Input contains characters not in alphabet: x, y
  ```

### lib/execute_turing.js
**Dosya:** `lib/execute_turing.js`

Turing makine simülasyon motoru.
- **Ana Fonksiyon:** `executeTuring(turingData, input, state, index, history)`
- **Özellikler:**
  - Recursive execution model
  - Tape visualization with head position
  - Transition history tracking
  - Dynamic tape expansion (sonsuz tape simülasyonu)
- **Tape İşlemleri:**
  ```javascript
  replaceChar(str, index, newChar)  // Karakter değiştirme
  prepend(str, char)                // Başa ekleme
  append(str, char)                 // Sona ekleme
  visualizeTape(str, index, blank)  // Görselleştirme
  ```

### lib/print_turing_data.js
**Dosya:** `lib/print_turing_data.js`

Makine bilgilerini ve simülasyon sonuçlarını görselleştirme modülü.
- **Ana Fonksiyon:** `printTuringData(turingData, history)`
- **Çıktı Bileşenleri:**
  - Dekoratif makine ismi (ASCII box ile çevrelenmiş)
  - Alfabe, state'ler, initial/final state'ler
  - Transition tablosu
  - Adım adım execution history
- **Örnek Çıktı:**
  ```
  ************************************************************
  *                                                          *
  *                    unary_addition                        *
  *                                                          *
  ************************************************************
  Alphabet : [ 1, +, ., = ]
  States : [ controller, convert_plus, end, ACCEPT ]
  Initial : controller
  Finals : [ ACCEPT ]
  (controller, 1) -> (controller, 1, RIGHT)
  (controller, +) -> (convert_plus, 1, RIGHT)
  (convert_plus, 1) -> (convert_plus, 1, RIGHT)
  (convert_plus, =) -> (end, ., LEFT)
  (end, 1) -> (ACCEPT, ., LEFT)
  ************************************************************
  [<.>111+11=..........] (controller, .) -> (controller, ., RIGHT)
  [.<1>11+11=..........] (controller, 1) -> (controller, 1, RIGHT)
  [.1<1>1+11=..........] (controller, 1) -> (controller, 1, RIGHT)
  [.11<1>+11=..........] (controller, 1) -> (controller, 1, RIGHT)
  [.111<+>11=..........] (controller, +) -> (convert_plus, 1, RIGHT)
  [.1111<1>1=..........] (convert_plus, 1) -> (convert_plus, 1, RIGHT)
  [.11111<1>=..........] (convert_plus, 1) -> (convert_plus, 1, RIGHT)
  [.111111<=>..........] (convert_plus, =) -> (end, ., LEFT)
  [.11111<1>...........] (end, 1) -> (ACCEPT, ., LEFT)
  ```

## 🤖 Makine Tanımları (machines/)

### unary_addition.json
**Makine:** Unary Toplama İşlemi

Unary (birli) sayı sisteminde toplama yapan Turing makinesi.
- **Alfabe:** `["1", "+", ".", "="]`
- **Giriş Formatı:** `111+11=` (3 + 2 = 5)
- **Algoritma:** 
  1. `+` işaretini `1` ile değiştir
  2. `=` işaretini sil
  3. Son `1`'i sil (normalize etme)
- **Çıktı:** `11111` (5 adet 1)
- **Örnek:**
  ```bash
  ./ft_turing machines/unary_addition.json "111+11="
  # Sonuç: 11111 (3+2=5)
  ```

### palindrome.json
**Makine:** Palindrom Kontrolcü

Binary string'in palindrom olup olmadığını kontrol eden makine.
- **Alfabe:** `["0", "1", ".", "n", "y"]`
- **Giriş:** Binary string (örn: "0101010")
- **Algoritma:**
  1. İlk karakteri al ve işaretle
  2. Son karaktere git
  3. Eşleşme kontrolü yap
  4. İç karakterlere doğru ilerle
- **Çıktı:** `y` (palindrom) veya `n` (değil)
- **Örnek:**
  ```bash
  ./ft_turing machines/palindrome.json "1001"  # → y
  ./ft_turing machines/palindrome.json "1010"  # → n
  ```

### equal.json
**Makine:** Eşit Sayı Kontrolcü

Binary string'de 0 ve 1 sayılarının eşit olup olmadığını kontrol eder.
- **Alfabe:** `["0", "1", ".", "n", "y"]`
- **Giriş:** Binary string (örn: "00001111")
- **Algoritma:**
  1. Bir `0` sil
  2. Karşılık gelen bir `1` bul ve sil
  3. Tekrarla
  4. String boşalırsa eşit
- **Çıktı:** `y` (eşit) veya `n` (eşit değil)
- **Örnek:**
  ```bash
  ./ft_turing machines/equal.json "0011"     # → y
  ./ft_turing machines/equal.json "000111"   # → n
  ```

### even_zeros.json
**Makine:** Çift Sıfır Kontrolcü

String'de çift sayıda sıfır olup olmadığını kontrol eder.
- **Alfabe:** `["0", ".", "n", "y"]`
- **Giriş:** Sadece `0` karakterleri içeren string
- **Algoritma:**
  1. State machine ile sıfırları say
  2. İki state arasında geçiş (çift/tek)
  3. Final state'e göre karar ver
- **Çıktı:** `y` (çift sayıda) veya `n` (tek sayıda)
- **Örnek:**
  ```bash
  ./ft_turing machines/even_zeros.json "0000"  # → y (4 sıfır)
  ./ft_turing machines/even_zeros.json "000"   # → n (3 sıfır)
  ```

### unary_sub.json
**Makine:** Unary Çıkarma İşlemi

Unary sayı sisteminde çıkarma yapan Turing makinesi.
- **Alfabe:** `["1", ".", "-", "="]`
- **Giriş Formatı:** `1111-11=` (4 - 2 = 2)
- **Algoritma:**
  1. Sağdan bir `1` sil
  2. `-` işaretinden sola git
  3. Soldan bir `1` sil
  4. Tekrarla
- **Çıktı:** Kalan `1`'ler (fark)
- **Örnek:**
  ```bash
  ./ft_turing machines/unary_sub.json "1111-11="
  # Sonuç: 11 (4-2=2)
  ```

## 🔧 Kullanım

### Deno Kurulumu
```bash
# Linux/macOS
curl -fsSL https://deno.land/install.sh | sh

# Windows (PowerShell)
iwr https://deno.land/install.ps1 -useb | iex

# Homebrew
brew install deno
```

### Proje Derleme ve Çalıştırma
```bash
# Derleme
make

# Tüm test makinelerini çalıştırma
make run1  # Unary addition
make run2  # Palindrome
make run3  # Equal zeros and ones
make run4  # Even zeros
make run5  # Unary subtraction

# Manuel çalıştırma
./ft_turing machines/palindrome.json "1001"
deno run --allow-all bin/ft_turing.js machines/equal.json "0011"
```

### Komut Satırı Seçenekleri
```bash
# Yardım
./ft_turing --help
./ft_turing -h

# Temel kullanım
./ft_turing <machine_file.json> <input_string>

# Örnekler
./ft_turing machines/palindrome.json "racecar"
./ft_turing machines/unary_addition.json "111+1111="
```

## 🎯 Öğrenilen Kavramlar

1. **Turing Machine Theory**: Computation theory ve formal languages
2. **State Machine Design**: Finite state automata ve transition functions
3. **JSON Schema Validation**: Complex data structure validation
4. **Recursive Algorithms**: Tail recursion ve functional programming
5. **Tape Simulation**: Infinite tape simulation with dynamic expansion
6. **JavaScript Modules**: ES6 modüler programlama
7. **Deno Runtime**: Modern JavaScript/TypeScript runtime environment
8. **Error Handling**: Comprehensive error management ve user feedback
9. **Computational Complexity**: Algorithm analysis ve optimization
10. **Formal Verification**: Input validation ve mathematical correctness

## 📋 Teknik Detaylar

### Makefile Özellikleri
- **Deno Compile**: Standalone executable oluşturma
- **Permission Management**: `--allow-all` flag ile dosya erişimi
- **Test Automation**: Predefined test cases
- **Clean Operations**: Build artifact cleanup

### Kod Mimarisi
- **Modüler Tasarım**: Separation of concerns principle
- **Functional Programming**: Pure functions ve immutability
- **Error-First Approach**: Comprehensive error handling
- **JSON-Driven Configuration**: Machine definitions via JSON

### Algoritmik Yaklaşımlar
- **Recursive State Transitions**: Elegant state machine implementation
- **Dynamic Tape Management**: Memory efficient tape simulation
- **Pattern Matching**: Character recognition ve transition selection
- **History Tracking**: Complete execution trace maintenance

## 🚀 Gelişmiş Özellikler

- **Unlimited Tape**: Sonsuz tape simülasyonu
- **Step-by-Step Visualization**: Her adımın görselleştirilmesi
- **Comprehensive Validation**: Makine tanımı ve girdi doğrulaması
- **Beautiful Output**: ASCII art ve formatted display
- **Error Recovery**: Graceful error handling
- **Extensible Design**: Yeni makine tanımları ekleme kolaylığı

## 📊 Proje İstatistikleri

- **Toplam Dosya:** 11 dosya
- **Kod Satırı:** ~400+ satır JavaScript
- **Test Makineleri:** 5 farklı makine
- **Kapsanan Algoritmalar:** Palindrom, eşitlik, aritmetik işlemler
- **Runtime:** Deno v1.x+
- **Platform Desteği:** Linux, macOS, Windows
