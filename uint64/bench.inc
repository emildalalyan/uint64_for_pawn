
/* Тест скорости для 64-битной арифметики.

   Весь вывод ведется в стандартной консоли.
   Внимание! Все сообщения написаны на английском языке.
   Так было сделано для предотвращения ошибок с кодировкой, так как
   большинство кодировок совместимы с ASCII.

   Для начала теста нужно вызвать функцию uint64bench.
*/

#include <time>

stock _uint64benchadd32test(const initialvalue[uint64], oper2, precision, time)
{
    new testvar[uint64];
    testvar = initialvalue;

    new i;
    new inittime = gettime();
    for(i = 0; ; i++)
    {
        uint64add32(testvar, oper2);

        if(!(i%precision) && ((gettime()-inittime) >= time))
            break;
    }
    _uint64benchprintresult(i, time);
}

stock _uint64benchsub32test(const initialvalue[uint64], oper2, precision, time)
{
    new testvar[uint64];
    testvar = initialvalue;

    new i;
    new inittime = gettime();
    for(i = 0; ; i++)
    {
        uint64sub32(testvar, oper2);

        if(!(i%precision) && ((gettime()-inittime) >= time))
            break;
    }
    _uint64benchprintresult(i, time);
}

stock _uint64benchmultest(const initialvalue[uint64], oper2, precision, time)
{
    new testvar[uint64];
    testvar = initialvalue;

    new i;
    new inittime = gettime();
    for(i = 0; ; i++)
    {
        uint64mul16(testvar, oper2);

        if(!(i%precision) && ((gettime()-inittime) >= time))
            break;
    }
    _uint64benchprintresult(i, time);
}

stock _uint64benchdivtest(const initialvalue[uint64], oper2, precision, time)
{
    new testvar[uint64];
    testvar = initialvalue;

    new i;
    new inittime = gettime();
    for(i = 0; ; i++)
    {
        uint64div16(testvar, oper2);

        if(!(i%precision) && ((gettime()-inittime) >= time))
            break;
    }
    _uint64benchprintresult(i, time);
}

stock _uint64benchprintresult(iterations, time)
{
    new Float:speed;
    speed = float(iterations)/float(time);

    if(speed > 1000000.0)
        printf(" -> %i operations per %i seconds (~%.3f megaops)", iterations, time, (speed/1000000.0));
    else if(speed > 1000.0)
        printf(" -> %i operations per %i seconds (~%.3f kiloops)", iterations, time, (speed/1000.0));
    else
        printf(" -> %i operations per %i seconds (~%.3f ops )", iterations, time, speed);
}

/* Начать тест скорости арифметики.

   Аргументы функции:
   * initialvalue - тестовое 64-битное значение (любое случайное значение),
   * oper2 - тестовое 32-битное значение (любое случайное значение),
   * precision - точность в определении кол-ва итераций (рекомендуется 10000),
   * time - время самого длинного теста.
*/
stock uint64bench(const initialvalue[uint64], oper2, precision, time)
{
    if(time == 0 || precision == 0)
    {
    #if UINT64_DEBUG >= 1
        uint64errno = uint64_errinvalidarg;
    #endif

        return;
    }

    print("\n== uint64 performance benchmark ================================");
    printf(" Benchmark is running with following input parameters: \n");

    printf(" Primary number (initial 64-bit value):   %08x_%08x",
        initialvalue[uint64_highhalf],
        initialvalue[uint64_lowhalf]);
    printf(" Secondary number (initial 32-bit value): %08x (%d)",
        oper2, oper2);
    printf(" Precision:       %d iterations", precision);
    printf(" Benchmark time:  %d seconds max", time);
    printf(" Debug level:     %d", UINT64_DEBUG);

    print("\n================================================================");

    print(" Division (uint64div16): ");
    new testtime = time;
    do _uint64benchdivtest(initialvalue, oper2, precision, testtime);
    while(testtime >>= 1)

    print("\n Multiplication (uint64mul16): ");
    testtime = time;
    do _uint64benchmultest(initialvalue, oper2, precision, testtime);
    while(testtime >>= 1)

    print("\n Subtraction (uint64sub32): ");
    testtime = time;
    do _uint64benchsub32test(initialvalue, oper2, precision, testtime);
    while(testtime >>= 1)

    print("\n Addition (uint64add32): ");
    testtime = time;
    do _uint64benchadd32test(initialvalue, oper2, precision, testtime);
    while(testtime >>= 1)

    print("\n== uint64 performance benchmark end ============================\n");
}
