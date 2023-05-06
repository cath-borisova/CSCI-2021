
## Read the following instructions carefully.
##
## You will provide your solution to the Data Lab by
## editing the collection of functions in this source file.
##
## A couple of rules from Project 2 are still in effect for your assembly code:
## 2. No global variables allowed.
## 3. You may not define or call any additional functions in this file.
##
## You may assume that your machine:
## 1. Uses two's complement, 32-bit representations of integers.
## 2. Performs right shifts arithmetically.
## 3. Has unpredictable behavior when shifting if the shift amount
## is less than 0 or greater than 31.
##
## Use the btest program to check your functions for correctness.

.text

# bitXor - x^y using only ~ and &
#   Example: bitXor(4, 5) = 1
#   Legal ops: ~ &
#   Max ops: 14
#   Rating: 1
#
.global bitXor
bitXor:
	movl 4(%esp), %eax	#move x to %eax
	movl 8(%esp), %ecx	#move y to %ecx
	xor %ecx, %eax		# x ^ y
   	ret

# isZero - returns 1 if x == 0, and 0 otherwise
#   Examples: isZero(5) = 0, isZero(0) = 1
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 2
#   Rating: 1
#
.global isZero
isZero:
	movl 4(%esp), %ecx	#move x into %ecx
	test %ecx, %ecx		#bitwise and
	jz EQUAL		#if %ecx == 0, jump to EQUAL
	movl $0, %eax		#return 0
	ret
	EQUAL:
	movl $1, %eax		#return 1
	ret

# allOddBits - return 1 if all odd-numbered bits in word set to 1
#   where bits are numbered from 0 (least significant) to 31 (most significant)
#   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 12
#   Rating: 2
#
.global allOddBits
allOddBits:
	movl 4(%esp), %eax		#move x into %eax
	movl $0xAAAAAAAA, %ecx		#move 0xAAAAAAAA into %ecx
	and %ecx, %eax			#x = x & 0xAAAAAAAA
	xor %ecx, %eax			# x= x ^ 0xAAAAAAAAA
	test %eax, %eax			# if x == 0 then all the odd bits are set to 1
	jz ODD				#jump to ODD if x is 0
	movl $0, %eax			#return 0
	ret
	ODD:
	movl $1, %eax			#return 1
	ret

# fitsBits - return 1 if x can be represented as an
#  n-bit, two's complement integer.
#   1 <= n <= 32
#   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 15
#   Rating: 2
.global fitsBits
fitsBits:
	movl 4(%esp), %edx	# move x into %edx as orinal x (used as var sign later)
	movl 8(%esp), %eax	# move n into %eax
	pushl %ebx		# push %ebx
	movl %edx, %ebx 	# move x into %ebx for var neg
	notl %ebx		# var neg = ~x
	pushl %edi		# push %edi
	movl %edx, %edi	# move x into %edi for var shiftN
	movl $31, %ecx		# store 31 in %ecx for shifting
	shrl %ecx, %edx		# var sign = x >> 31
	andl $1, %edx		# sign = 1 & (x>>31)
	subl $1, %eax		# n = n-1
	movl %eax, %ecx		# store n in %ecx for shifting
	shrl %ecx, %edi		# var shiftN = x >> n
	shrl %ecx, %ebx		# neg = neg >> n
	testl %edx, %edx	#test if sign is neg or pos
	jz POSITIVE		#jump if the sign is pos, stay if the sign is neg
	testl %ebx, %ebx	#test if neg (~x >> n) is equal to 0
	jz NEGANDSHIFTN
	movl $0, %eax		#this means that x was a negative number and when it was inversed and shifted by n it left as least one bit, therefore it did not fit (so return 0)
	jmp FITSBITSEND
	POSITIVE:
	testl  %edi, %edi	#test if shiftN (x >> (n-1)) is equal to 0
	jz POSANDSHIFTN
	movl $0, %eax		#this means that x was a positive number and when it was shifted by n it left at least one 1 bit, therefore it did not fit (so return 0)
	jmp FITSBITSEND
	POSANDSHIFTN:		#this means that x was a positive number and when it was shifted by n it did not leave any 1 bits, therefore it fit (so return 1)
	movl $1, %eax
	jmp FITSBITSEND
	NEGANDSHIFTN:
	movl $1, %eax		#this means that x was a negative number, and when it was inversed and shifted, it did not leave any 1 bits, therefore it fit (so return 1)

	FITSBITSEND:
	popl %edi		#pop %edi
	popl %ebx 		#pop %ebx
	ret

