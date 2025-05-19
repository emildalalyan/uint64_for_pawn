
/* Поделить число uint64 на 16-битное беззнаковое число div16.

   Если значение div16 не укладывается в 16-бит, то функция обрежет
   его до 16-бит. Таким образом, максимальный делитель - 65 535.

   Внимание! Если UINT64_DEBUG меньше 1, то функция не проверяет входной аргумент div8,
   таким образом при попытке деления на ноль могут возникнуть ошибки интерпретатора.
   Это было сделано для ускорения работы функции.

   Функция записывает результат во входную переменную number и
   возвращает остаток от деления.
*/
stock uint64div16(number[uint64], div16)
{
    div16 &= 0xFFFF;

#if UINT64_DEBUG >= 1
    if(!div16)
    {
        uint64errno = uint64_errinvalidarg;
        return 0;
    }
#endif

    new remainder, quot, lowhalf;

    #emit LOAD.S.pri number
    #emit ADD.C 4
    #emit LOAD.I
    #emit LOAD.S.alt div16
    #emit UDIV
    #emit STOR.S.pri quot

    #emit MOVE.pri
    #emit LOAD.S.alt div16
    #emit SHL.C.pri 16
    #emit UDIV
    #emit SHL.C.pri 16
    #emit STOR.S.pri lowhalf

    #emit MOVE.pri
    #emit LOAD.S.alt div16
    #emit SHL.C.pri 16
    #emit UDIV
    #emit STOR.S.alt remainder
    #emit LOAD.S.alt lowhalf
    #emit OR
    #emit STOR.S.pri lowhalf
    number[uint64_highhalf] = quot;

    quot = number[uint64_lowhalf];
    #emit LOAD.S.pri quot
    #emit LOAD.S.alt div16
    #emit UDIV
    #emit STOR.S.pri quot
    #emit LOAD.S.pri remainder
    #emit ADD
    #emit STOR.S.pri remainder
    number[uint64_lowhalf] = quot + lowhalf;

    //uint64add32(number, lowhalf);
    uint64add32(number, remainder/div16);
    remainder %= div16;

    return remainder;
}
