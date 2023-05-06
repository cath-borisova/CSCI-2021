
/*
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

Replace the "return" statement in each function with one
or more lines of C code that implements the function. Your code
must conform to the following style:

int Funct(arg1, arg2, ...) {
    /* brief description of how your implementation works */
    int var1 = Expr1;
    ...
    int varM = ExprM;

    varJ = ExprJ;
    ...
    varN = ExprN;
    return ExprR;
}

Each "Expr" is an expression using ONLY the following:
1. Integer constants 0 through 255 (0xFF), inclusive. You are
not allowed to use big constants such as 0xffffffff.
2. Function arguments and local variables (no global variables).
3. Unary integer operations ! ~
4. Binary integer operations & ^ | + << >>

Some of the problems restrict the set of allowed operators even further.
Each "Expr" may consist of multiple operators. You are not restricted to
one operator per line.

You are expressly forbidden to:
1. Use any control constructs such as if, do, while, for, switch, etc.
2. Define or use any macros.
3. Define any additional functions in this file.
4. Call any functions.
5. Use any other operations, such as &&, ||, -, or ?:
6. Use any form of casting.
7. Use any data type other than int.  This implies that you
cannot use arrays, structs, or unions.


You may assume that your machine:
1. Uses 2s complement, 32-bit representations of integers.
2. Performs right shifts arithmetically.
3. Has unpredictable behavior when shifting if the shift amount
is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
/*
 * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
 */
int pow2plus1(int x) {
    /* exploit ability of shifts to compute powers of 2 */
    return (1 << x) + 1;
}

/*
 * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
 */
int pow2plus4(int x) {
    /* exploit ability of shifts to compute powers of 2 */
    int result = (1 << x);
    result += 4;
    return result;
}

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
1. Define or use any macros.
2. Define any additional functions in this file.
3. Call any functions.
4. Use any form of casting.
5. Use any data type other than int or unsigned.  This means that you
cannot use arrays, structs, or unions.
6. Use any floating point data types, operations, or constants.


NOTES:
1. Use the dlc (data lab checker) compiler (described in the spec) to
check the legality of your solutions.
2. Each function has a maximum number of operations (integer, logical,
or comparison) that you are allowed to use for your implementation
of the function.  The max operator count is checked by dlc.
Note that assignment ('=') is not counted; you may use as many of
these as you want without penalty.
3. Use the btest test harness to check your functions for correctness.
4. The maximum number of ops for each function is given in the
header comment for each function. If there are any inconsistencies
between the maximum ops in the writeup and in this file, consider
this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */
#endif

/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
	/*
	Truth table:
		First half:
		x | y | ~x | ~x&y | x&y | ~(x&y) | (~x&y) & ~(x&y) | ~((~x&y) & ~(x&y))		the values here are currently inversed, so
		1 | 1 | 0  | 0    | 1   | 0      | 0               | 1				when x and y are both 1, that means false
		1 | 0 | 0  | 0    | 0   | 1      | 0               | 1				when x is 1 and y is 0 its false (but the second half of the code will fix that)
		0 | 1 | 1  | 1    | 0   | 1      | 1               | 0 				when x is 0 and y is 1, its true
		0 | 0 | 1  | 0    | 0   | 1      | 0               | 1				when x and y are both 0, its false
		Second half:
		x | y | ~y | x&~y | x&y | ~(x&y) | (x&~y) & ~ (x&y) | ~((x&~y) & ~(x&y))
		1 | 1 | 0  | 0    | 1   | 0      | 0                | 1				false
		1 | 0 | 1  | 1    | 0   | 1      | 1                | 0				true
		0 | 1 | 0  | 0    | 0   | 1      | 0                | 1				false
		0 | 0 | 1  | 0    | 0   | 1      | 0                | 1				false
		Then, you combine the two halves and negate it to get the actual answer
		x | y | ~((~x&y) & ~(x&y)) | ~((x&~y) & ~(x&y)) | (~((~x&y) & ~(x&y))) & (~((x&~y) & ~(x&y))) | negated version
		1 | 1 | 1                  | 1                  | 1                                           | 0
		1 | 0 | 1                  | 0                  | 0                                           | 1
		0 | 1 | 0                  | 1                  | 0                                           | 1
		0 | 0 | 1                  | 1                  | 1                                           | 0
	*/
    return ~((~((~x&y) & ~(x&y))) & (~((x&~y) & ~(x&y))));
}

