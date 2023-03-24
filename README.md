# remote-procedure-calls
Aplikacja serwer-klient łącząca się po protokole TCP. Klient po połączeniu może zdalnie wywoływać pobrane z serwera procedury.

Aplikacja kliencka została stworzona z języku C++ z frameworkiem Qt dla systemu Windows. Plik wykonywalny umieszczony jest w katalogu 'client', a jego pliki źródłowe znajdują się w katalogu 'source'. Do kompilacji wymagane jest środowisko Qt.

Aplikacja serwera znajduje się w katalogu 'server'. Została stworzona dla systemu Linux. Sposób kompilacji i uruchomienia:

Kompilacja serwera:
    g++ *.cpp -o 'server name'
    g++ -Wall -std=c++17 -pthread *.cpp -o 'server name'
Uruchomienie serwera:
    ./'server name'
