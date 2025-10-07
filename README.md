# 42 Kocaeli LIBASM
## Assembly Programming and System Calls (Assembly Programlama ve Sistem Çağrıları)

Bu proje, 42 Okulu'nun libasm projesidir. Assembly dilinde temel C kütüphane fonksiyonlarının implementasyonu, sistem çağrılarının kullanımı ve bellek yönetimini öğretmek için tasarlanmıştır.

## 📚 Temel Fonksiyonlar

### ft_strlen
**Dosya:** `ft_strlen.s`

String uzunluğunu hesaplayan fonksiyon.
- **Fonksiyon:** `ft_strlen(const char *str) -> size_t`
- **Açıklama:** Verilen string'in null terminator'a kadar olan karakter sayısını döndürür
- **Registrlar:** RDI (input), RAX (output)
- **Algoritma:** 
  ```asm
  mov rax, rdi        ; başlangıç adresini kopyala
  .loop:
    cmp byte [rax], 0 ; null karakter kontrolü
    je .done          ; bulundu ise çık
    inc rax           ; sonraki karakter
    jmp .loop
  .done:
    sub rax, rdi      ; uzunluğu hesapla
  ```

### ft_strcpy
**Dosya:** `ft_strcpy.s`

String kopyalama fonksiyonu.
- **Fonksiyon:** `ft_strcpy(char *dst, const char *src) -> char *`
- **Açıklama:** Kaynak string'i hedef buffer'a kopyalar, null terminator dahil
- **Registrlar:** RDI (dst), RSI (src), RAX (return dst), DL (temp)
- **Özellik:** Orijinal destination pointer'ı döndürür
- **Örnek:**
  ```c
  char dest[20];
  ft_strcpy(dest, "Hello World"); // "Hello World" -> dest
  ```

### ft_strcmp
**Dosya:** `ft_strcmp.s`

String karşılaştırma fonksiyonu.
- **Fonksiyon:** `ft_strcmp(const char *s1, const char *s2) -> int`
- **Açıklama:** İki string'i lexicographic olarak karşılaştırır
- **Registrlar:** RDI (s1), RSI (s2), AL/DL (temp), RAX (result)
- **Dönüş Değerleri:**
  - `0`: String'ler eşit
  - `1`: s1 > s2
  - `-1`: s1 < s2
- **Örnek:**
  ```c
  ft_strcmp("abc", "abc");  // 0
  ft_strcmp("abz", "abd");  // 1
  ft_strcmp("abd", "abz");  // -1
  ```

### ft_write
**Dosya:** `ft_write.s`

