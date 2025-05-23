#### [<< главная](../Main.md)
## Операции сравнения
Кроме уже рассмотренных [арифметических операций](./5\)%20Arithmetic%20operations.md), в библиотеке существуют функции для сравнения чисел.

Далее представлено описание каждой из функции:

### Проверка на ноль
```pawn
stock bool:uint64iszero(const oper[uint64])
```
Функция проверяет является ли число нулём или нет. В случае, если оно имеет значение равное нулю, она возвращает <code>true</code>, иначе — <code>false</code>.

#### Пример использования:
```pawn
main()
{
    new number[uint64];
    uint64zero(number);
    // ^^^ создаем переменную и зануляем с помощью uint64zero

    if(uint64iszero(number))
    {
        print("число number = 0");
        // ^^^ всегда будет срабатывать, поскольку в
        // переменную до этого записывается нуль
    }
    else
    {
        print("число number - не нуль");
        // ^^^ никогда не сработает, поскольку в
        // переменную до этого записывается нуль
    }
}
```

### Сравнение двух 64-битных чисел
```pawn
stock uint64cmp(oper1[uint64], oper2[uint64])
```
Сравнивает два числа uint64, и возвращает:
* <code>UINT64_EQUAL</code>, если числа равны.<br><br>
* <code>UINT64_LESS</code>, если число <code>oper1</code> меньше, чем <code>oper2</code>.<br><br>
* <code>UINT64_GREATER</code>, если число <code>oper1</code> больше, чем <code>oper2</code>.

#### Пример использования
```pawn
main()
{
    new number[uint64];
    uint64fromstr(number, "500 000 000 000");

    new number2[uint64];
    uint64fromstr(number, "440 000 000 000");

    // ^^^ создаём переменные и присваиваем значения.

    if(uint64cmp(number, number2) == UINT64_GREATER)
    {
        print("число number - больше чем number2");
        // ^^^ всегда будет срабатывать, поскольку в
        // переменную number записывается значение
        // больше, чем в number2.
    }
    else
    {
        print("число number не больше чем number2");
        // ^^^ никогда не сработает по вышеизложенным
        // причинам.
    }
}
```

### Сравнение 64-битного числа с 32-битным
```pawn
stock uint64cmp32(const oper1[uint64], oper2)
```
Сравнивает число <code>oper1</code> с числом <code>oper2</code> и возвращает:
* <code>UINT64_EQUAL</code>, если числа равны.<br><br>
* <code>UINT64_LESS</code>, если число <code>oper1</code> меньше, чем <code>oper2</code>.<br><br>
* <code>UINT64_GREATER</code>, если число <code>oper1</code> больше, чем <code>oper2</code>.

#### Важное замечание
Число <code>oper2</code> трактуется как беззнаковое, так что функция будет работать некорректно с отрицательными числами.

#### Пример использования
```pawn
main()
{
    new number[uint64];
    uint64fromstr(number, "50 000");

    new number2 = 440000;

    // ^^^ создаём переменные и присваиваем значения.

    if(uint64cmp32(number, number2) == UINT64_LESS)
    {
        print("число number - меньше чем number2");
        // ^^^ всегда будет срабатывать, поскольку в
        // переменную number записывается значение
        // меньше, чем в number2.
    }
    else
    {
        print("число number не меньше чем number2");
        // ^^^ никогда не сработает по вышеизложенным
        // причинам.
    }
}
```