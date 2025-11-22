# 42 Kocaeli Hangouts Projesi
## Flutter SMS ve Kişi Yönetimi Uygulaması

Bu proje, 42 Okulu advanced projelerinden biridir. Flutter framework'ü kullanarak Android platformu için SMS mesajlaşma ve kişi yönetimi uygulaması geliştirmeyi amaçlar. Native Android kodları (Kotlin) ile Flutter arasında köprü kurarak platform özelliklerini kullanmayı öğretir.

## 📱 Özellikler

### Ana Özellikler
- **Kişi Yönetimi**: Kişi ekleme, düzenleme, silme ve görüntüleme
- **SMS Mesajlaşma**: SMS gönderme ve alma, gerçek zamanlı mesaj bildirimleri
- **Çok Dilli Destek**: İngilizce ve Türkçe dil desteği
- **Özelleştirilebilir Tema**: Kullanıcı tarafından seçilebilir renk temaları
- **Veritabanı Entegrasyonu**: SQLite ile veri saklama
- **Native Platform Entegrasyonu**: Android özelliklerinin kullanımı

## 📚 Dosya Yapısı ve Modüller

### Flutter Dart Dosyaları

#### main.dart
**Dosya:** `lib/main.dart`

Uygulamanın giriş noktası ve ana konfigürasyonu.
- **Sınıf:** `MyApp extends StatelessWidget`
- **Açıklama:** Material Design kullanarak ana uygulama widget'ını oluşturur
- **Özellikler:** 
  - Debug banner'ını gizler
  - Ana sayfa olarak `HomePage`'i ayarlar
  - Uygulama başlığını "Hangouts" olarak belirler

#### models/contact.dart
**Dosya:** `lib/src/models/contact.dart`

Kişi veri modeli ve yönetim fonksiyonları.
- **Sınıf:** `Contact`
- **Özellikler:** `id`, `name`, `surname`, `nickname`, `phoneNumber`, `email`, `profilePicture`
- **Fonksiyonlar:**
  - `getAllContacts()`: Tüm kişileri veritabanından yükler
  - `addContact()`: Yeni kişi ekler
  - `updateContact()`: Mevcut kişiyi günceller
  - `deleteContact()`: Kişi siler
  - `profileAvatar()`: Profil avatar widget'ı oluşturur
  - `card()`: Kişi kartı UI component'i

#### models/sms.dart
**Dosya:** `lib/src/models/sms.dart`

SMS mesaj veri modeli.
- **Sınıf:** `SmsMessage`
- **Özellikler:** `contactId`, `status`, `body`, `timestamp`
- **Açıklama:** SMS mesajlarını temsil eden basit veri sınıfı
- **Kullanım:** Gelen ve giden mesajları saklamak için kullanılır

#### models/app_words.dart
**Dosya:** `lib/src/models/app_words.dart`

Çok dilli destek için metin tanımlamaları.
- **Yapı:** `Map<String, Map<String, String>>`
- **Diller:** İngilizce (`en`) ve Türkçe (`tr`)
- **İçerik:** UI metinleri, hata mesajları, form etiketleri
- **Örnek:**
  ```dart
  appWords['en']['contacts'] = 'Contacts'
  appWords['tr']['contacts'] = 'Kişiler'
  ```

#### pages/home_page.dart
**Dosya:** `lib/src/pages/home_page.dart`

Ana sayfa ve kişi listesi görüntüleme.
- **Sınıf:** `HomePage extends StatefulWidget`
- **Özellikler:**
  - Event channel'lar ile native koddan veri alma
  - Gerçek zamanlı SMS alma bildirimleri
  - Kişi listesi ve genişletilebilir kartlar
  - Renk teması yönetimi
  - Dil değiştirme ayarları
- **Native Entegrasyon:**
  - SMS alma event'leri
  - Veri yenileme event'leri
  - Renk ayarları method channel'ı

#### pages/chat_page.dart
**Dosya:** `lib/src/pages/chat_page.dart`

Birebir mesajlaşma sayfası.
- **Sınıf:** `ChatPage extends StatefulWidget`
- **Özellikler:**
  - Mesaj geçmişi görüntüleme
  - Yeni mesaj gönderme
  - Gerçek zamanlı mesaj alma
  - Otomatik scroll yönetimi
  - Mesaj durumu takibi