Sistem write çağrısı wrapper'ı.
- **Fonksiyon:** `ft_write(int fd, const void *buf, size_t count) -> ssize_t`
- **Açıklama:** Dosya descriptor'a veri yazar, hata yönetimi dahil
- **Sistem Çağrısı:** syscall 1 (SYS_write)
- **Hata Yönetimi:** errno set eder, -1 döndürür
- **Extern:** `__errno_location` (errno pointer'ı için)
- **Örnek:**
  ```c
  ssize_t ret = ft_write(1, "Hello\n", 6); // stdout'a yaz
  ```

### ft_read
**Dosya:** `ft_read.s`

Sistem read çağrısı wrapper'ı.
- **Fonksiyon:** `ft_read(int fd, void *buf, size_t count) -> ssize_t`
- **Açıklama:** Dosya descriptor'dan veri okur, hata yönetimi dahil
- **Sistem Çağrısı:** syscall 0 (SYS_read)
- **Hata Yönetimi:** errno set eder, -1 döndürür
- **Extern:** `__errno_location` (errno pointer'ı için)
- **Örnek:**
  ```c
  char buffer[100];
  ssize_t ret = ft_read(0, buffer, 99); // stdin'den oku
  ```

### ft_strdup
**Dosya:** `ft_strdup.s`

String duplicate fonksiyonu.
- **Fonksiyon:** `ft_strdup(const char *s) -> char *`
- **Açıklama:** Verilen string'in heap'te kopyasını oluşturur
- **Extern:** `malloc`, `ft_strlen`, `ft_strcpy`, `__errno_location`
- **Hata Yönetimi:** malloc başarısız olursa NULL döndürür, errno=12 (ENOMEM)
- **Bellek Yönetimi:** Caller tarafından free edilmeli
- **Örnek:**
  ```c
  char *dup = ft_strdup("original");
  // kullan...
  free(dup);
  ```

## 🎯 Bonus Fonksiyonlar

### ft_atoi_base
**Dosya:** `ft_atoi_base_bonus.s`

Belirtilen tabanda string'i integer'a çeviren fonksiyon.
- **Fonksiyon:** `ft_atoi_base(const char *str, const char *base) -> int`
- **Açıklama:** Farklı sayı tabanlarında (minimum base 2) string'den integer çevrimi
- **Base Kontrolü:** 
  - Minimum 2 karakter
  - Duplicate karakter yok
  - +, -, whitespace ve tab karakterleri yok (ASCII 9-13)
- **Özellikler:**
  - Whitespace skip
  - Sign handling (+/-)
  - Invalid karakter durumunda durdurma
- **Örnek:**
  ```c
  ft_atoi_base("42", "0123456789");     // 42 (base 10)
  ft_atoi_base("2A", "0123456789ABCDEF"); // 42 (base 16)
  ft_atoi_base("1010", "01");           // 10 (base 2)
  ```

### ft_list_push_front
**Dosya:** `ft_list_push_front_bonus.s`

Linked list'in başına eleman ekleyen fonksiyon.
- **Fonksiyon:** `ft_list_push_front(t_list **begin_list, void *data) -> void`
- **Açıklama:** Yeni node oluşturur ve listenin başına ekler
- **Struct:** `t_list { void *data; struct s_list *next; }`
- **Extern:** `malloc`, `__errno_location`
- **Bellek:** 16 byte allocate (data pointer + next pointer)
- **Hata Yönetimi:** malloc başarısız olursa errno=12
- **Örnek:**
  ```c
  t_list *list = NULL;
  ft_list_push_front(&list, "data");
  ```

### ft_list_size
**Dosya:** `ft_list_size_bonus.s`

Linked list'in boyutunu hesaplayan fonksiyon.
- **Fonksiyon:** `ft_list_size(t_list *begin_list) -> int`
- **Açıklama:** Liste elemanlarını sayar ve toplam sayıyı döndürür
- **Algoritma:** İteratif traversal, counter artırma
- **Kompleksite:** O(n)
- **Örnek:**
  ```c
  int size = ft_list_size(list); // liste boyutu
  ```

### ft_list_sort
**Dosya:** `ft_list_sort_bonus.s`

Linked list'i sıralayan fonksiyon.
- **Fonksiyon:** `ft_list_sort(t_list **begin_list, int (*cmp)()) -> void`
- **Açıklama:** Bubble sort algoritması kullanarak listeyi sıralar
- **Algoritma:** Bubble sort (adjacent element swapping)
- **Compare Function:** Kullanıcı tanımlı karşılaştırma fonksiyonu
- **Kompleksite:** O(n²)
- **Örnek:**
  ```c
  ft_list_sort(&list, strcmp); // alfabetik sıralama
  ```

### ft_list_remove_if
**Dosya:** `ft_list_remove_if_bonus.s`

Koşula uyan elemanları listeden çıkaran fonksiyon.
- **Fonksiyon:** `ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *)) -> void`
- **Açıklama:** Compare fonksiyonu 0 döndüren elemanları siler
- **Extern:** `free`
- **Parametreler:**
  - `data_ref`: Karşılaştırma referansı
  - `cmp`: Karşılaştırma fonksiyonu
  - `free_fct`: Data için free fonksiyonu
- **Örnek:**
  ```c
  ft_list_remove_if(&list, "target", strcmp, free_data);
  ```

## 🔧 Derleme ve Kullanım

### Gereksinimler
```bash
# Ubuntu/Debian
sudo apt-get install nasm gcc make

# Arch Linux
sudo pacman -S nasm gcc make
```

### Makefile Komutları
```bash
# Temel kütüphane oluşturma
make

# Bonus fonksiyonları dahil etme
make bonus

# Test programı çalıştırma (bonus fonksiyonları dahil)
make test

# Valgrind ile test
make vtest

# Temizlik
make clean
make fclean

# Yeniden derleme
make re
```

### Kütüphane Kullanımı
```c
// Programınızda include
#include "libasm.h"

// Derleme
gcc -Wall -Wextra -Werror main.c -L. -lasm -o program
```

### Assembly Derleme
```bash
# Tek dosya derleme
nasm -f elf64 ft_strlen.s -o ft_strlen.o

# Arşiv oluşturma
ar rcs libasm.a *.o
```

## 📋 Teknik Detaylar

### Assembly Konvansiyonları
- **Calling Convention:** System V AMD64 ABI
- **Parametreler:** RDI, RSI, RDX, RCX, R8, R9
- **Dönüş:** RAX
- **Preserve:** RBX, RSP, RBP, R12-R15
- **Scratch:** RAX, RCX, RDX, RSI, RDI, R8-R11

### Sistem Çağrıları
- **Linux x86_64 syscall ABI**
- **SYS_read:** rax=0
- **SYS_write:** rax=1
- **Parametreler:** rdi, rsi, rdx, r10, r8, r9

### Bellek Yönetimi
- **malloc/free:** C library fonksiyonları
- **Error handling:** errno kullanımı
- **PLT (Procedure Linkage Table):** External function calls

### Linked List Yapısı
```c
typedef struct s_list {
    void *data;           // 8 bytes
    struct s_list *next;  // 8 bytes
} t_list;                 // toplam 16 bytes
```

## 🎯 Öğrenilen Kavramlar

1. **Assembly Programming**: x86_64 assembly dili syntax ve semantiği
2. **System Calls**: Linux kernel interface kullanımı
3. **Memory Management**: malloc/free ve pointer manipülasyonu
4. **Error Handling**: errno mekanizması ve hata yakalama
5. **Calling Conventions**: Function parameter passing ve return values
6. **Data Structures**: Linked list implementasyonu ve manipulation
7. **Algorithm Implementation**: Sorting ve searching algoritmaları
8. **Binary Operations**: Bit-level operations ve optimizasyonlar

## 💡 Performans Notları

- **ft_strlen**: Linear O(n) complexity, cache-friendly
- **ft_strcmp**: Early termination on first difference
- **ft_list_sort**: Bubble sort - O(n²), simple but inefficient for large lists
- **System calls**: Minimal overhead, direct kernel interface
- **Memory allocation**: Error checking ve proper cleanup

## 🔍 Debug ve Test

### Test Programı
`main.c` dosyası tüm fonksiyonların kapsamlı testini içerir:
- Basic functionality tests
- Error condition handling
- Memory leak checking (valgrind)
- Performance comparison

### Debug Teknikleri
```bash
# GDB ile debug
gdb ./test
# Valgrind ile memory check
valgrind --leak-check=full ./test
# Objdump ile disassembly
objdump -d ft_strlen.o
```
