## uint64_for_pawn — 64-bit numbers for SAMP game modes
This library implements storage and arithmetic on 64-bit numbers using standard 32-bit Pawn arithmetic.

**ATTENTION!** The library is designed to work with [the Pawn Community Compiler](https://github.com/pawn-lang/compiler).

### Advantages
💳 Such numbers can be useful in the implementation of in-game systems of banks, casinos, etc., as a player can get
enough money to feel the maximum imposed by the 32-bit signed representation of numbers.
(the maximum is **2 147 483 647**).

Storing a balance value using the 64-bit unsigned representation
allows to "raise the bar" of the maximum. ((2^64)-1 = **18 446 744 073 709 551 615**)

🧰 At the same time, the library is "*header-only*", so it can be embedded in a code using the <code>#include</code> directive
and does not require plug-ins to be loaded.

### Usage example
The following program converts strings into two numbers, adds them up, and displays the result in the console:
```pawn
#include <uint64>

main()
{
    new number1[uint64]; // creating the variable for the number (and allocating memory for it)
    uint64fromstr(number1, "550000000"); // conversion from the string
    
    new number2[uint64]; // creating the variable for the number (and allocating memory for it)
    uint64fromstr(number2, "48255024242"); // conversion from the string
    
    uint64add(number1, number2); // adding the number2, the result is written to the first variable.
    
    new string[50];
    uint64tostr(string, sizeof(string), number1); // conversion to the string
    print(string);
}
```

### Calculation speed
⏱️ The main disadvantage of this implementation is **a speed of calculations**.

Despite of the optimization of operations *(for example, some functions were rewritten in the AMX assembly language)*,
an execution time of many operations **is several times longer** than similar standard operations on 32-bit numbers.

🚨 This disadvantage is compensated by the rare use of these numbers. It is recommended to use them **only in necessary places** *(see [Advantages](#Advantages))*.
In this case, speed of operations won't become a "stumbling block".

### Documentation
Sorry, but there is no English documentation yet.

Now, there are documentations only in these languages:
* [Документация на русском языке](docs-ru/Main.md)