# floatAbsVal - Return bit-level equivalent of absolute value of f for
#   floating point argument f.
#   Both the argument and result are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representations of
#   single-precision floating point values.
#   When argument is NaN, return argument..
#   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
#   Max ops: 10
#   Rating: 2
#
.global floatAbsVal
floatAbsVal:
	movl 4(%esp), %eax	#move uf into %eax
	movl %eax, %ecx		#move uf into %ecx for var pos
	notl %ecx		# var pos = ~uf
	orl $0x80000000, %ecx	# pos = pos | (0x80000000)
	notl %ecx		# pos = ~pos
	movl $0x7f800000, %edx	#move value of NaN for comparison
	cmpl %edx, %ecx		# pos > 0x7f800000
	jg ABSVALNAN		#then its inf, so jump to ABSVALNAN	
	movl %ecx, %eax		# return the positive number
	ret
	ABSVALNAN:		#return uf
	ret


# floatIsEqual - Compute f == g for floating point arguments f and g.
#   Both the arguments are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representations of
#   single-precision floating point values.
#   If either argument is NaN, return 0.
#   +0 and -0 are considered equal.
#   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
#   Max ops: 25
#   Rating: 2
#
.global floatIsEqual
floatIsEqual:
	movl 4(%esp), %eax		#move uf into %eax
	movl 8(%esp), %edx		#move ug into %edx
	pushl %edi			#push %edi
	pushl %esi			#push %esi
	pushl %ebx			#push %ebx
	movl %eax, %edi			#move uf into %edi for var posUF
	movl %edx, %esi			#move ug into %esi for var posUG
	not %edi			#var posUF = ~uf
	not %esi			# var posUF = ~ug
	orl $0x80000000, %edi		#posUF | 0x80000000
	orl $0x80000000, %esi		#posUG | 0x80000000
	not %edi			#posUF = ~posUF
	not %esi			#posUG | ~posUG
	movl $0x7f800000, %ebx		#move the value of NaN (0x7f800000) into %ebx
	cmpl %ebx, %edi			#checks if posUF is NaN
	jg ISEQUALNAN			#if it is, then jump to ISEQUALNAN
	cmpl %ebx, %esi			#checks if posUG is NaN
	jg ISEQUALNAN			#if it is, then jump to ISEQUALNAN
	testl %esi, %esi		#checks to see if posUG is 0
	jz UGZERO			#if it is, jump to UGZERO	
	jmp EQUALITYCHECK		#otherwise check to see if uf == ug
	ISEQUALNAN:
	movl $0, %eax			#if posUG or posUF are equal to NaN, return 0
	jmp ISEQUALEND			#jump to ISEQUALEND
	UGZERO:
	testl %edi, %edi		#checks if posUF is also 0
	jz UFZERO			#if it is, jump to UFZERO
	jmp EQUALITYCHECK		#if not, check to see if uf == ug
	EQUALITYCHECK:
	cmpl %eax, %edx			#checks to see if uf == ug
	je UFEQUALUG			#if it is, jump to UFEQUALUG
	movl $0, %eax			#return 0 if uf != ug
	jmp ISEQUALEND			#jump to ISEQUALEND
	UFZERO:
	movl $1, %eax			#if posUF is also 0, return 1
	jmp ISEQUALEND			#jump to ISEQUALEND
	UFEQUALUG:
	movl $1, %eax			#returns 1 if uf == ug
	ISEQUALEND:
	popl %ebx			#pop %ebx
	popl %esi			#pop %esi
	popl %edi			#pop %edi
	ret


# getByte - Extract byte n from word x
#   Bytes numbered from 0 (least significant) to 3 (most significant)
#   Examples: getByte(0x12345678, 1) = 0x56
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 6
#   Rating: 2
#
.global getByte
getByte:
	movl 4(%esp), %eax	#move x into %eax
	movl 8(%esp), %edx	#move n into %edx
	movl $3, %ecx		#move 3 into %ecx for shifting
	shll %ecx, %edx		#n = n<<3
	movl %edx, %ecx		#move n into %ecx for shifting
	shrl %ecx, %eax		#x = x >> (n << 3)
	andl $255, %eax		# x & 0xFF
	ret


