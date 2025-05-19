
/* Умножить число uint64 на 16-битное беззнаковое число mult16.

   Если значение mult16 не укладывается в 16-бит, то функция обрежет
   его до 16-бит. Таким образом, максимальный множитель - 65 535.
*/
stock uint64mul16(number[uint64], mult16)
{
	mult16 &= 0xFFFF;

	new lowhalf, highhalf;
	#emit LREF.S.pri number
	#emit CONST.alt 0xFFFF
	#emit AND
	
	#emit LOAD.S.alt mult16
	#emit UMUL
	#emit STOR.S.pri lowhalf
	
	#emit LREF.S.pri number
	#emit SHR.C.pri 16
	#emit UMUL
	#emit MOVE.alt
	#emit SHL.C.pri 16
	#emit SREF.S.pri number
	#emit SHR.C.alt 16
	#emit STOR.S.alt highhalf

	#emit LOAD.S.pri number
	#emit ADD.C 4
	#emit LOAD.I

	#emit LOAD.S.alt mult16
	#emit UMUL
	
	#emit MOVE.alt
	#emit LOAD.S.pri number
	#emit ADD.C 4
	#emit XCHG
	#emit STOR.I

	uint64add32(number, lowhalf);

#if UINT64_DEBUG >= 1
	if(uint32cmp((~number[uint64_highhalf]), highhalf) == UINT64_LESS)
		uint64errno = uint64_erroverflow;
#endif
	number[uint64_highhalf] += highhalf;
}
