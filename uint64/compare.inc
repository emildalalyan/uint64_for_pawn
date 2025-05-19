#define UINT64_GREATER 1
#define UINT64_EQUAL 0
#define UINT64_LESS -1

/* Проверить, является ли число uint64 нулем. */
stock bool:uint64iszero(const oper[uint64])
{
	return !(oper[uint64_highhalf] || oper[uint64_lowhalf]);
}

/* Сравнить два беззнаковых 32-битных числа. */
stock uint32cmp(oper1, oper2)
{
#pragma warning push
#pragma warning disable 225 /* (unreachable code) */
	goto check;
	// Это - обход ограничений компилятора,
	// из-за которых невозможно переходить
	// на метки, которые объявлены после
	// кода, который на эту метку переходит.

isequal:
	return UINT64_EQUAL;

check:
	#emit LOAD.S.pri oper1
	#emit LOAD.S.alt oper2
	
	#emit JEQ isequal
	#emit GRTR
	#emit SHL.C.pri 1
	#emit ADD.C 0xFFFFFFFF
	#emit RETN

	return 0xFF;
#pragma warning pop
}

/* Сравнить число uint64 с 32-битным беззнаковым числом. */
stock uint64cmp32(const oper1[uint64], oper2)
{
	if(oper1[uint64_highhalf] != 0)
		return UINT64_GREATER;
	else
		return uint32cmp(oper1[uint64_lowhalf], oper2);
}

/* Сравнить два числа uint64.
   
   Функция возвращает UINT64_EQUAL, если числа равны,
   UINT64_GREATER, если первое число больше второго,
   и UINT64_LESS, если первое число меньше второго.

   См. макросы в файле "uint64/compare.inc".
*/
stock uint64cmp(oper1[uint64], oper2[uint64])
{
#pragma warning push
#pragma warning disable 225 /* (unreachable code) */
	goto check;
	// Это - обход ограничений компилятора,
	// из-за которых невозможно переходить
	// на метки, которые объявлены после
	// кода, который на эту метку переходит.

isless:
	return UINT64_LESS;

isgreater:
	return UINT64_GREATER;

check:
	#emit LOAD.S.pri oper1
	#emit LOAD.S.alt oper2

	#emit ADD.C 4
	#emit LOAD.I
	#emit XCHG
	#emit ADD.C 4
	#emit LOAD.I
	#emit XCHG

	#emit JLESS isless
	#emit JGRTR isgreater

	#emit LOAD.S.pri oper1
	#emit LOAD.S.alt oper2

	#emit LOAD.I
	#emit XCHG
	#emit LOAD.I
	#emit XCHG

	#emit JLESS isless
	#emit JGRTR isgreater

	return UINT64_EQUAL;
	
#pragma warning pop
}
