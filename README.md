# Temat projektu: Ostatnie miejsce na magistrali CAN
Ten projekt jest symulacją modułów rakiety, które generują wiadomości dla komputera pokładowego oraz symulacją magistrali CAN 2.0A, która transportuje te ramki. 

---

## Struktura projektu

* `Config.h` – stałe konfiguracyjne standardu CAN 2.0A.
* `CANFrame.h` / `CANFrame.cpp` – reprezentacja pojedynczej ramki CAN.
* `Module.h` / `Module.cpp` – model modułu pokładowego, generującego zdefiniowane typy wiadomości w zadanych interwałach.
* `CANBus.h` / `CANBus.cpp` – symulacja magistrali z kolejką priorytetową opartą o identyfikator ID, zliczaniem statystyk opóźnień, spóźnień oraz zagłodzonych ramek.
* `main.cpp` – konfiguracja modułów oraz uruchomienie pętli symulacji.

---
## Jak uruchomić

### Wymagania
* Kompilator wspierający standard **C++20**
* (opcjonalnie) **CMake** w wersji 3.20 lub nowszej 

#### Opcja 1 - bezpośrednio przez terminal:
```bash
  g++ -std=c++20 -O2 main.cpp CANBus.cpp CANFrame.cpp Module.cpp -o can_bus_sim
  ./can_bus_sim
```
---
#### Opcja 2 - budowanie za pomocą CMake: 
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
następnie uruchom wygenerowany plik wykonalny:
* Windows: .\can_bus_sim.exe
* Linux/MacOS: ./can_bus_sim

---
## Założenia
* **Krok czasowy:** do symulacji wykorzystano krok czasowy równy **1 µs**, ponieważ dla podanej w zadaniu prędkości magistrali czas obsługi najmniejszej jednostki informacji, czyli 1 bitu wynosi 2µs.
* **Czas transmisji bitu:** wyliczany dynamicznie na podstawie bitrate jako $\frac{10^6}{\text{bitrate}}$ [µs].
* **Struktura i rozmiar ramki:** maksymalna ilość danych to 8B = 64 bity, do tego 44 bitów narzutu + 3 bity IFS(przerwa miedzy ramkami), + bit stuffing, maksymalnie ramka ma 135 bitów(wliczając IFS).
* **Arbitraż priorytetów:** pierwszeństwo wejścia na magistralę mają ramki z niższym ID.

---
## Konfiguracja wiadomości i optymalizacja ID
W systemie zaimplementowano 4 moduły pokładowe (`Silnik`, `Nawigacja`, `Zasilanie`, `Diagnostyka`) generujące łącznie 8 typów wiadomości:

| Moduł | Wiadomość | Rozmiar [B] | Okres [ms] | Deadline [ms] | ID domyślne | ID zoptymalizowane (DMS) |
| :--- | :--- | :---: | :---: | :---: |:-----------:| :---: |
| Silnik | Ciśnienie | 4 | 20 | 10 |   `0x020`   | `0x010` |
| Nawigacja | Przyśpieszenie | 4 | 30 | 20 |   `0x030`   | `0x020` |
| Silnik | Temperatura | 4 | 10 | 50 |   `0x010`   | `0x030` |
| Nawigacja | Nachylenie | 6 | 100 | 50 |   `0x150`   | `0x100` |
| Nawigacja | Pozycja GPS | 8 | 200 | 100 |   `0x200`   | `0x110` |
| Silnik | Poziom paliwa | 4 | 100 | brak |   `0x050`   | `0x120` |
| Zasilanie | Napięcie akumulatora | 2 | 100 | brak |   `0x100`   | `0x200` |
| Diagnostyka | Status systemu | 4 | 500 | brak |   `0x300`   | `0x300` |

### Uzasadnienie zmiany ID

Aby zoptymalizować system i zminimalizować liczbę wiadomości docierających po terminie, im mniejszy deadline ma wiadomość tym szybciej powinna docierać na bus, czyli tym niższy ID powinna mieć.

---
## Wyniki eksperymentów i analiza

### 1. Wpływ optymalizacji ID na opóźnienia i terminowość

Porównano dwa przydziały identyfikatorów:

- **domyślny**: ID z treści zadania (np. Temperatura `0x010`, Ciśnienie `0x020`),
- **zoptymalizowany (DMS)**: *Deadline Monotonic Scheduling*, czyli niższe ID dostają wiadomości z krótszym terminem. Wiadomości bez terminu dostają najwyższe ID.

Każdą konfigurację uruchomiono dla czasu symulacji **50 s** przy dwóch prędkościach magistrali: **500 kbit/s** (wartość z polecenia) oraz **25 kbit/s** (wysokie obciążenie magistrali). W każdym przebiegu wygenerowano **11 017 wiadomości**, a wszystkie opóźnienia podano w µs. Wiadomości Przyspieszenie, Nachylenie i Poziom paliwa to dodatkowe wiadomości z części „dla ambitnych".