/*
 * isZero - returns 1 if x == 0, and 0 otherwise
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
	//if a number is 0, ! will switch it to 1 and vice versa
    return !x;
}

/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
	/*
	Mask x with fourthByte which is equivalent to 0xAAAAAAAA, which is where all the odd bits are set to 1
	(x & fourthByte) will return a binary int where all the odd bits that x and fourthByte had in common are set to 1.
	Then, if that is equivalent to fourthByte (meaning x had all the odd bits set to 1), it will return true.
	To evaluate if that is true, xOR was used. if a single odd bit was not set to 1 in x, then fourthByte > 0
	*/
	int firstByte = 0xAA;
	int secondByte = firstByte | (0xAA <<8);
	int thirdByte = secondByte | (0xAA <<16);
	int fourthByte = thirdByte | (0xAA << 24);
	return !((x & fourthByte) ^ fourthByte);
}

/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
	/*
	   posFits is for the case of a positive number, so x is left unchanged.
	   To check to see it can fit in n, it's shifted by n-1. If any 1's remain (so doesn't fit), !(x>>n) will return 0, otherwise 1.
	   That result is compared with the sign bit to make sure its a positive number.

	   negFits is for the case of a neg number, so x is inversed because a number less than -1 changes bits from 1s to 0s so its the opposite of the pos case.
	   X (neg) is then shifted by n, and if any 1's remain (doesn't fit), !(neg>>n) returns 0, otherwise 1.
	   That result is then compared with the sign bit to make sure its a negative number.
	*/
	int neg = ~x;
	int sign = 1 & (x>>31);
	int posFits = 0;
	int negFits = 0;
	n = n + (~0);
	posFits = ((!(x>>n))) & (!sign);
	negFits = ((!(neg>>n))) & sign;
	return posFits | negFits;
}

/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
	/*
	  A number is NaN if it's greater than 0x7f800000.
	  Otherwise, a number is made positive by inversing it to make it easier to change the sign bit. After setting the sign bit to 1, it can be ivnersed again and now the sign bit is 0 if it wasn't previously.
	*/
	int pos = ~uf;
	pos = pos | (1 << 31);
	pos = ~pos;
	if(pos > 0x7f800000){
		return uf;
	} else{
		return pos;
	}
}

/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
	/*
	  A number NaN if it's greater than 0x7f800000.
	  Both 0x0 and 0x80000000 are ways to represent 0, so if uf and ug are either one of those, 1 is returned.
	  Otherwise, returns uf == ug, which returns 1 if true and 0 if false.
	*/
	int posUF = ~uf;
	int posUG = ~ug;
	posUF = posUF | (1 << 31);
	posUG = posUG | (1 << 31);
	posUF = ~posUF;
	posUG = ~posUG;
	if(posUF > 0x7f800000 || posUG > 0x7f800000){
		return 0;
	} else if ((uf == 0x80000000 || uf == 0) && (ug == 0x80000000 || ug == 0)){
		return 1;
	}
    return uf == ug;
}

/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (least significant) to 3 (most significant)
 *   Examples: getByte(0x12345678, 1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
	//The nth byte is shifted to the least significant spot and then compared to 0xFF, that way any bytes in front of it are thrown out.
	int shift = n << 3;
	int byte = x >> shift;
	byte = byte & (0xFF);
    return byte;
}

/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
	/*
	  Addition caused an overflow error if the sum sign became a 0 when the sign of x and y are 1 or if the sum sign became a 1 when the sign of x and y are 0.
	  So the signs of the sum, x and y are isolated to be compared.
	*/
	int sum = x + y;
	int sumSign = sum;
	int xSign = sum;
	int ySign = sum;
	sumSign = 1 & (sum >> 31);
	xSign = 1 & (x>>31);
	ySign = 1 & (y>>31);
return !(((!xSign & !ySign) & sumSign) | ((xSign & ySign) & (!sumSign)));
}

