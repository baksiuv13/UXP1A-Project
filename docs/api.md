## 3. Pełen opis funkcjonalny - “black-box” - API

Interfejs dostępy jest poprzez klasę *Linda* z przestrzeni nazw `uxp` zadeklarowaną
w pliku `src/linda.h`. Obiekty tej klasy udostępniają  podstawowe operacje zgodnie
z językiem Linda oraz dodatkowe metody i konstruktory pozwalające na stworzenie
i podłączenie się do współdzielonej pamięci. 

### Realizacja operacji Linda

| Operacja w języku Linda | Metoda z klasy Linda                                       |
| ----------------------- | ---------------------------------------------------------- |
| output(krotka)          | `bool Output(Tuple tuple)`                                 |
| input(wzorzec, timeout) | `Tuple Input(TupleDesc describe, unsigned int timeout_ms)` |
| read(wzorzec, timeout)  | `Tuple Read(TupleDesc describe, unsigned int timeout_ms)`  |
_Tab. 1. Zestawienie operacji w języku Linda i deklaracji realizujących je metod_

1. Operacja **output**: metoda przyjmuje jako parametr strukturę reprezentującą
   krotkę, która zostanie skopiowana do systemu. Metoda zwraca `false`, jeśli
   dodanie krotki nie powiodło się z powodu zapełnienia pamięci współdzielonej
   (liczba krotek osiągnęła maksymalną).
2. Operacje **input** oraz **read**: metody przyjmują jako parametry strukturę
   reprezentującą wzorzec krotki oraz limit czasu (w milisekundach), przez jaki
   ma poszukiwać krotki. Zwracana jest struktura reprezentująca znalezioną krotkę
   lub krotka pusta (o rozmiarze 0), jeśli w zadanym czasie nie udało się 
   odnaleźć krotki pasującej do wzorca. Dodatkowo, dla operacji:
     * **input** - znaleziona krotka zostanie usunięta z systemu,
     * **read** - znaleziona krotka pozostanie w systemie.

### Reprezentacja krotek

Krotki reprezentowane są przez strukturę `Tuple` złożoną z informacji o rozmiarze
oraz tablicy elementów reprezentowanych przez strukturę `Element`. Wzorzec krotki
to odpowidnio struktury `TupleDesc` oraz `ElementDesc`, zawierające dodatkowe
informacje o sposobie dopasowania.

Struktury `Tuple` oraz `Element` udostępniają metdę `ToString()` przedstawiającą
ich zawartość jako `std::string`, w formacie listy wartości wewnątrz okrągłych
nawiasów, np. `(1, 2.0, "c")`.

| Pole struktury              | Opis dla `Tuple`  | Opis dla `TupleDesc`               |
| --------------------------- | ----------------- | ---------------------------------- |
| `size_t size`               | Liczba elementów  | Oczekiwana długość, liczba wzorców |
| `Element elements[...]`     | Tablica elementów | _brak_                             |
| `ElementDesc elements[...]` | _brak_            | Tablica wzorców elementów          |
_Tab. 2. Zestawienie pól dla `Tuple` oraz `TupleDesc`_

| Pole struktury                     | Opis dla `Element`  | Opis dla `ElementDesc` | Możliwe wartości                                                                                                                  |
| ---------------------------------- | ------------------- | ---------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| `ElemType type`                    | Typ elementu (enum) | Typ elementu (enum)    | `INT`, `FLOAT`, `STRING`                                                                                                          |
| `union {...} value`                | Wartość elementu    | Wzorzec do porównania  | `int32_t int_` <br> `float float_` <br> `char string_[...]` <br> - zgodnie z typem elementu                                       |
| `ElementDesc::Condition condition` | _brak_              | Sposób dopasowania     | `ANY` (dowolna wartość),<br> `LESS` (`<`),<br> `GREATER` (`>`),<br> `LESS_EQ` (`<=`),<br> `GREATER_EQ` (`>=`),<br> `EQUAL` (`==`) |
_Tab. 3. Zestawienie pól dla `Element` oraz `ElementDesc`_

### Inicjalizacja
Konstruktor klasy przyjmuje ścieżkę do pliku, która będzie podstawą wygenerowania
klucza dla wywołań systemowych. Wszystkie instancje mające być połączone w jeden
komunikujący się system, muszą odwoływać się do jednego pliku. Plik musi istnieć
i być dostępny.

Specyfiokacja konstruktora: `Linda(const char *path)`

Taki sam efekt można uzyskać wywołując najpierw pusty konstruktor, a następnie
metodą `int Attach(const char *path)`.  
^^^^^^ chciałbym zostawić tylko konstruktor, jeśli Piotrkowi uda się to zunifikować

### Ograniczenia implementacyjne
Następujące parametry systemu zostały ustalone podczas implementacji. Programy
korzystające z biblioteki mogą odczytywać ich wartości z podanych lokalizacji.

| Ograniczenie                               | Wartość | Miejsce zdefiniowania (tylko do odczytu)  |
| ------------------------------------------ | ------- | ----------------------------------------- |
| Maksymalna liczba krotek na raz w systemie | 100     | Stała `TUPLES_NUMBER` z klasy `Linda`     |
| Maksymalna liczba elementów w krotce       | 100     | Stała `MAX_TUPLE_SIZE` w `src/tuple.h`    |
| Maksymalna długość stringa w elemencie     | 100     | Stała `MAX_STRING_SIZE` w `src/element.h` |

### Możliwe wyjątki, błędy, obsługa ograniczeń

TODO:
* Błąd inicjalizacji/podłączenia pamięci lub semaforów
* Pamięć zapełniona
* Zbyt długi string

### Przykład

```C++
#include "src/linda.h"

int main(){
     uxp::Linda linda("path/to/file");
     
     // (3.0, 1, "aaa")
     uxp::Tuple tuple{3,
          {uxp::Element(3.0f), uxp::Element(1), uxp::Element("aaa")}};
     linda.Output(tuple);

     // pattern: (float:>=1.0, int:*, string:aaa)
     uxp::TupleDesc pattern{3,
          {uxp::ElementDesc(1.0f, uxp::ElementDesc::Condition::GREATER_EQ),
          uxp::ElementDesc(0, uxp::ElementDesc::Condition::ANY),
          uxp::ElementDesc("aaa", uxp::ElementDesc::EQUAL)}};

     auto read = linda.Read(pattern, 1000);
     std::cout << read.ToString() << "\n";

     auto input = linda.Input(pattern, 1000);
     std::cout << input.ToString() << "\n";

     auto notfound = linda.Input(pattern, 1000);
     std::cout << notfound.ToString() << "\n";

     return 0;
}
```

Oczekiwany rezultat:

     (3.000000, 1, aaa)
     (3.000000, 1, aaa)
     ()

