## 6. Szczegółowy opis interfejsu użytkownika

Testowy interfejs użytkownika dostarczany jest przez aplikację wykorzystującą
przygotowaną bibliotekę `Linda` w formie prostej konsoli, w której użytkownik
może wykonywać poszczególne operacje wpisując odpowiednie komendy.

### Kompilacja i uruchomienie

Aby skompilować program, należy w katalogu głównym repozytorium wykonać:

    cmake CMakeList.txt
    make

W ten sposób skompilowany zostanie program wynikowy `client.out` zawierający
klienta oraz pliki testów automatycznych, które można wywołać poleceniem `make test`.

Aby uruchomić klienta, należy podać mu jako parametr ścieżkę do pliku, który
zostanie użyty do wygenerowania klucza dla systemowych wywołań. Plik ten musi
istnieć i być dostępny. Przykładowo:

    ./client.out shm-test-file

### Obsługa klienta

