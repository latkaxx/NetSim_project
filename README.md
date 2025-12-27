# NetSim\_project



Projekt realizowany w ramach zajęć z \*\*Zaawansowanego Programowania w C++\*\*.  

Celem projektu jest implementacja symulatora przepływu półproduktów w sieci produkcyjnej.



---



\## Wymagania



\- System: \*\*Windows\*\*

\- Kompilator: \*\*MinGW (g++)\*\*

\- Narzędzia:

&nbsp; - \*\*CMake\*\* (>= 3.14)

\- Środowisko testowe: \*\*GoogleTest\*\*



---



\## Instalacja i uruchomienie



\### 1. Pobranie GoogleTest



Można pobrać GoogleTest oraz umieścić je w katalogu /lib/googletest



https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip



Jak się tego nie zrobi to w trakcie budowania program powinien automatycznie to pobrać i dać to do odpowiedneigo folderu. Ale niestety czasami nie działa



\### 2. Konfiguracja projektu (CMake)

W katalogu projektu należy otworzyć terminal oraz zbudować projekt

```bash

cmake -S . -B build -G "MinGW Makefiles"

cmake --build build

```

\###Testy

Po poprawnym zbudowaniu projektu można uruchomić testy:

```bash

.\\build\\tests.exe

```



