
/* Сложить два числа uint64.

   Результат этой операции будет записан в oper1.
*/
stock uint64add(oper1[uint64], const oper2[uint64])
{
	uint64add32(oper1, oper2[uint64_lowhalf]);

#if UINT64_DEBUG >= 1

	if(uint32cmp((~oper1[uint64_highhalf]), oper2[uint64_highhalf]) == UINT64_LESS)
		uint64errno = uint64_erroverflow;
#endif

	oper1[uint64_highhalf] += oper2[uint64_highhalf];
}

/* Сложить число uint64 с 32-битным беззнаковым числом.

   Результат этой операции будет записан в number.
*/
stock uint64add32(number[uint64], add32)
{
	goto check;
	// Это - обход ограничений компилятора,
	// из-за которых невозможно переходить
	// на метки, которые объявлены после
	// кода, который на эту метку переходит.

overflow:
#pragma unused overflow
	#emit ZERO.pri
	#emit STOR.I

	#emit CONST.pri uint64_erroverflow
	#emit STOR.pri uint64errno
	
	#emit RETN

carrying:
	#emit INVERT
	#emit ADD
	#emit SREF.S.pri number

	#emit LOAD.S.pri number
	#emit ADD.C 4
	
#if UINT64_DEBUG >= 1

	#emit MOVE.alt
	#emit LOAD.I

	#emit INVERT
	#emit JZER overflow
	#emit INVERT
	#emit STOR.I
#else

	#emit INC.I
#endif

	#emit RETN

check:
	#emit LREF.S.pri number
	#emit INVERT
	#emit LOAD.S.alt add32

	#emit JLESS carrying

	#emit INVERT
	#emit ADD
	#emit SREF.S.pri number
	#emit RETN

	return;
}

/* Инкремент числа uint64.

   Вызов этой функции - быстрее, чем uint64add32(number, 1);
*/
stock uint64inc(number[uint64])
{
	if(number[uint64_lowhalf] == 0xFFFFFFFF)
	{
	#if UINT64_DEBUG >= 1
		if(number[uint64_highhalf] == 0xFFFFFFFF)
			uint64errno = uint64_erroverflow;
	#endif
		
		number[uint64_highhalf] += 1;
	}

	number[uint64_lowhalf] += 1;
}
