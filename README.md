# 42-Kocaeli-Avaj_Launcher

## Aircraft Weather Simulation (Uçak Hava Durumu Simülasyonu)

Bu proje, 42 Okulu müfredatının ileri seviye projelerinden biridir. Java dilinde hava durumu simülasyonu ile uçakların hareketlerini simüle etmeyi amaçlar. Observer tasarım deseni, Factory deseni ve Singleton deseni kullanılarak nesne yönelimli programlama prensipleri uygulanır.

## 🏗️ Mimari

### Tasarım Desenleri

Proje, aşağıdaki tasarım desenlerini kullanır:

- **Observer Pattern**: Hava durumu değişikliklerini uçaklara bildirmek için kullanılır
- **Factory Pattern**: Farklı uçak türlerini oluşturmak için kullanılır
- **Singleton Pattern**: WeatherProvider ve AircraftFactory sınıflarında kullanılır

### Veri Yapıları

#### Coordinates

```java
public class Coordinates {
    private int longitude;  // 1-360 arası
    private int latitude;   // 1-90 arası
    private int height;     // 0-100 arası
}
```

#### Flyable Interface

```java
public abstract class Flyable {
    protected WeatherTower weatherTower;

    public abstract void updateConditions();
    public void registerTower(WeatherTower p_tower);
}
```

#### Aircraft Abstract Class

```java
public abstract class Aircraft extends Flyable {
    protected long id;
    protected String name;
    protected Coordinates coordinates;

    public abstract void log();
    public String getIdentifier();
    protected void updateCoordinates(int longitude, int latitude, int height);
    public boolean hasLanded();
}
```

## 📚 Sınıflar

### Simulator - Ana Simülasyon Sınıfı

**Dosya:** `com/enesates/avaj/simulator/Simulator.java`

Simülasyonun giriş noktası.

- **Fonksiyon:** `public static void main(String[] args)`
- **Açıklama:** Senaryo dosyasını okur, uçakları oluşturur, simülasyonu çalıştırır
- **Parametre:** Senaryo dosyası yolu
- **Çıktı:** simulation.txt dosyası

### AircraftFactory - Uçak Fabrikası

**Dosya:** `com/enesates/avaj/simulator/AircraftFactory.java`

Farklı uçak türlerini oluşturur.

- **Desen:** Singleton
- **Desteklenen Türler:** Balloon, Helicopter, JetPlane
- **Fonksiyon:** `public Flyable newAircraft(String type, String name, Coordinates coordinates)`

### WeatherProvider - Hava Durumu Sağlayıcısı

**Dosya:** `com/enesates/avaj/simulator/WeatherProvider.java`

Koordinatlara göre hava durumunu hesaplar.

- **Desen:** Singleton
- **Hava Durumları:** SUN, RAIN, FOG, SNOW
- **Hesaplama:** `(longitude² + e^latitude + √height) % 4`

### WeatherTower - Hava Kulesi

**Dosya:** `com/enesates/avaj/simulator/WeatherTower.java`

Hava durumu değişikliklerini yönetir.

- **Fonksiyon:** `public void changeWeather()`
- **Açıklama:** Tüm kayıtlı uçaklara hava durumu değişikliğini bildirir

### Tower - Gözlemci Kulesi

**Dosya:** `com/enesates/avaj/simulator/Tower.java`

Observer pattern'in subject kısmını implement eder.

- **Fonksiyonlar:**
  - `register(Flyable flyable)`: Uçak kaydı
  - `unregister(Flyable flyable)`: Uçak kaydını silme
  - `conditionChanged()`: Koşul değişikliği bildirimi

### Aircraft Türleri

#### Balloon - Balon

**Dosya:** `com/enesates/avaj/simulator/Balloon.java`

Balon hareket simülasyonu.

- **SUN:** longitude +2, height +4 (max 100)
- **RAIN:** height -5 (min 0)
- **FOG:** height -3 (min 0)
- **SNOW:** height -15 (min 0)

#### Helicopter - Helikopter

**Dosya:** `com/enesates/avaj/simulator/Helicopter.java`

Helikopter hareket simülasyonu.

- **SUN:** longitude +10, height +2 (max 100)
- **RAIN:** longitude +5
- **FOG:** longitude +1
- **SNOW:** height -12 (min 0)

#### JetPlane - Jet Uçağı

**Dosya:** `com/enesates/avaj/simulator/JetPlane.java`

Jet uçağı hareket simülasyonu.

- **SUN:** latitude +10 (max 90), height +2 (max 100)
- **RAIN:** latitude +5 (max 90)
- **FOG:** latitude +1 (max 90)
- **SNOW:** height -7 (min 0)

### SimulationWriter - Simülasyon Yazıcısı

**Dosya:** `com/enesates/avaj/simulator/SimulationWriter.java`

Simülasyon çıktılarını dosyaya yazar.

