
/* 64-битная беззнаковая арифметика для Pawn.

   Она использует массив из двух 32-битных чисел.
   Такая реализация арифметики - очень медленная,
   но она может пригодиться там, где высокая скорость
   не является решающим фактором.

   Эта реализация будет работать исключительно при условии
   использования 32-битных ячеек интерпретатором Pawn.

   Пример использования:

    new number[uint64]; // создание переменной для числа (выделение памяти под число)
    uint64fromstr(number, "550000000"); // преобразование из строки
    uint64add(number, number); // добавление числа

    new string[50];
    uint64tostr(string, sizeof(string), number); // преобразование в строку
    print(string);
*/

#if defined(UINT64_INCLUDED)
    #endinput
#endif
#define UINT64_INCLUDED

#if cellbits != 32
    #error Pawn compiler has non-32 bit cells.
    // Компилятор Pawn использует ячейки с длиной, отличной от 32 бит
    //
    // Текст представлен на англ. языке для избежания
    // проблем с кодировкой при отображении ошибки компилятором.
#endif

#if !defined(UINT64_DEBUG)
    /* Этот макрос определяет степень отладки.

       0 - отсутствие отладки (наивысшая скорость операций, по умолчанию)
       1 - включение вывода ошибок (вывод в uint64errno, сниженная скорость)
       
       В дальнейшем количество степеней может стать больше.
    */
    #define UINT64_DEBUG 0
#endif

/* Эта конструкция улучшает читаемость при создании числа uint64.

   Пример использования:
   
    new number[uint64];
*/
enum uint64
{
    /* Индекс нижней половины числа uint64. */
    uint64_lowhalf = 0,

    /* Индекс верхней половины числа uint64. */
    uint64_highhalf = 1
}

/* Перечисление ошибок, которые могут возникнуть при
   операциях над числами uint64.
*/
enum uint64err
{
    /* Операция была выполнена успешно. */
    uint64_errnoerror = 0,

    /* При выполнении операции случилось переполнение. */
    uint64_erroverflow = 1,

    /* Были обнаружены неверные аргументы операции.
    
       Типичной причиной появления такой ошибки является,
       например, вызов uint64tostr со слишком маленьким массивом
       в качестве выходной строки.
    */
    uint64_errinvalidarg = 2
}

/* Переменная, содержащая состояние выполнения последней операции.

   Код ошибки будет записываться в неё, только в том случае, если
   макрос UINT64_DEBUG равен 1 или более.
   Список всех ошибок имеется в enum uint64err (файл uint64.inc).

   Для проверки статуса некоторой операции необходимо задать значение
   uint64_errnoerror перед её выполнением.
*/
new uint64err:uint64errno = uint64_errnoerror;
#pragma unused uint64errno

/* Конвертировать 32-битное беззнаковое число в uint64.

   Пример использования:

    new number[uint64];
    uint64from32(number, 1048576);
*/
stock uint64from32(dest[uint64], value)
{
    dest[uint64_lowhalf] = value;
    dest[uint64_highhalf] = 0;
}

/* Установить ноль как значение числа uint64. */
stock uint64zero(value[uint64])
{
    #emit LOAD.S.alt value
    #emit ZERO.pri
    #emit STOR.I
    // Обнуление первой половины

    #emit XCHG
    #emit ADD.C 4
    #emit XCHG
    #emit STOR.I
    // Обнуление второй половины
}

#include "uint64/compare"
#include "uint64/addition"
#include "uint64/subtraction"
#include "uint64/multiplication"
#include "uint64/division"
#include "uint64/bench"

#include <string>

/* Конвертировать строку с числом в десятичной
   системе счисления в число uint64.

   Конечный результат записывается в число,
   указанное в первом аргументе.
*/
stock uint64fromstr(dest[uint64], const string[])
{
    uint64zero(dest);

    new length = strlen(string);
    for(new i = 0; i < length; i++)
    {
        if(string[i] < '0' || string[i] > '9')
            continue;

        uint64mul16(dest, 10);
        uint64add32(dest, (string[i] - '0'));
    }
}

/* Конвертировать число uint64 в строку с записью
   числа в десятичной системе счисления.

   Аргументы функции:
   * str - массив строки
   * length - его длина
   * num - число, которое необходимо преобразовать
   * putspaces - при включении данного параметра, функция вставляет
   пробелы через каждые три цифры для улучшения читаемости.
*/
stock uint64tostr(str[], length, const num[uint64], putspaces = true)
{
    if(length < 1)
    {
    #if UINT64_DEBUG >= 1
        uint64errno = uint64_errinvalidarg;
    #endif	
        return;
    }
    // Если длина данной строки меньше единицы, то
    // это значит, что туда нельзя будет записать
    // даже нуль-терминатор, т.е. аргумент - неверный.

    new numcopy[uint64];
    numcopy = num;
    // Копируем значение, оно будет изменено при делении.

    length--;
    // Один символ отводится под нуль-терминатор, поэтому его
    // не учитываем в длине.

    new i, digits = 0;
    for(i = 0; (!uint64iszero(numcopy) || i < 1); i++)
    {
        if(i == length)
        {
        #if UINT64_DEBUG >= 1
            uint64errno = uint64_errinvalidarg;
        #endif

            i--;
            break;
        }

        if(putspaces && digits == 3)
        {
            digits = 0;
            str[i] = ' ';
        }
        else
        {
            str[i] = ('0' + uint64div16(numcopy, 10));
            digits++;
        }
    }

    new end = i>>1;
    // При таком способе разворачивания строки, необходимо
    // доходить лишь до её середины.

    i--;
    // Индекс последнего символа на единицу меньше итератора.
    
    for(new j = 0; j < end; j++)
    {
        new tmp = str[j];
        
        str[j] = str[i-j];
        str[i-j] = tmp;
    }

    str[i+1] = '\0';
    // В строке нужен нуль-терминатор, по нему
    // определяется конец строки.
}

/* Вывести число uint64 в шестнадцатиричной
   системе счисления в консоль.
*/
stock uint64print(const num[uint64])
{
    printf("%08x:%08x", num[uint64_highhalf], num[uint64_lowhalf]);
}

/* Макрос, позволяющий скопировать число из первой
   переменной во вторую.
*/
#define uint64cpy(%0,%1); \ 
    %0[uint64_lowhalf] = %1[uint64_lowhalf]; \
    %0[uint64_highhalf] = %1[uint64_highhalf];
