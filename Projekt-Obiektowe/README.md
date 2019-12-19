#  Programowanie obiektowe - Projekt

## Klasa `Tool`

##### Metody:
- `void setEntry(string field, string value)` - ustawia wartość pola
- `string getEntry(string field)` - zwraca wartość z pola
- `void init()` - inicjalizuje działanie klasy
- `void redraw()` - przerysowuje okno z narzędziem

##### Pola:
- `Backend* backend` - wskaźnik na obiekt typu Backend


## Klasa `Backend`

##### Metody:
- `void bind(string str, function<void()> func, string help) ` - wiąże komendę z funkcją
    - `string str` - komenda
    - `function<void()> func` - funkcja do wywołania, gdy komenda zostanie uruchomiona
    - `string help` - opis działania funkcji
        - Przykład działania:
        ```c++
        backend->bind("<ENTER>", []() { cout << "Wcisnieto Enter"; }, "Pomoc"); // Wyrażenie lambda
        backend->bind("<ENTER>", bind(&ExampleTool::saveFile, this), "Pomoc");  // Wiązanie
        ```
- `void start()` - rozpoczyna działanie backendu

##### Pola:
- `Tool* tool` - wskaźnik na obiekt typu Tool


## Przykład
```c++
ExampleBackend b;
ExampleTool t;
b.tool = &t;
t.backend = &b;
t.init();
b.start();
```