- **Desen:** Singleton
- **Çıktı Dosyası:** simulation.txt
- **Fonksiyon:** `println(String message)`

## 🎁 Özellikler

### Senaryo Dosyası Formatı

```
<simülasyon sayısı>
<TYPE> <NAME> <LONGITUDE> <LATITUDE> <HEIGHT>
<TYPE> <NAME> <LONGITUDE> <LATITUDE> <HEIGHT>
...
```

**Örnek:**

```
25
Balloon B1 2 3 20
Helicopter H1 654 33 20
JetPlane J1 23 44 32
```

### Koordinat Validasyonları

- **Longitude:** 1-360 (0 ise 360 yapılır)
- **Latitude:** 1-90
- **Height:** 0-100 (0 ise iniş yapmış sayılır)

### Hava Durumu Hesaplama

Hava durumu, matematiksel formül ile hesaplanır:

```java
int index = (longitude² + e^latitude + √height) % 4;
String[] weather = {"RAIN", "FOG", "SUN", "SNOW"};
```

### Otomatik İniş

Uçakların yüksekliği 0'a ulaştığında otomatik olarak iniş yapmış sayılır ve simülasyondan çıkarılır.

## 🔧 Kullanım

### Derleme ve Çalıştırma

```bash
# Tüm Java dosyalarını derle
make

# Simülasyonu çalıştır
make run

# Temizlik
make clean
```

### Manuel Çalıştırma

```bash
# Derleme
find * -name "*.java" > sources.txt
javac @sources.txt

# Çalıştırma
java com.enesates.avaj.simulator.Simulator scenario.txt
```

### Çıktı

Simülasyon sonuçları `simulation.txt` dosyasına yazılır:

```
Tower says: Balloon#B1(1) registered to weather tower.
Tower says: Helicopter#H1(2) registered to weather tower.
Balloon#B1(1): Let's enjoy the good weather and take some pics.
Helicopter#H1(2): This is hot.
...
Balloon#B1(1) landing.
Tower says: Balloon#B1(1) unregistered from weather tower.
```

## 🎯 Teknik Detaylar

### Koordinat Güncellemeleri

- **Longitude:** 360 derece sisteminde döngüsel
- **Latitude:** 90 derece kuzey sınırı
- **Height:** 0-100 arası, 0'da iniş

### Exception Handling

- **InvalidScenarioException:** Geçersiz senaryo formatı
- **InvalidAircraftTypeException:** Bilinmeyen uçak türü
- **InvalidCoordinatesException:** Geçersiz koordinat değerleri
- **SimulationWriterException:** Dosya yazma hatası

### Observer Pattern Akışı

1. Uçaklar WeatherTower'a kaydolur
2. `changeWeather()` çağrılır
3. `conditionChanged()` tüm uçakların `updateConditions()`'ını çağırır
4. Hava durumu değiştiyse `logObservers()` ile mesajlar loglanır
5. İnen uçaklar unregister edilir

## 🧪 Test Stratejisi

### Temel Testler

1. **Senaryo Parsing:** Geçerli ve geçersiz senaryo dosyaları
2. **Uçak Oluşturma:** Tüm uçak türleri ve edge case'ler
3. **Hareket Simülasyonu:** Farklı hava durumlarında koordinat güncellemeleri
4. **İniş Algılama:** Yükseklik 0 olduğunda unregister

### Edge Case'ler

1. **Sınır Koordinatlar:** Min/max değerler
2. **Geçersiz Senaryo:** Yanlış format, negatif değerler
3. **Uzun Simülasyon:** Çok sayıda iterasyon
4. **Çoklu Uçak:** Aynı anda birçok uçak

## 📋 Öğrenilen Kavramlar

1. **Object-Oriented Programming:** Inheritance, polymorphism, abstraction
2. **Design Patterns:** Observer, Factory, Singleton
3. **File I/O:** Java NIO ile dosya okuma/yazma
4. **Exception Handling:** Custom exception sınıfları
5. **Mathematical Modeling:** Hava durumu hesaplama algoritması
6. **Simulation Design:** Gerçek dünya sistemlerinin modellenmesi
7. **Java Package Structure:** Paketleme ve modülerlik

## ⚠️ Bilinen Sınırlamalar

- **Koordinat Hassasiyeti:** Integer aritmetiği sınırlaması
- **Hava Durumu Deterministik:** Aynı koordinatlar aynı hava durumunu verir
- **Simülasyon Adımları:** Sadece tam sayı iterasyonları
- **Çıktı Formatı:** Basit text tabanlı loglama

## 📋 Notlar

- Tüm sınıflar Java 8+ uyumludur
- Observer pattern gerçek zamanlı hava durumu güncellemeleri için uygundur
- Factory pattern uçak türlerinin kolay genişletilmesine olanak verir
- Singleton pattern global state yönetimini basitleştirir
- Exception handling robust error management sağlar
- File I/O operations buffered writing ile performans optimize edilmiştir