| Wiadomość (deadline) | ID domyślne → DMS | 500 kbit/s: domyślne (śr. / max) [µs] | 500 kbit/s: DMS (śr. / max) [µs] | 25 kbit/s: domyślne (śr. / max) [µs] | 25 kbit/s: DMS (śr. / max) [µs] | Zmiana max opóźnienia (500k / 25k) |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Ciśnienie** (10 ms) | `0x020` → `0x010` | 380 / 380 | **190 / 190** | 8 471 / 11 000 | **4 698 / 9 000** | **-50% / -18%** |
| **Przyspieszenie** (20 ms) | `0x030` → `0x020` | 475 / 570 | **285 / 380** | 11 212 / 18 600 | **6 444 / 10 400** | **-33% / -44%** |
| **Temperatura** (50 ms) | `0x010` → `0x030` | 190 / 190 | 348 / 570 | 4 911 / 8 600 | 8 078 / 14 200 | +200% / +65% |
| **Nachylenie** (50 ms) | `0x150` → `0x100` | 1 013 / 1 140 | **673 / 800** | 30 392 / 34 200 | **14 738 / 19 800** | **-30% / -42%** |
| **Pozycja GPS** (100 ms) | `0x200` → `0x110` | 1 283 / 1 410 | **943 / 1 070** | 47 230 / 54 800 | **23 938 / 25 200** | **-24% / -54%** |
| **Poziom paliwa** (brak) | `0x050` → `0x120` | 633 / 760 | 998 / 1 260 | 13 938 / 19 000 | 33 276 / 51 800 | +66% / +173% |
| **Napięcie akumulatora** (brak) | `0x100` → `0x200` | 783 / 910 | 1 148 / 1 410 | 20 730 / 22 000 | 45 115 / 58 600 | +55% / +166% |
| **Status systemu** (brak) | `0x300` → `0x300` | 1 339 / 1 600 | 1 339 / 1 600 | 54 038 / 62 400 | 54 038 / 62 400 | 0% / 0% |
| **Wiadomości po terminie** | — | **0** | **0** | **333** | **0** | **-100%** |

**Wnioski:** Zastosowanie DMS pomogło z terminowością, liczba spoźnionych ramek spadła z **333 do 0**. 

### 2. Analiza zagłodzenia niskich priorytetów

Konfigurację ze zoptymalizowanymi ID (DMS) uruchomiono na czas **50 s** (łącznie **11 017 wygenerowanych wiadomości**) dla trzech przepływności magistrali: **500 kbit/s**, **25 kbit/s** oraz **10 kbit/s**.

| Wiadomość (ID DMS) | 500 kbit/s (wysłane / wygen.) | 25 kbit/s (wysłane / wygen.) | 10 kbit/s (wysłane / wygen.) | Zagłodzone przy 10 kbit/s | Max opóźnienie przy 10 kbit/s [µs] |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Ciśnienie** (`0x010`) | 2 500 / 2 500 | 2 500 / 2 500 | **2 500 / 2 500** | 0 (0%) | 18 500 |
| **Przyspieszenie** (`0x020`) | 1 667 / 1 667 | 1 667 / 1 667 | **1 667 / 1 667** | 0 (0%) | 28 000 |
| **Temperatura** (`0x030`) | 5 000 / 5 000 | 5 000 / 5 000 | **1 097 / 5 000** | **3 903 (78,1%)** | 48 239 000 |
| **Nachylenie** (`0x100`) | 500 / 500 | 500 / 500 | **0 / 500** | **500 (100%)** | — *(brak wysyłki)* |
| **Pozycja GPS** (`0x110`) | 250 / 250 | 250 / 250 | **0 / 250** | **250 (100%)** | — *(brak wysyłki)* |
| **Poziom paliwa** (`0x120`) | 500 / 500 | 500 / 500 | **0 / 500** | **500 (100%)** | — *(brak wysyłki)* |
| **Napięcie akumulatora** (`0x200`) | 500 / 500 | 500 / 500 | **0 / 500** | **500 (100%)** | — *(brak wysyłki)* |
| **Status systemu** (`0x300`) | 100 / 100 | 100 / 100 | **0 / 100** | **100 (100%)** | — *(brak wysyłki)* |
| **Suma / Po terminie** | **11 017 / 11 017** (0 po terminie) | **11 017 / 11 017** (0 po terminie) | **5 264 / 11 017** (4 030 po terminie) | **5 753 (52,2%)** | — |

**Wnioski:** Dla magistrali o niskiej przepustowości(w tym przypadku 10kbit/s) pasmo jest całkowicie blokowane przez wiadomości o wyższym priorytecie przez co wiadomości typu *Temperatura* są zagłodzone w 78,1%, a wiadomości o wyższym id są całkowice zagładzane.
