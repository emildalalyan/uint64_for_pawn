
/* Вычесть одно число uint64 из другого числа uint64. */
stock uint64sub(oper1[uint64], oper2[uint64])
{
#if UINT64_DEBUG >= 1
	if(uint64cmp(oper1, oper2) == UINT64_LESS)
		uint64errno = uint64_erroverflow;
#endif

	if(uint32cmp(oper1[uint64_lowhalf], oper2[uint64_lowhalf]) == UINT64_LESS)
		oper1[uint64_highhalf] -= 1;

	oper1[uint64_highhalf] -= oper2[uint64_highhalf];
	oper1[uint64_lowhalf] -= oper2[uint64_lowhalf];
}

/* Вычесть 32-битное беззнаковое число из числа uint64. */
stock uint64sub32(oper1[uint64], oper2)
{
	goto check;
	// Это - обход ограничений компилятора,
	// из-за которых невозможно переходить
	// на метки, которые объявлены после
	// кода, который на эту метку переходит.

overflow:
#pragma unused overflow
	#emit CONST.pri uint64_erroverflow
	#emit STOR.pri uint64errno

	#emit CONST.pri 0xFFFFFFFF
	#emit STOR.I

	#emit RETN

borrow:
	#emit SUB
	#emit LOAD.S.alt oper1
	#emit STOR.I

	#emit MOVE.pri
	#emit ADD.C 4

#if UINT64_DEBUG >= 1

	#emit MOVE.alt
	#emit LOAD.I
	#emit JZER overflow

	#emit DEC.pri
	#emit STOR.I
#else

	#emit DEC.I
#endif

	#emit RETN

check:
	#emit LREF.S.pri oper1
	#emit LOAD.S.alt oper2

	#emit JLESS borrow

	#emit SUB
	#emit LOAD.S.alt oper1
	#emit STOR.I
	#emit RETN
}

/* Декремент числа uint64.

   Вызов этой функции - быстрее, чем uint64sub32(number, 1);
*/
stock uint64dec(number[uint64])
{
	if(number[uint64_lowhalf] == 0)
	{
	#if UINT64_DEBUG >= 1
		if(number[uint64_highhalf] == 0)
			uint64errno = uint64_erroverflow;
	#endif

        number[uint64_highhalf] -= 1;
	}

    number[uint64_lowhalf] -= 1;
}
