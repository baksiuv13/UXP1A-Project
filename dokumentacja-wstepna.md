# Projekt wstępny

## 1. Treść zadania oraz wariant

Napisać wieloprocesowy system realizujący komunikację w języku komunikacyjnym Linda.

Przestrzeń krotek będzie realizowana przy pomocy pamięci dzielonej i semaforów(SYSV IPC).

## 2. Interpretacja treści zadania i wymagania funkcjonalne

Celem projektu jest zrealizowanie komunikacji między-procesowej w języku Linda. Końcowym efektem będzie możliwość uruchomienia kilku procesów (wykorzystując np: kilka terminali) i realizacja wyżej wymienionego celu. Przykładowo, na jednym terminalu wpisujemy krotkę A(operacja output). Następnie na drugim terminalu zadajemy zadanie read o kryteriach które spełnia krotka A. Drugi terminal zwróci wartość krotki A.

Uruchamiany program będzie interfejsem do biblioteki, która będzie udostępniać operacje realizowane przez język Linda (output, input, read).

Wymagania funkcjonalne:

podłączenie się procesu do pamięci współdzielonej
odłączenie się procedury od pamięci współdzielonej
realizacja funkcji input - umieszczenie krotki w przestrzeni krotek
realizacja funkcji output - atomowe pobranie i usunięcie krotki z przestrzeni (wg wzorca)
realizacja funkcji read - pobieranie lecz nie usuwa krotki z przestrzeni krotek

## 3. Krótki opis funkcjonalny - “black-box” - proponowane API

Dostarczamy bibliotekę udostępniającą klasę, która w konstruktorze podłącza się do przestrzeni krotek (należy jej przekazać identyfikator pamięci współdzielonej, w przypadku braku identyfikatora - tworzenie pamięci i generowanie identyfikatora) oraz udostępnia metody operacji na krotkach.

Krotkę reprezentuje tablica o ustalonym stałym rozmiarze. Na początku ma zapisane, ile ma elementów.

Elementy krotki to struktury składające się z informacji o typie (enum) i wartości (union).

Elementu opisu oczekiwanej krotki to struktury składające się z informacji o typie (enum) oraz rodzaju warunku (enum) i wartości (union).

Elementy krotki mogą być różnej długości, te z liczbami mają stałą długość, natomiast krotki z napisami mają dowolną długość mieszcząca się w krotce razem z innymi elementami. Koniec napisu jest oznaczony znakiem ‘\0’, a następny element po nim jest wyrównany do wielokrotności 4 bajtów.

Destruktor obiektu realizuje odłączenie się od pamięci współdzielonej.

## 4. i 5. Planowany podział na moduły/procesy i strukturę komunikacji między nimi, w tym koncepcja realizacji współbieżności oraz opis zaproponowanych protokołów komunikacji między-procesowej

Pierwszy proces rezerwuje pamięć współdzieloną i tworzy semafory. Następne podłączają się do zarezerwowanej pamięci, komunikacja między procesorami odbywa się wyłącznie poprzez odpowiednie wykorzystanie pamięci. Ostatni program odłączający się od pamięci ją zamyka.

### Koncepcja realizacji współbieżności

Zadanie zostanie rozwiązane jako problem czytelników i pisarzy w wariancie nie powodującym zagłodzenia żadnego z typu procesów. Czytelnikami w naszym przypadku będą procesy wykonujące operację read. Pisarzami będą procesy wykonujące operacje input oraz output. [Rozwiązanie wzorcowe.](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem#Third_readers-writers_problem)

## 6. Zarys koncepcji implementacji (najważniejsze funkcje, algorytmy, obiekty komunikacyjne, itp.)

* segment pamięci współdzielonej alokowany przez pierwszy włączony program i zwalniany przez ostatni wyłączony,
* semafor nadzorujący, ile programów używa pamięci, program, który ją tworzy ustawia na jeden, podłączające zwiększają ją o jeden, a każdy, który odłącza się od pamięci, próbuje opuścić semafor bez blokowania i jeśli nie może, to znaczy, że był ostatni i zamyka tę pamięć
* blokada będąca muteksem czytelników i pisarzy

```C++
struct element {
    enum Typ : int16_t {
INT, FLOAT, STRING
}typ;
int16_t empty;
union {
    int32_t a;
    float b;
    char c; // to tylko dla adresu pierwszego znaku
};
};

struct element_opis {
    enum Typ : int16_t {
INT, FLOAT, STRING
}typ;
enum Warunek : int16_t {
    <, >, <=, >=, ==
};
union {
    int32_t a;
    float b;
    char c; // to tylko dla adresu pierwszego znaku
};
};

krotka{
    element table[rozmiar_calosci]
}
```

## 7. Sytuacje krytyczne i nietypowe

Zamykanie pamięci: kiedy ostatni proces zdecyduje się zamknąć pamięć, to przed zwolnieniem pamięci jakiś proces może próbować się dostać.
Zagłodzenia procesów - istnieje możliwość zagłodzenia procesu, który próbuje dokonać funkcji output. Sytuacja taka nastąpi w momencie w którym proces próbuje znaleźć dla siebie krotkę, nie znajdzie jej, zostanie wstawiony na koniec kolejki oczekujących pisarzy, po czym inny proces doda krotkę która pasuje do jego wzorca a jeszcze następny proces ją pobierze. W wymienionej tu sytuacji krotkę pasującą do wzorca otrzyma ten proces, który być może później zgłosił swoje żądanie znalezienia krotki, a proces zgłaszający się wcześniej może zostać zagłodzony. Sytuacji tej możnaby uniknąć w ten sposób, że po dodaniu krotki, wszystkie procesy oczekujące z funkcją output musiałyby po kolei wg czasu swojego pierwszego zgłoszenia przeglądać nowo dodane krotki. To mogłoby bardzo znacząco obciążyć cały nasz system i spowodować niepotrzebny narzut czasowy, aby uniknąć skrajnej sytuacji zagłodzenia.