- **UI Bileşenleri:**
  - Mesaj baloncukları
  - Metin giriş alanı
  - Gönder butonu
  - Scroll controller

#### pages/info_page.dart
**Dosya:** `lib/src/pages/info_page.dart`

Kişi bilgileri görüntüleme ve düzenleme sayfası.
- **Sınıf:** `InfoPage extends StatefulWidget`
- **Özellikler:**
  - Kişi detaylarını görüntüleme
  - Kişi bilgileri düzenleme
  - Profil resmi değiştirme
  - Kişi silme işlemi
  - Arama yapma fonksiyonu

#### sms/sms_helper.dart
**Dosya:** `lib/src/sms/sms_helper.dart`

SMS işlemleri için yardımcı sınıf.
- **Sınıf:** `SmsHelper`
- **Fonksiyonlar:**
  - `sendSms()`: SMS gönderme
  - `getAllMessages()`: Tüm mesajları alma
- **Native Entegrasyon:** Method channel kullanarak Kotlin kodları ile iletişim

#### widgets/show_dialog.dart
**Dosya:** `lib/src/widgets/show_dialog.dart`

Özel dialog widget'ları.
- **Fonksiyonlar:**
  - `showAddContactDialog()`: Kişi ekleme dialogu
  - `showEditContactDialog()`: Kişi düzenleme dialogu
  - `showSettingsDialog()`: Ayarlar dialogu
  - `showDeleteConfirmDialog()`: Silme onay dialogu
- **Form Validasyonu:** Girdi doğrulama ve hata mesajları

#### widgets/snackbar.dart
**Dosya:** `lib/src/widgets/snackbar.dart`

Özel snackbar bildirimleri.
- **Enum:** `SnackBarType` (success, info, error)
- **Fonksiyon:** `showCustomSnackBar()`
- **Özellikler:**
  - Farklı renklerde bildirimler
  - Otomatik gizlenme
  - Floating tasarım
  - Tekrar eden mesaj engelleme

### Android Kotlin Dosyaları

#### MainActivity.kt
**Dosya:** `android/app/src/main/kotlin/com/example/hangouts/MainActivity.kt`

Ana Android activity ve Flutter bridge.
- **Sınıf:** `MainActivity : FlutterActivity`
- **Özellikler:**
  - İzin yönetimi (SMS, telefon, bildirim)
  - Method channel ile Flutter iletişimi
  - Event channel ile gerçek zamanlı veri aktarımı
  - Veritabanı işlemleri
  - SMS gönderme ve alma
  - Renk ayarları yönetimi
- **İzinler:**
  ```kotlin
  SEND_SMS, CALL_PHONE, RECEIVE_SMS, 
  READ_SMS, READ_EXTERNAL_STORAGE, POST_NOTIFICATIONS
  ```

#### DbHelper.kt
**Dosya:** `android/app/src/main/kotlin/com/example/hangouts/DbHelper.kt`

SQLite veritabanı yardımcı sınıfı.
- **Sınıf:** `DbHelper : SQLiteOpenHelper`
- **Tablolar:**
  - `contacts`: Kişi bilgileri
  - `messages`: Mesaj geçmişi
  - `header_colors`: Tema renk ayarları
- **Veritabanı İşlemleri:**
  - Tablo oluşturma
  - Varsayılan değer ekleme
  - Versiyon yönetimi

#### SmsReceiver.kt
**Dosya:** `android/app/src/main/kotlin/com/example/hangouts/SmsReceiver.kt`

SMS alma broadcast receiver.
- **Sınıf:** `SmsReceiver : BroadcastReceiver`
- **Özellikler:**
  - Gelen SMS'leri yakalama
  - Bildirim oluşturma
  - Veritabanına mesaj kaydetme
  - Flutter'a gerçek zamanlı bildirim gönderme
  - Uygulama durumu kontrolü

### Konfigürasyon Dosyaları

#### pubspec.yaml
**Dosya:** `pubspec.yaml`

Flutter proje konfigürasyonu.
- **Proje Adı:** hangouts
- **Flutter SDK:** ^3.9.2
- **Bağımlılıklar:** 
  - `flutter`: Flutter framework
  - `flutter_lints`: Kod analizi
- **Ayarlar:** Material Design kullanımı

#### Android Manifest
**Dosyalar:** `android/app/src/main/AndroidManifest.xml`