# addOK - Determine if can compute x+y without overflow
#   Example: addOK(0x80000000,0x80000000) = 0,
#            addOK(0x80000000,0x70000000) = 1,
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 20
#   Rating: 3
#
.global addOK
addOK:
	movl 4(%esp), %eax		#move x into %eax
	movl 8(%esp), %edx		#move y into %edx
	pushl %ebx			#push %ebx for var sumSign
	pushl %edi			#push %edi for var xSign
	pushl %esi			#push %esi for var ySign
	movl %edx, %ebx			#var sumSign = y
	movl %eax, %edi			#var xSign = x
	movl %edx, %esi			# var ySign = y
	addl %eax, %ebx			#sumSign = x + y
	movl $31, %ecx			#move 31 into %ecx for shifting
	shrl %ecx, %ebx			#sumSign = sumSign >> 31
	andl $1, %ebx			#sumSign = sumSign & 1
	shrl %ecx, %edi			#xSign = x >> 31
	andl $1, %edi			#xSign = xSign & 1
	shrl %ecx, %esi			#ySign = y >> 31
	andl $1, %esi			#ySign = ySign & 1
	cmpl %edi, %esi			#check to see if ySign and xSign are the same
	je SAMESIGN			#if they are, jump to SAMESIGN
	jmp ADDOK			#otherwise, jump to ADDOK
	SAMESIGN:
	cmpl %edi, %ebx			#check to see if the x and y signs are the same as sumSign
	je ADDOK			#if they are, jump to ADDOK
	jmp NOTADDOK			#otherwise jump to NOTADDOK
	ADDOK:
	movl $1, %eax			#return 1
	jmp ADDOKEND			#jump to the end
	NOTADDOK:
	movl $0, %eax			#return 0
	ADDOKEND:
	popl %esi			#pop %esi
	popl %edi			#pop %edi
	popl %ebx			#pop %ebx
	ret

# bitMask - Generate a mask consisting of all 1's
#   lowbit and highbit
#   Examples: bitMask(5,3) = 0x38
#   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
#   If lowbit > highbit, then mask should be all 0's
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 16
#   Rating: 3
#
.global bitMask
bitMask:
	movl 4(%esp), %ecx		#move highbit into %eax for shifting
	movl 8(%esp), %eax		#move lowbit into %eax
	movl $0xFFFFFFFE, %edx		#move var ones = 0xfffffffe into %edx
	pushl %ebx			#push %ebx to retain original value of ones
	movl %edx, %ebx			#move ones into ebx
	shll %ecx, %edx			# ones = ones << highbit
	xor %ebx, %edx			# ones = 0xfffffffe ^ ( 0xfffffffe << highbit)
	or $1, %edx			# ones = ones | 1
	movl %eax, %ecx 		#move lowbits into %ecx for shifting
	shrl %ecx, %edx			# ones = ones >> lowbit
	shll %ecx, %edx			# ones = ones << lowbit
	movl %edx, %eax			#return ones
	popl %ebx			#pop %ebx
	ret


# replaceByte(x,n,c) - Replace byte n in x with c
#   Bytes numbered from 0 (LSB) to 3 (MSB)
#   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
#   You can assume 0 <= n <= 3 and 0 <= c <= 255
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 10
#   Rating: 3
#
.global replaceByte
replaceByte:
	movl 4(%esp), %eax		#move x into %eax
	movl 8(%esp), %edx		#move n into %edx
	movl 12(%esp), %ecx		#move c into %ecx
	pushl %ebx			#push %ebx
	movl %ecx, %ebx			#move c into %ebx to leave %ecx for shifting values
	movl $3, %ecx			#move 3 into %excx for shifting
	shll %ecx, %edx			#n = n << 3
	pushl %edi			#push %edi for var subtract
	movl $0, %edi			# var subtract = 0
	movl %edx, %ecx			#move n into %ecx for shifting
	shll %ecx, %ebx			# c = c << n
	movl $255, %edx			#use %edx to store 0xff now
	shll %ecx, %edx			# 0xFF << n
	orl %edx, %edi			#subtract = subtract | (0xFF << n)
	not %edx			#subtract = ~subtract
	and %edx, %eax			# x = x & subtract
	addl %ebx, %eax			# x = x + c
	popl %edi			#pop %edi
	popl %ebx			#pop %ebx
	ret


# isPower2 - returns 1 if x is a power of 2, and 0 otherwise
#   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
#   Note that no negative number is a power of 2.
#   Legal ops: ! ~ & ^ | + << >>
#   Max ops: 20
#   Rating: 4
#
.global isPower2
isPower2:
	movl 4(%esp), %eax		#move x into %eax
	pushl %ebx			#push %ebx
	movl %eax, %ebx			#move x into %ebx to save the original value of x
	test %eax, %eax			#checking to see if x is 0
	jz NOTPOWERTWO			#if it is, jump to NOTPOWERTWO
	movl $31, %ecx			#move 31 into %ecx for shifting
	shrl %ecx, %eax			# x >> 31
	andl $1, %eax			# x & 1
	testl %eax, %eax		#checking sign bit
	jnz NOTPOWERTWO			#if it is not zero, jump to NOTPOWERTWO
	movl %ebx, %eax			#reset %eax to original value of x
	movl %eax, %edx			#move x into %edx for var shifted = x
	movl $1, %ecx			#move 1 into %ecx for shifting
	shrl %ecx, %edx			#shifted = shifted >> 1
	movl %eax, %ecx			#move x into %ecx for var number = x
	subl $1, %ecx			# var number = x -1
	orl %ecx, %edx			# shifted = shifted | number
	xorl %ecx, %edx			# shifted = shifted ^ number
	testl %edx, %edx		#test if the shifted is 0
	jnz NOTPOWERTWO			#if it isnt, jump to NOTPOWERTWO
	movl $1, %eax			#return 1
	jmp ISPOWERTWOEND		#jump to the end
	NOTPOWERTWO:
	movl $0, %eax			#return 0
	ISPOWERTWOEND:
	popl %ebx			#pop %ebx
	ret