/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
	//Ones is the mask, which is first set to 0xFFFFFFFE becaues the least significant bit needs to be 0.
	int ones = 0xfe;
	ones = ones | (0xff << 8);
	ones = ones | (0xff << 16);
	ones = ones | (0xff << 24);
	ones = ones ^ (ones << (highbit)); //Here, ones is setting any bit before the highbit to 0.
	ones = ones | (1<<0);
	ones = ones >> lowbit; //Here and in the next line, ones is setting any bit after the lowbit to 0.
	ones = ones << lowbit;
    return ones;
}

/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
	/*
	  subtract finds byte n, and then "subtracts" the value of that byte from c.
	  This is done by inversing subtract and &-ing it with x so that byte n becomes all 0's in x.
	  Then, to replace n with c, c is simply added to x.
	*/
	int shift = n << 3;
	int subtract = 0;
	c = c << shift;
	subtract = subtract | (0xFF << shift);
	subtract = ~ subtract;
	x = x & subtract;
	x = x + c;
	return x;
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
	/*
	  A number is a power of 2 if it is positive, nonzero and has exactly one bit set to 1.
	  number = x-1, and is |-ed with shifted (which is x shifted right 1). If x is in fact a power of 2, this operation should result in n 1's, where n is equal to the position to the right of the 1-bit in x, which is equal to
	  number.
	*/
	int neg = 1 & (x>>31);
	int number = x;
	int shifted = x;
	int isNotNeg = 0;
	int isNotZero = 0;
	int ones = ~0;
	number = number + ones;
	shifted = shifted >> 1;
	isNotNeg = !((neg & 0) ^ 0); //Equals 1 if x is not negative.
	isNotZero = !!x;	    //Equals 1 if x is not 0.
	return ((!((shifted | number) ^ number)) & isNotNeg) & isNotZero; //Checks to see if shifted | number == number and if x is not negative and not zero.
}

/*
 * floatScale4 - Return bit-level equivalent of expression 4*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale4(unsigned uf) {
	/*
	  Separated the exponent, sign and value (mantissa). Then, if the exponent was equal to all 1's (0xFF), the number was NaN.
	  If the exponent was all 0's (0x0), x was denormalized. So if the value was 0, then 0 x 4 is still 0.
	  If value was > 0, then it could be multiplied by 4, but if value took up more than 23 bits (number of bits allocated for mantissa), it had to be normalized.
	  So a leading 1 was implied, and the exponent increased.
	  Otherwise, if the exponent was > 0, it right shifted the exponent by 2 (equivalent to multiplying by 4); however, if this resulted in the exponent equaling 0xff, the value of NaN was returned.
	*/
	unsigned sign = 0x80000000 & uf;
	unsigned exponent = (uf & 0x7f800000) >> 23;
	unsigned value = uf & 0x7FFFFF;
	if(exponent == 0xff){ //Checks to see if NaN.
		return uf;
	} else if(exponent == 0){ //Checks to see if 0.
		if(value == 0){ //Checks to see if uf = 0.
			return uf;
		}
		if(value <= 0x1fffff){ //If value is less than 1/4 of 0x7fffff (max value for mantissa).
			value = value << 2; //Then multiply it by 4 (aka left shift 2).
		} else if (value <= 0x3fffff){ //If value is less than 1/2 of 0x7fffff (max value for mantissa).
			value = value << 1; //Then multiply it by 2 (aka left shift 1).
			exponent = 0x1; //Set the exponent to 1 to multiply it by another 2.
		} else {		//If the value is too big to be multiplied by itself.
			value = value << 1; //Then left shift it 1.
			value = value & (0x7fffff); //Then only use the first 23 bits.
			exponent = 0x2; //And set the exponent to 2 to multiply it by 4.
		}
		exponent = exponent << 23;
		return sign |exponent | value;
	} else if((exponent + 2) > 0xfe){ //Checks to see if adding 2 to the exponent will cause uf to become NaN.
		return sign | 0x7f800000;
	} else {
		exponent = exponent + 2; //Otherwise add 2 to the exponent.
		exponent = exponent << 23;
		return sign | exponent | value;
	}
}
