# STM32 Tabanlı Dijital Saat Projesi

## Proje Açıklaması
Bu proje DS3231 RTC modülü ve 4 haneli 7 segment display kullanarak hassas zaman gösterimi yapan bir dijital saat uygulamasıdır. STM32F1xx serisi mikrodenetleyici üzerinde geliştirilmiştir.

## Temel Özellikler
- Gerçek zamanlı saat (RTC) fonksiyonları
- 7 segment display tarama ve kontrolü
- Butonlarla zaman ayarlama menüsü
- EEPROM üzerinden veri saklama
- Otomatik aydınlık/karanlık mod geçişi

## Dosya Yapısı
1. `main.c`:
   - Timer kesmeleri ile display tarama
   - Buton interrupt handler'ları
   - Zamanlama ve menü kontrol mantığı
   - Otomatik kapanma/başlama fonksiyonları

2. `ds3231.c`:
   - I2C ile DS3231 RTC haberleşmesi
   - BCD-DEC dönüşüm fonksiyonları
   - Zaman ve tarih okuma/yazma işlemleri

3. `ee.c`:
   - Flash bellek üzerinde EEPROM emülasyonu
   - Veri okuma/yazma ve formatlama fonksiyonları
   - Donanıma özel flash konfigürasyonları

## Donanım Bağlantıları
| Bileşen      | GPIO Pinleri       |
|--------------|--------------------|
| 7 Segment    | PA0-PA6, PB0-PB3   |
| DS3231 (I2C) | PB6 (SCL), PB7 (SDA)|
| Butonlar     | PB4, PB5, PB8      |

## Önemli Fonksiyonlar
- `HAL_TIM_PeriodElapsedCallback()`: Display tarama ve LED kontrol
- `SAATAYAR()`: Butonlarla interaktif zaman ayarlama
- `DS3231_sendData()`: RTC modülüne veri gönderme
- `ee_write()`: Kalıcı veri saklama işlemleri

## Derleme ve Kurulum
1. STM32CubeIDE ile projeyi içe aktarın
2. Donanım bağlantılarını kontrol edin
3. `EE_USE_FLASH_PAGE_OR_SECTOR` değerini flash boyutunuza göre ayarlayın
4. Programı mikrodenetleyiciye yükleyin