Android uygulama izinleri ve konfigürasyonu.
- **İzinler:** SMS, telefon, depolama, bildirim
- **Broadcast Receiver:** SMS alma
- **Activity:** Ana uygulama activity

## 🔧 Kurulum ve Çalıştırma

### Gereksinimler
```bash
# Flutter SDK kurulumu
# https://flutter.dev/docs/get-started/install adresinden Flutter'ı indirin

# Android Studio veya VS Code
# Android SDK ve emülatör kurulumu
```

### Proje Kurulumu
```bash
# Proje dizinine git
cd hangouts

# Bağımlılıkları yükle
flutter pub get

# Android cihaz/emülatör bağlantısını kontrol et
flutter devices

# Uygulamayı çalıştır
flutter run
```

### Build İşlemleri
```bash
# Debug APK oluşturma
flutter build apk --debug

# Release APK oluşturma
flutter build apk --release

# Android App Bundle oluşturma (Play Store için)
flutter build appbundle --release
```

### İzinler ve Ayarlar
```bash
# Android cihazda manuel izinler:
# - SMS gönderme/alma izni
# - Telefon arama izni
# - Bildirim izni
# - Depolama erişim izni
```

## 🎯 Öğrenilen Kavramlar

### Flutter Geliştirme
1. **Widget Yapısı**: StatefulWidget ve StatelessWidget kullanımı
2. **State Management**: setState ve state lifecycle yönetimi
3. **Navigation**: Sayfalar arası geçiş ve veri aktarımı
4. **Future ve Async**: Asenkron programlama ve veri yükleme
5. **Form Validation**: Kullanıcı girdi doğrulama
6. **Custom Widgets**: Yeniden kullanılabilir UI bileşenleri

### Native Platform Entegrasyonu
1. **Method Channels**: Flutter-Native kod iletişimi
2. **Event Channels**: Gerçek zamanlı veri aktarımı
3. **Platform Specific Code**: Android Kotlin entegrasyonu
4. **Permissions**: İzin yönetimi ve kontrol
5. **Broadcast Receivers**: Sistem event'lerini yakalama

### Android Geliştirme
1. **SQLite Database**: Yerel veri saklama
2. **SMS Management**: SMS gönderme ve alma
3. **Notifications**: Bildirim sistemi
4. **Intent Handling**: Android intent sistem
5. **Lifecycle Management**: Activity yaşam döngüsü

### Veritabanı ve Veri Yönetimi
1. **SQLite Operations**: CRUD işlemleri
2. **Data Modeling**: Veri modellemesi
3. **Relationships**: Tablolar arası ilişkiler
4. **Migration**: Veritabanı versiyon yönetimi

## 🌟 Uygulama Akışı

### Uygulama Başlatma
1. `main.dart` - Uygulama başlatılır
2. `HomePage` - Ana sayfa yüklenir
3. İzin kontrolleri yapılır
4. Veritabanı ve kişiler yüklenir

### Mesajlaşma Akışı
1. SMS alınır → `SmsReceiver` → Veritabanına kaydedilir
2. Flutter'a event gönderilir → UI güncellenir
3. Bildirim gösterilir
4. Mesaj geçmişi güncellenir

### Kişi Yönetimi Akışı
1. Kişi listesi → Kişi seçimi → Detay sayfası
2. Düzenleme/Silme işlemleri
3. Veritabanı güncellemeleri
4. UI yenileme

## 📋 Teknik Detaylar

### Flutter-Native Köprüsü
- **Method Channels**: Tek yönlü fonksiyon çağrıları
- **Event Channels**: Sürekli veri akışı
- **Platform Messages**: Asenkron mesajlaşma

### Veritabanı Şeması
```sql
contacts: id, name, surname, nickname, phone_number, email, profile_picture
messages: id, status, body, timestamp, contact_id
header_colors: red, green, blue
```

### Performans Optimizasyonları
- Lazy loading ile kişi listesi
- Event dinleyici temizliği
- Memory leak önleme
- Background işlem yönetimi

### Güvenlik Önlemleri
- İzin kontrolleri
- Input validation
- SQL injection koruması
- Platform security best practices

Bu proje, modern mobil uygulama geliştirme tekniklerini, platform entegrasyonunu ve kullanıcı deneyimi tasarımını öğrenmek için kapsamlı bir örnek teşkil eder.