# floatScale4 - Return bit-level equivalent of expression 4*f for
#   floating point argument f.
#   Both the argument and result are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representation of
#   single-precision floating point values.
#   When argument is NaN, return argument
#   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
#   Max ops: 30
#   Rating: 4
#
.global floatScale4
floatScale4:
	movl 4(%esp), %eax		#move uf into %eax
	movl %eax, %edx			#move uf into %edx for var exponent = uf
	pushl %edi			#push %edi
	pushl %esi			#push %esi
	andl $0x7f800000, %edx		#exponent = uf & 0x7f800000
	movl $23, %ecx			#move 23 into %ecx for shifting
	shrl %ecx, %edx			# exponent = exponent >> 23
	movl %eax, %edi			#move uf into %edi for var value = uf
	andl $0x7FFFFF, %edi		#var value = uf & 0x7FFFFF
	movl %eax, %esi			#move uf into %esi for var sign = uf
	andl $0x80000000, %esi		#var sign = uf & 0x80000000
	cmpl $0xFF, %edx		#check to see if exponent is NaN
	je EXPONENTNAN			#if it is, jump to EXPONENTNAN	
	test %edx, %edx			#check to see if exponent = 0
	jz EXPONENTZERO			#if it is, jump to EXPONENTZERO
	jmp EXPONENTADDNAN		#otherwise, jump to EXPONENTADDNAN
	EXPONENTNAN:
	jmp SCALEEND			#if the exponent is NaN then do nothing and jump to end
	EXPONENTZERO:
	testl %edi, %edi		#check to see if the value = 0
	jz SCALEEND			#if it is, then do nothing and jump to the end
	cmpl $0x1FFFFF, %edi		#checks to see if value <= 1/4 of max mantissa 
	jg HALF				#if its greater, then jump to HALF
	movl $2, %ecx			#move 2 into %ecx for shifting
	shll %ecx, %edi			# value = value << 2
	jmp RETURNEXPONENTZERO		#jump to RETURNEXPONENTZERO (for formatting)
	HALF:
	cmpl $0x3FFFFF, %edi		#check to see if value <= 1/2 of max mantissa
	jg TOOBIG			#if its greater, then jump to TOOBIG
	movl $1, %ecx			#move 1 into %ecx
	shll %ecx, %edi			# value = value << 1
	movl $1, %edx			#set exponent to 1
	jmp RETURNEXPONENTZERO		#jump to RETURNEXPONENTZERO (for formatting)
	TOOBIG:				#value is too big to multiply by itself because it will exceed the max mantissa value
	movl $1, %ecx			#move 1 into %ecx for shifting
	shll %ecx, %edi			#value = value << 1
	andl $0x7FFFFF, %edi		#value = value & 0x7FFFFF
	movl $2, %edx			#set the exponent to 2
	RETURNEXPONENTZERO:		#formats the return for a value with an originally 0 exponent
	movl $23, %ecx			#move 23 into %ecx for shifting
	shll %ecx, %edx			# exponent = exponent << 23
	orl %esi, %edx			# exponent = sign | exponent
	orl %edx, %edi			# value = exponent | value
	movl %edi, %eax			#return the value
	jmp SCALEEND			#jump to the end
	EXPONENTADDNAN:			#checks to see if adding 2 to exponent will make it become NaN
	addl $2, %edx			#exponent = exponent + 2
	cmpl $0xFE, %edx		#checks to see if exponent is NaN
	jle NORMAL			#if it is not, then jump to NORMAL
	orl $0x7F800000, %esi		#sign = sign | 0x7f800000
	movl %esi, %eax			#return sign
	jmp SCALEEND			#jump to the end
	NORMAL:
	movl $23, %ecx			#move 23 into %ecx for shifting
	shll %ecx, %edx			#exponent = exponent << 23
	orl %esi, %edx			# exponent = sign | exponent
	orl %edx, %edi			#value = value | exponent
	movl %edi, %eax			#return value
	SCALEEND:
	popl %esi			#pop %esi
	popl %edi			#pop %edi
	ret

