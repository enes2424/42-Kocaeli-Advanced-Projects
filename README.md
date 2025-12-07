# 42 Kocaeli ft_ping
## Network Programming and ICMP Protocol (Ağ Programlama ve ICMP Protokolü)

Bu proje, 42 Okulu'nun sistem ve ağ programlama projesidir. ICMP (Internet Control Message Protocol) protokolü kullanarak network host'larına ping gönderen bir uygulama geliştirmeyi amaçlar. Raw socket programlama, paket manipülasyonu, ve düşük seviye network protokolleri konularında uzmanlaşmayı sağlar.

## 📚 Modüller ve Dosyalar

### main.c
**Açıklama:** Uygulamanın giriş noktası
- **İçerik:** Program argümanlarını parse eder ve ping döngüsünü başlatır
- **Varsayılan Değerler:**
  - Paket boyutu: 64 byte
  - Paket gönderim aralığı: 1 saniye
- **İşlevler:**
  - Komut satırı argümanlarını kontrol eder
  - IP adresini çözer ve sockaddr_in yapısını başlatır
  - Ping döngüsünü başlatır ve birden fazla host desteği sağlar

### ft_ping.h
**Açıklama:** Projenin ana header dosyası
- **Sabitler:**
  - `PACKET_BUFFER_SIZE`: 65535 byte (maksimum IP paket boyutu)
  - `ICMP_PATTERN`: 0x42 (paket payload'ında kullanılan desen)
- **Yapılar:**
  - `t_options`: Kullanıcı seçeneklerini tutar (verbose, count, interval, timeout, preload, size)
  - `t_ping_ctx`: Ping bağlamını tutar (socket, paket, dup kontrolü, istatistikler)
- **Tüm fonksiyon prototipleri bu dosyada tanımlanmıştır**

### args_control.c
**Açıklama:** Komut satırı argüman işleme ve doğrulama modülü
- **Fonksiyonlar:**
  - `control_args()`: Ana argüman kontrol fonksiyonu
  - `try_convert_int()`: String'i integer'a dönüştürür ve hata kontrolü yapar
  - `try_convert_double()`: String'i double'a dönüştürür
  - Çeşitli hata mesajı fonksiyonları
- **Desteklenen Seçenekler:**
  - `-c, --count`: Gönderilecek paket sayısı
  - `-i, --interval`: Paketler arası bekleme süresi (minimum 0.2 saniye)
  - `-v, --verbose`: Detaylı çıktı
  - `-w, --timeout`: Timeout süresi (saniye)
  - `-l, --preload`: Hızlı gönderim için paket sayısı (root gerektirir)
  - `-s, --size`: Gönderilecek veri byte sayısı (max 65399)
  - `-?, --help`: Yardım mesajı
  - `--usage`: Kısa kullanım bilgisi
- **Validasyon:**
  - Sayısal değer kontrolü
  - Aralık kontrolü (min/max değerler)
  - Geçersiz karakter kontrolü
  - Belirsiz seçenek kontrolü

### usage_option.c
**Açıklama:** Kısa kullanım mesajı
- **Fonksiyon:** `usage_option()`
- **Çıktı:** Program kullanım sözdizimini gösterir
- **Format:** Tüm seçeneklerin kısa bir özeti

### help_option.c
**Açıklama:** Detaylı yardım mesajı
- **Fonksiyon:** `help_option()`
- **Çıktı:** Tüm seçeneklerin detaylı açıklamaları
- **İçerik:**
  - Her seçeneğin kısa ve uzun versiyonu
  - Seçenek açıklamaları
  - Root gereksinimi olan seçenekler işaretli
  - Bug raporlama bilgisi

### split_ip.c
**Açıklama:** IP adresi string manipülasyon modülü
- **Fonksiyonlar:**
  - `split_ip()`: IP adresini nokta ile ayırır
  - `free_double_array()`: Dinamik string array'i temizler
  - `count_words()`: Nokta ile ayrılmış parça sayısını hesaplar
  - `ft_substr()`: Substring oluşturur
- **Validasyon:**
  - Maksimum 4 oktet kontrolü
  - Başta/sonda nokta kontrolü
  - Ardışık nokta kontrolü
  - Bellek hatası kontrolü

### init_addr.c
**Açıklama:** DNS çözümleme ve adres yapısı başlatma
- **Fonksiyon:** `init_addr()`
- **İşlevler:**
  - `getaddrinfo()` ile DNS çözümlemesi
  - Hostname'den IP adresine dönüşüm
  - `sockaddr_in` yapısını doldurur
  - IPv4 adres validasyonu
- **Hata Kontrolü:**
  - Bilinmeyen host hataları
  - Bellek tahsis hataları
  - IP dönüşüm hataları

### socket_setup.c
**Açıklama:** Raw socket oluşturma ve yapılandırma
- **Fonksiyonlar:**
  - `create_socket()`: Raw ICMP socket oluşturur
  - `setup_socket_options()`: Socket seçeneklerini ayarlar
- **Socket Özellikleri:**
  - Family: AF_INET (IPv4)
  - Type: SOCK_RAW (raw socket)
  - Protocol: IPPROTO_ICMP
  - Option: IP_RECVTTL (TTL bilgisi almak için)
- **Not:** Raw socket oluşturmak için root yetkisi gerekir

### packet.c
**Açıklama:** ICMP paket oluşturma ve checksum hesaplama
- **Fonksiyonlar:**
  - `icmp_checksum()`: RFC 1071 uyumlu checksum hesaplar
  - `init_icmp_packet()`: ICMP header'ı başlatır
  - `prepare_packet()`: Tam paketi hazırlar
- **ICMP Paket Yapısı:**
  - Type: ICMP_ECHO (8)
  - Code: 0
  - Checksum: Hesaplanmış değer
  - ID: Process ID (PID)
  - Sequence: Paket sıra numarası
- **Payload:**
  - Timestamp (timespec yapısı) - RTT hesabı için
  - Pattern (0x42) ile doldurulmuş veri

### ping_loop.c
**Açıklama:** Ana ping döngüsü ve signal yönetimi
- **Fonksiyonlar:**
  - `ping_loop()`: Ana ping döngüsü
  - `signal_control()`: SIGINT (Ctrl+C) handler
  - `print_ping_header()`: Ping başlık bilgisi
  - `resize_dup_array()`: Duplicate kontrol array'ini büyütür
  - `send_packet()`: Tek bir ICMP paketi gönderir
- **Akış:**
  1. Socket oluşturma ve yapılandırma
  2. ICMP paketi için bellek ayırma
  3. Signal handler'ları kurma (SIGINT, SIGALRM)
  4. Preload paketlerini gönderme
  5. Normal ping döngüsü (paket gönder + cevap bekle)
  6. Count veya timeout kontrolü
  7. İstatistikleri yazdırma
- **Özellikler:**
  - Dinamik duplicate array boyutlandırma
  - Preload desteği (hızlı paket gönderimi)
  - Timeout ve count limitleri
  - Interrupt handling

### receive.c
**Açıklama:** ICMP yanıt alma ve işleme modülü
- **Fonksiyonlar:**
  - `receive_replies()`: Belirli süre boyunca cevap bekler
  - `process_reply()`: Alınan paketi işler
  - `print_icmp_error()`: ICMP hata mesajlarını yazdırır
- **İşlenen ICMP Tipleri:**
  - ICMP_ECHOREPLY: Normal ping yanıtı
  - ICMP_DEST_UNREACH: Hedef erişilemez (7 alt tip)
  - ICMP_TIME_EXCEEDED: TTL aşıldı
  - ICMP_PARAMETERPROB: Parametre problemi
  - ICMP_SOURCE_QUENCH: Kaynak durdu
  - ICMP_REDIRECT: Yönlendirme (4 alt tip)
- **Validasyonlar:**
  - IP header uzunluğu kontrolü
  - ICMP checksum doğrulama
  - Packet ID ve sequence kontrolü
  - Truncated paket kontrolü
  - Duplicate detection
- **RTT Hesaplama:**
  - Monotonic clock kullanımı
  - Nanosaniye hassasiyeti
  - Milisaniye olarak gösterim
- **Verbose Mod:**
  - IP header dump (hex format)
  - Header detayları (Version, TOS, TTL, Protocol, etc.)
  - ICMP detayları

### statistics.c
**Açıklama:** Ping istatistiklerini hesaplama ve gösterme
- **Fonksiyon:** `print_statistics()`
- **İstatistikler:**
  - Gönderilen paket sayısı
  - Alınan paket sayısı
  - Duplicate paket sayısı
  - Paket kaybı yüzdesi
- **Format:** GNU ping uyumlu çıktı

### cleanup.c
**Açıklama:** Kaynak temizleme ve bellek yönetimi
- **Fonksiyonlar:**
  - `cleanup_resources()`: Tüm kaynakları temizler
  - `cleanup_and_exit()`: Temizlik yapıp programı sonlandırır
- **Temizlenen Kaynaklar:**
  - Socket file descriptor
  - Paket buffer'ı
  - Duplicate array
  - IP adresi string'i

### handle_remaining_hosts.c
**Açıklama:** Birden fazla host parametresi yönetimi
- **Fonksiyon:** `handle_remaining_hosts()`
- **İşlev:** Komut satırında birden fazla host verildiğinde, ilk host'tan sonrakileri işler
- **Özellik:** Her host için ayrı DNS çözümlemesi ve başlık gösterimi (ancak ping gönderilmez)

### ft_ping.c
**Açıklama:** Global değişken tanımlamaları
- **Global:** `volatile sig_atomic_t is_open`
- **Amaç:** Signal handler ile ana döngü arasında güvenli iletişim
- **Değer:** 1 (çalışıyor), 0 (durduruldu)

## 🔧 Kullanım

### Derleme
```bash
# Projeyi derle
make

# Temizlik
make clean    # Obje dosyalarını sil
make fclean   # Tüm derlenmiş dosyaları sil
make re       # Yeniden derle
```

### Çalıştırma
```bash
# Temel kullanım (root gerekli)
sudo ./ft_ping google.com

# 5 paket gönder
sudo ./ft_ping -c 5 8.8.8.8

# 0.5 saniye aralıkla ping at
sudo ./ft_ping -i 0.5 localhost

# Verbose mod
sudo ./ft_ping -v google.com

# 10 saniye timeout
sudo ./ft_ping -w 10 github.com

# Özel paket boyutu (100 byte data)
sudo ./ft_ping -s 100 1.1.1.1

# Birden fazla seçenek
sudo ./ft_ping -v -c 10 -i 0.3 google.com

# Birden fazla host
sudo ./ft_ping google.com github.com

# Yardım
./ft_ping --help
./ft_ping --usage
```

### Örnek Çıktılar
```
PING google.com (142.250.185.78): 64 data bytes
72 bytes from 142.250.185.78: icmp_seq=0 ttl=117 time=13.456 ms
72 bytes from 142.250.185.78: icmp_seq=1 ttl=117 time=12.987 ms
72 bytes from 142.250.185.78: icmp_seq=2 ttl=117 time=13.123 ms
^C
--- google.com ping statistics ---
3 packets transmitted, 3 packets received, 0% packet loss
```

## 🎯 Öğrenilen Kavramlar

1. **Raw Socket Programlama**: Düşük seviye socket API kullanımı
2. **ICMP Protokolü**: Internet Control Message Protocol detayları
3. **Network Byte Order**: htons(), ntohs(), inet_ntop() kullanımı
4. **DNS Çözümleme**: getaddrinfo() ile hostname çözümleme
5. **Checksum Algoritması**: RFC 1071 uyumlu checksum hesaplama
6. **Signal Handling**: SIGINT, SIGALRM ile asenkron işlemler
7. **Time Management**: clock_gettime(), CLOCK_MONOTONIC kullanımı
8. **Packet Analysis**: IP ve ICMP header parsing
9. **Select System Call**: Non-blocking I/O ve timeout yönetimi
10. **Memory Management**: Dinamik bellek tahsisi ve temizleme
11. **Error Handling**: Sistem çağrısı hata kontrolü ve reporting
12. **String Parsing**: Komut satırı argüman işleme
13. **Binary Protocol**: Struct packing ve network protokol yapıları

## 🌐 Teknik Detaylar

### ICMP Echo Request/Reply
- **Type 8 (Echo Request)**: Ping gönderme
- **Type 0 (Echo Reply)**: Ping yanıtı
- **Identifier**: Process ID ile belirlenir
- **Sequence Number**: Her paket için artırılır
- **Data**: Timestamp + pattern ile doldurulur

### Checksum Hesaplama
```
1. Paketi 16-bit word'lere böl
2. Tüm word'leri topla
3. Overflow bitlerini ekle
4. 1'e tamamlayıcısını al
```

### RTT (Round Trip Time) Ölçümü
```
1. Paket gönderirken monotonic timestamp ekle
2. Cevap geldiğinde yeni timestamp al
3. Farkı hesapla (ns hassasiyetinde)
4. Milisaniyeye çevir
```

### Duplicate Detection
- Her sequence number için sayaç tutulur
- İkinci aynı sequence geldiğinde DUP! işareti konur
- Duplicate paketler istatistiklerde ayrı gösterilir

## 📋 Önemli Notlar

- **Root Yetkisi:** Raw socket kullanımı için `sudo` gereklidir
- **Minimum Interval:** Flood attack önlemek için minimum 0.2 saniye (non-root için)
- **Preload Limiti:** Sadece root kullanıcısı preload seçeneğini kullanabilir
- **Paket Boyutu:** ICMP header (8 byte) otomatik eklenir, maksimum 65399 byte
- **Thread Safety:** Signal handler'da sadece atomic operasyonlar kullanılır
- **Platform:** Linux sistemler için geliştirilmiştir
- **Standart Uyumluluk:** GNU ping davranışına uyumludur

## 🔍 Hata Ayıklama

```bash
# Detaylı hata mesajları için verbose mod
sudo ./ft_ping -v problematic-host.com

# Packet drop test için büyük paket
sudo ./ft_ping -s 2000 -c 5 host.com

# Network timeout test
sudo ./ft_ping -w 5 -c 100 slow-host.com

# Memory leak kontrolü
valgrind --leak-check=full sudo ./ft_ping -c 10 localhost
```

## 📚 Referanslar

- RFC 792: Internet Control Message Protocol
- RFC 1071: Computing the Internet Checksum
- POSIX Socket API Documentation
- Linux Raw Socket Programming Guide
