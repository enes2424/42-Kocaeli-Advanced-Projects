# 42-Kocaeli-Malloc
## Dynamic Memory Allocation Implementation (Dinamik Bellek Yönetimi Implementasyonu)

Bu proje, 42 Okulu müfredatının ileri seviye projelerinden biridir. C dilinde standart malloc, free, realloc fonksiyonlarının kendi implementasyonunu yapmayı amaçlar. Bellek yönetimi, zone tabanlı allocation, thread safety ve debugging özellikleri içerir.

## 🏗️ Mimari

### Zone Tabanlı Bellek Yönetimi
Proje, bellek ayırmalarını üç farklı kategoride yönetir:

- **TINY**: ≤ 256 byte allocations (16 * pagesize zone'lar)
- **SMALL**: 257 byte - 4KB allocations (128 * pagesize zone'lar)  
- **LARGE**: > 4KB allocations (mmap ile direkt allocation)

### Veri Yapıları

#### t_block
```c
typedef struct s_block
{
    size_t          size;        // Block'un payload boyutu
    bool            free;        // Block'un serbest olup olmadığı
    struct s_block  *next;       // Sonraki block pointer'ı
}   t_block;
```

#### t_zone
```c
typedef struct s_zone
{
    size_t          size;        // Zone'un kullanılabilir boyutu
    struct s_zone   *next;       // Sonraki zone pointer'ı
    void            *ptr;        // İlk block/payload pointer'ı
}   t_zone;
```

#### t_memory_management
```c
typedef struct s_memory_management
{
    t_zone  *tiny_zones;         // TINY zone'ların listesi
    t_zone  *small_zones;        // SMALL zone'ların listesi
    t_zone  *large_zones;        // LARGE zone'ların listesi
    size_t  total_used_size;     // Toplam kullanılan bellek
}   t_memory_management;
```

## 📚 Fonksiyonlar

### malloc - Bellek Ayırma
**Dosya:** `src/malloc.c`

Dinamik bellek ayırma fonksiyonu.
- **Fonksiyon:** `void *malloc(size_t size)`
- **Açıklama:** İstenen boyutta bellek ayırır, boyuta göre TINY/SMALL/LARGE kategorisine yerleştirir
- **Thread Safety:** pthread_mutex ile korunur
- **Örnek:**
  ```c
  void *ptr = malloc(64);      // TINY allocation
  void *ptr2 = malloc(2048);   // SMALL allocation
  void *ptr3 = malloc(8192);   // LARGE allocation
  ```

### free - Bellek Serbest Bırakma
**Dosya:** `src/free.c`

Ayrılan belleği serbest bırakır.
- **Fonksiyon:** `void free(void *ptr)`
- **Açıklama:** Verilen pointer'ı serbest bırakır, adjacent block'ları birleştirir
- **Güvenlik:** NULL pointer kontrolü, double-free koruması
- **Örnek:**
  ```c
  void *ptr = malloc(100);
  free(ptr);  // Belleği serbest bırak
  free(ptr);  // Güvenli - hata mesajı verir
  ```

### realloc - Bellek Yeniden Boyutlandırma
**Dosya:** `src/realloc.c`

Mevcut bellek bloğunu yeniden boyutlandırır.
- **Fonksiyon:** `void *realloc(void *ptr, size_t size)`
- **Açıklama:** Mevcut bellek bloğunu genişletir/daraltır veya yeni allocation yapar
- **Optimizasyon:** Mümkünse in-place resize, yoksa copy
- **Örnek:**
  ```c
  void *ptr = malloc(100);
  ptr = realloc(ptr, 200);  // Boyutu artır
  ptr = realloc(ptr, 50);   // Boyutu azalt
  ```

### show_alloc_mem - Bellek Durumu Görüntüleme
**Dosya:** `src/show_alloc_mem.c`

Mevcut bellek durumunu görüntüler.
- **Fonksiyon:** `void show_alloc_mem()`
- **Açıklama:** Tüm zone'ları ve block'ları listeler, toplam kullanım gösterir
- **Çıktı Format:**
  ```
  TINY : 0x7f8b13c00000
  0x7f8b13c00020 - 0x7f8b13c00030 : 16 bytes
  SMALL : 0x7f8b13c01000
  0x7f8b13c01020 - 0x7f8b13c01420 : 1024 bytes
  Total : 1040 bytes
  ```

## 🎁 Bonus Özellikler

### show_alloc_mem_ex - Genişletilmiş Bellek Görüntüleme
**Dosya:** `bonus/src/show_alloc_mem_ex.c`

Bellek içeriğini hexdump formatında gösterir.
- **Fonksiyon:** `void show_alloc_mem_ex()`
- **Açıklama:** Her block'un içeriğini hex ve ASCII formatında görüntüler
- **Çıktı Format:**
  ```
  TINY : 0x7f8b13c00000
  0x7f8b13c00020 - 0x7f8b13c00030 : 16 bytes
  48656c6c 6f20576f 726c6421 00000000  Hello World!....
  Total : 16 bytes
  ```

### MALLOC_SCRIBBLE - Debug Desteği
**Dosya:** `bonus/src/alloc.c`

Serbest bırakılan belleği 0x55 pattern ile doldurur.
- **Fonksiyon:** `void scribble_block(t_block *block)`
- **Açıklama:** Çevre değişkeni MALLOC_SCRIBBLE=1 olduğunda aktif olur
- **Amaç:** Use-after-free bug'larını tespit etmeyi kolaylaştırır
- **Örnek:**
  ```bash
  export MALLOC_SCRIBBLE=1
  ./program  # Freed memory 0x55 ile doldurulur
  ```

## 🔧 Kullanım

### Derleme ve Kurulum
```bash
# Ana projeyi derle
make

# Bonus versiyonu derle  
make bonus

# Test programını derle ve çalıştır
make test

# Bonus test programını derle ve çalıştır
cd bonus && make comp && LD_LIBRARY_PATH=. ./malloc
```

### Kütüphane Kullanımı
```bash
# Programınızı malloc kütüphanesi ile derle
gcc -L. -lft_malloc your_program.c -o your_program

# Kütüphaneyi yükleyerek çalıştır
LD_LIBRARY_PATH=. ./your_program
```

### Çevre Değişkenleri
```bash
# Memory scribbling aktif et (bonus)
export MALLOC_SCRIBBLE=1
```

### Test Senaryoları
```c
#include "includes/malloc.h"

int main() {
    // Temel kullanım
    void *ptr1 = malloc(100);    // TINY allocation
    void *ptr2 = malloc(2000);   // SMALL allocation  
    void *ptr3 = malloc(8000);   // LARGE allocation
    
    // Bellek durumunu görüntüle
    show_alloc_mem();
    
    // Realloc test
    ptr1 = realloc(ptr1, 200);
    
    // Cleanup
    free(ptr1);
    free(ptr2);
    free(ptr3);
    
    return 0;
}
```

## 🎯 Teknik Detaylar

### Zone Boyutları
- **TINY Zone**: 16 × page_size (genellikle 65,536 bytes)
- **SMALL Zone**: 128 × page_size (genellikle 524,288 bytes)
- **Block Overhead**: sizeof(t_block) = 24 bytes (64-bit sistemde)

### Allocation Algoritması
1. **Boyut Kontrolü**: İstenen boyuta göre TINY/SMALL/LARGE belirlenir
2. **Zone Arama**: Mevcut zone'larda uygun boş block aranır
3. **Block Split**: Gerekirse büyük block'lar bölünür
4. **Yeni Zone**: Uygun zone yoksa yeni zone oluşturulur

### Free Algoritması
1. **Pointer Validasyonu**: Verilen pointer'ın geçerliliği kontrol edilir
2. **Block Marking**: Block serbest olarak işaretlenir
3. **Coalescing**: Komşu serbest block'lar birleştirilir
4. **Zone Cleanup**: Tamamen boş zone'lar kaldırılır

### Thread Safety
- **Global Mutex**: `pthread_mutex_t g_memory_lock`
- **Atomic Operations**: Tüm malloc/free/realloc işlemleri korunur
- **Race Condition Prevention**: Kritik bölgelerin mutex ile korunması

## 🧪 Test Stratejisi

### Temel Testler
1. **Basic Allocation**: TINY/SMALL/LARGE allocation'lar
2. **Free Operations**: Normal ve edge case free'ler
3. **Realloc Tests**: Grow/shrink/move operations
4. **Memory Visualization**: show_alloc_mem çıktı kontrolü

### Stress Testler
1. **Fragmentation**: Yoğun alloc/free döngüleri
2. **Large Allocations**: Multi-megabyte allocation'lar
3. **Thread Safety**: Multi-threaded allocation testleri
4. **Edge Cases**: NULL pointer'lar, 0 boyut, overflow

### Debug Testler (Bonus)
1. **Scribble Testing**: Use-after-free detection
2. **Hexdump Verification**: Memory content validation
3. **Corruption Detection**: Heap corruption testleri

## 📋 Öğrenilen Kavramlar

1. **Bellek Yönetimi**: Virtual memory, page alignment, mmap kullanımı
2. **Veri Yapıları**: Linked list'ler, zone management, block chaining
3. **Algoritma Optimizasyonu**: First-fit, coalescing, fragmentation prevention
4. **System Programming**: POSIX API'ları, thread safety, error handling
5. **Debugging Teknikleri**: Memory leak detection, corruption analysis
6. **Performance Analysis**: Allocation patterns, cache efficiency
7. **Library Design**: Shared library creation, symbol visibility, ABI compatibility

## ⚠️ Bilinen Sınırlamalar

- **Alignment**: 8-byte alignment zorunlu (64-bit sistemlerde)
- **Maximum Size**: SIZE_MAX sınırı, integer overflow koruması
- **Thread Contention**: Yoğun multi-threaded kullanımda performance düşüşü
- **Fragmentation**: Extreme fragmentation durumlarında bellek verimsizliği

## 📋 Notlar

- Tüm fonksiyonlar POSIX.1-2001 standartlarına uyumludur
- Thread safety pthread kütüphanesi ile sağlanır
- Zone tabanlı yaklaşım fragmentation'ı minimize eder
- Debug özellikleri development sürecinde kritik bilgi sağlar
- Performance optimizasyonları production kullanımı için uygundur 
