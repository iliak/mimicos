// http://f.osdev.org/viewtopic.php?t=23858&p=194027


/* Copyright (c) Microsoft Corporation. All rights reserved. */
/***
;ulldiv - unsigned long divide
;
;Purpose:
;       Does a unsigned long divide of the arguments.  Arguments are
;       not changed.
;
;Entry:
;       Arguments are passed on the stack:
;               1st pushed: divisor (QWORD)
;               2nd pushed: dividend (QWORD)
;
;Exit:
;       EDX:EAX contains the quotient (dividend/divisor)
;       NOTE: this routine removes the parameters from the stack.
;
;Uses:
;       ECX
;
;Exceptions:
;
******************************************************************************/

__declspec(naked) void __cdecl _aulldiv(void)
{
	__asm {
		push    ebx
			push    esi

			; Set up the local stack and save the index registers.When this is done
			; the stack frame will look as follows(assuming that the expression a / b will
			; generate a call to uldiv(a, b)) :
			;
		;               ---------------- -
			; | |
			; | -------------- - |
			; | |
			; | --divisor(b)-- |
			; | |
			; | -------------- - |
			; | |
			; | --dividend(a) - |
			; | |
			; | -------------- - |
			; | return addr** |
			; | -------------- - |
			; | EBX |
			; | -------------- - |
			;       ESP---- > | ESI |
			;               ---------------- -
			;

#define DVNDLO  [esp + 12]      /* stack address of dividend (a) loword */
#define DVNDHI  [esp + 16]      /* stack address of dividend (a) hiword */
#define DVSRLO  [esp + 20]      /* stack address of divisor (b) loword */
#define DVSRHI  [esp + 24]      /* stack address of divisor (b) hiword */

		;
		; Now do the divide.First look to see if the divisor is less than 4194304K.
			; If so, then we can use a simple algorithm with word divides, otherwise
			; things get a little more complex.
			;

		mov     eax, DVSRHI      ; check to see if divisor < 4194304K
			or      eax, eax
			jnz     short L1        ; nope, gotta do this the hard way
			mov     ecx, DVSRLO      ; load divisor
			mov     eax, DVNDHI      ; load high word of dividend
			xor     edx, edx
			div     ecx             ; get high order bits of quotient
			mov     ebx, eax         ; save high bits of quotient
			mov     eax, DVNDLO      ; edx:eax < -remainder : lo word of dividend
			div     ecx             ; get low order bits of quotient
			mov     edx, ebx         ; edx:eax < -quotient hi : quotient lo
			jmp     short L2        ; restore stack and return

			;
		; Here we do it the hard way.Remember, eax contains DVSRHI
			;

	L1:
		mov     ecx, eax         ; ecx:ebx < -divisor
			mov     ebx, DVSRLO
			mov     edx, DVNDHI      ; edx:eax < -dividend
			mov     eax, DVNDLO
	L3 :
		shr     ecx, 1           ; shift divisor right one bit; hi bit < -0
			rcr     ebx, 1
			shr     edx, 1           ; shift dividend right one bit; hi bit < -0
			rcr     eax, 1
			or      ecx, ecx
			jnz     short L3        ; loop until divisor < 4194304K
			div     ebx             ; now divide, ignore remainder
			mov     esi, eax         ; save quotient

			;
		; We may be off by one, so to check, we will multiply the quotient
			; by the divisor and check the result against the orignal dividend
			; Note that we must also check for overflow, which can occur if the
			; dividend is close to 2 * *64 and the quotient is off by 1.
			;

		mul     dword ptr DVSRHI ; QUOT * DVSRHI
			mov     ecx, eax
			mov     eax, DVSRLO
			mul     esi             ; QUOT * DVSRLO
			add     edx, ecx         ; EDX:EAX = QUOT * DVSR
			jc      short L4        ; carry means Quotient is off by 1

			;
		; do long compare here between original dividend and the result of the
			; multiply in edx : eax.If original is larger or equal, we are ok, otherwise
			; subtract one(1) from the quotient.
			;

		cmp     edx, DVNDHI      ; compare hi words of result and original
			ja      short L4        ; if result > original, do subtract
			jb      short L5        ; if result < original, we are ok
			cmp     eax, DVNDLO      ; hi words are equal, compare lo words
			jbe     short L5        ; if less or equal we are ok, else subtract
	L4 :
		dec     esi             ; subtract 1 from quotient
	L5 :
		xor     edx, edx         ; edx:eax < -quotient
			mov     eax, esi

			;
		; Just the cleanup left to do.edx:eax contains the quotient.
			; Restore the saved registers and return.
			;

	L2:

		pop     esi
			pop     ebx

			ret     16
	}
}



/* Copyright (c) Microsoft Corporation. All rights reserved. */
/***
;lldiv - signed long divide
;
;Purpose:
;       Does a signed long divide of the arguments.  Arguments are
;       not changed.
;
;Entry:
;       Arguments are passed on the stack:
;               1st pushed: divisor (QWORD)
;               2nd pushed: dividend (QWORD)
;
;Exit:
;       EDX:EAX contains the quotient (dividend/divisor)
;       NOTE: this routine removes the parameters from the stack.
;
;Uses:
;       ECX
;
;Exceptions:
;
******************************************************************************/
__declspec(naked) void __cdecl _alldiv(void)
{
	__asm {
		push    edi
			push    esi
			push    ebx

			; Set up the local stack and save the index registers.When this is done
			; the stack frame will look as follows(assuming that the expression a / b will
			; generate a call to lldiv(a, b)) :
			;
		;               ---------------- -
			; | |
			; | -------------- - |
			; | |
			; | --divisor(b)-- |
			; | |
			; | -------------- - |
			; | |
			; | --dividend(a) - |
			; | |
			; | -------------- - |
			; | return addr** |
			; | -------------- - |
			; | EDI |
			; | -------------- - |
			; | ESI |
			; | -------------- - |
			;       ESP---- > | EBX |
			;               ---------------- -
			;

#define DVNDLO  [esp + 16]      /* stack address of dividend (a) loword */
#define DVNDHI  [esp + 20]      /* stack address of dividend (a) hiword */
#define DVSRLO  [esp + 24]      /* stack address of divisor (b) loword */
#define DVSRHI  [esp + 28]      /* stack address of divisor (b) hiword */


		; Determine sign of the result(edi = 0 if result is positive, non - zero
			; otherwise) and make operands positive.

			xor     edi, edi         ; result sign assumed positive

			mov     eax, DVNDHI      ; hi word of a
			or      eax, eax         ; test to see if signed
			jge     short L1        ; skip rest if a is already positive
			inc     edi             ; complement result sign flag
			mov     edx, DVNDLO      ; lo word of a
			neg     eax             ; make a positive
			neg     edx
			sbb     eax, 0
			mov     DVNDHI, eax      ; save positive value
			mov     DVNDLO, edx
	L1 :
		mov     eax, DVSRHI      ; hi word of b
			or      eax, eax         ; test to see if signed
			jge     short L2        ; skip rest if b is already positive
			inc     edi             ; complement the result sign flag
			mov     edx, DVSRLO      ; lo word of a
			neg     eax             ; make b positive
			neg     edx
			sbb     eax, 0
			mov     DVSRHI, eax      ; save positive value
			mov     DVSRLO, edx
	L2 :

		;
		; Now do the divide.First look to see if the divisor is less than 4194304K.
			; If so, then we can use a simple algorithm with word divides, otherwise
			; things get a little more complex.
			;
		; NOTE - eax currently contains the high order word of DVSR
			;

		or      eax, eax         ; check to see if divisor < 4194304K
			jnz     short L3        ; nope, gotta do this the hard way
			mov     ecx, DVSRLO      ; load divisor
			mov     eax, DVNDHI      ; load high word of dividend
			xor     edx, edx
			div     ecx             ; eax < -high order bits of quotient
			mov     ebx, eax         ; save high bits of quotient
			mov     eax, DVNDLO      ; edx:eax < -remainder : lo word of dividend
			div     ecx             ; eax < -low order bits of quotient
			mov     edx, ebx         ; edx:eax < -quotient
			jmp     short L4        ; set sign, restore stack and return

			;
		; Here we do it the hard way.Remember, eax contains the high word of DVSR
			;

	L3:
		mov     ebx, eax         ; ebx:ecx < -divisor
			mov     ecx, DVSRLO
			mov     edx, DVNDHI      ; edx:eax < -dividend
			mov     eax, DVNDLO
	L5 :
		shr     ebx, 1           ; shift divisor right one bit
			rcr     ecx, 1
			shr     edx, 1           ; shift dividend right one bit
			rcr     eax, 1
			or      ebx, ebx
			jnz     short L5        ; loop until divisor < 4194304K
			div     ecx             ; now divide, ignore remainder
			mov     esi, eax         ; save quotient

			;
		; We may be off by one, so to check, we will multiply the quotient
			; by the divisor and check the result against the orignal dividend
			; Note that we must also check for overflow, which can occur if the
			; dividend is close to 2 * *64 and the quotient is off by 1.
			;

		mul     dword ptr DVSRHI ; QUOT * DVSRHI
			mov     ecx, eax
			mov     eax, DVSRLO
			mul     esi             ; QUOT * DVSRLO
			add     edx, ecx         ; EDX:EAX = QUOT * DVSR
			jc      short L6        ; carry means Quotient is off by 1

			;
		; do long compare here between original dividend and the result of the
			; multiply in edx : eax.If original is larger or equal, we are ok, otherwise
			; subtract one(1) from the quotient.
			;

		cmp     edx, DVNDHI      ; compare hi words of result and original
			ja      short L6        ; if result > original, do subtract
			jb      short L7        ; if result < original, we are ok
			cmp     eax, DVNDLO      ; hi words are equal, compare lo words
			jbe     short L7        ; if less or equal we are ok, else subtract
	L6 :
		dec     esi             ; subtract 1 from quotient
	L7 :
		xor     edx, edx         ; edx:eax < -quotient
			mov     eax, esi

			;
		; Just the cleanup left to do.edx:eax contains the quotient.Set the sign
			; according to the save value, cleanup the stack, and return.
			;

	L4:
		dec     edi             ; check to see if result is negative
			jnz     short L8        ; if EDI == 0, result should be negative
			neg     edx             ; otherwise, negate the result
			neg     eax
			sbb     edx, 0

			;
		; Restore the saved registers and return.
			;

	L8:
		pop     ebx
			pop     esi
			pop     edi

			ret     16
	}
}


/* Copyright (c) Microsoft Corporation. All rights reserved. */
/***
;ullshr - long shift right
;
;Purpose:
;       Does a unsigned Long Shift Right
;       Shifts a long right any number of bits.
;
;Entry:
;       EDX:EAX - long value to be shifted
;       CL    - number of bits to shift by
;
;Exit:
;       EDX:EAX - shifted value
;
;Uses:
;       CL is destroyed.
;
;Exceptions:
;
*****************************************************************************/

__declspec(naked) void __cdecl _aullshr(void)
{
	__asm {
		;
		; Handle shifts of 64 bits or more(if shifting 64 bits or more, the result
			; depends only on the high order bit of edx).
			;
		cmp     cl, 64
			jae     short RETZERO

			;
		; Handle shifts of between 0 and 31 bits
			;
		cmp     cl, 32
			jae     short MORE32
			shrd    eax, edx, cl
			shr     edx, cl
			ret

			;
		; Handle shifts of between 32 and 63 bits
			;
	MORE32:
		mov     eax, edx
			xor     edx, edx
			and     cl, 31
			shr     eax, cl
			ret

			;
		; return 0 in edx : eax
			;
	RETZERO:
		xor     eax, eax
			xor     edx, edx
			ret
	}
}






/* Copyright(c) Microsoft Corporation.All rights reserved. * /
/***
;ullrem - unsigned long remainder
;
;Purpose:
;       Does a unsigned long remainder of the arguments.  Arguments are
;       not changed.
;
;Entry:
;       Arguments are passed on the stack:
;               1st pushed: divisor (QWORD)
;               2nd pushed: dividend (QWORD)
;
;Exit:
;       EDX:EAX contains the remainder (dividend%divisor)
;       NOTE: this routine removes the parameters from the stack.
;
;Uses:
;       ECX
;
;Exceptions:
;
******************************************************************************/

__declspec(naked) void __cdecl _aullrem(void)
{
	__asm {
		push    ebx

			; Set up the local stack and save the index registers.When this is done
			; the stack frame will look as follows(assuming that the expression a%b will
			; generate a call to ullrem(a, b)) :
			;
		;               ---------------- -
			; | |
			; | -------------- - |
			; | |
			; | --divisor(b)-- |
			; | |
			; | -------------- - |
			; | |
			; | --dividend(a) - |
			; | |
			; | -------------- - |
			; | return addr** |
			; | -------------- - |
			;       ESP---- > | EBX |
			;               ---------------- -
			;

#define DVNDLO  [esp + 8]       /* stack address of dividend (a) loword */
#define DVNDHI  [esp + 12]      /* stack address of dividend (a) hiword */
#define DVSRLO  [esp + 16]      /* stack address of divisor (b) loword */
#define DVSRHI  [esp + 20]      /* stack address of divisor (b) hiword */

		; Now do the divide.First look to see if the divisor is less than 4194304K.
			; If so, then we can use a simple algorithm with word divides, otherwise
			; things get a little more complex.
			;

		mov     eax, DVSRHI      ; check to see if divisor < 4194304K
			or      eax, eax
			jnz     short L1        ; nope, gotta do this the hard way
			mov     ecx, DVSRLO      ; load divisor
			mov     eax, DVNDHI      ; load high word of dividend
			xor     edx, edx
			div     ecx             ; edx < -remainder, eax < -quotient
			mov     eax, DVNDLO      ; edx:eax < -remainder : lo word of dividend
			div     ecx             ; edx < -final remainder
			mov     eax, edx         ; edx:eax < -remainder
			xor     edx, edx
			jmp     short L2        ; restore stack and return

			;
		; Here we do it the hard way.Remember, eax contains DVSRHI
			;

	L1:
		mov     ecx, eax         ; ecx:ebx < -divisor
			mov     ebx, DVSRLO
			mov     edx, DVNDHI      ; edx:eax < -dividend
			mov     eax, DVNDLO
	L3 :
		shr     ecx, 1           ; shift divisor right one bit; hi bit < -0
			rcr     ebx, 1
			shr     edx, 1           ; shift dividend right one bit; hi bit < -0
			rcr     eax, 1
			or      ecx, ecx
			jnz     short L3        ; loop until divisor < 4194304K
			div     ebx             ; now divide, ignore remainder

			;
		; We may be off by one, so to check, we will multiply the quotient
			; by the divisor and check the result against the orignal dividend
			; Note that we must also check for overflow, which can occur if the
			; dividend is close to 2 * *64 and the quotient is off by 1.
			;

		mov     ecx, eax         ; save a copy of quotient in ECX
			mul     dword ptr DVSRHI
			xchg    ecx, eax         ; put partial product in ECX, get quotient in EAX
			mul     dword ptr DVSRLO
			add     edx, ecx         ; EDX:EAX = QUOT * DVSR
			jc      short L4        ; carry means Quotient is off by 1

			/*
			; do long compare here between original dividend and the result of the
			; multiply in edx:eax.  If original is larger or equal, we're ok, otherwise
			; subtract the original divisor from the result.
			*/

			cmp     edx, DVNDHI      ; compare hi words of result and original
			ja      short L4        ; if result > original, do subtract
			jb      short L5        /* if result < original, we're ok */
			cmp     eax, DVNDLO      ; hi words are equal, compare lo words
			jbe     short L5        /* if less or equal we're ok, else subtract */
	L4 :
		sub     eax, DVSRLO      ; subtract divisor from result
			sbb     edx, DVSRHI
	L5 :

		;
		; Calculate remainder by subtracting the result from the original dividend.
			; Since the result is already in a register, we will perform the subtract in
			; the opposite direction and negate the result to make it positive.
			;

		sub     eax, DVNDLO      ; subtract original dividend from result
			sbb     edx, DVNDHI
			neg     edx             ; and negate it
			neg     eax
			sbb     edx, 0

			;
		; Just the cleanup left to do.dx:ax contains the remainder.
			; Restore the saved registers and return.
			;

	L2:

		pop     ebx

			ret     16
	}
}


/* Copyright (c) Microsoft Corporation. All rights reserved. */
/***
;llrem - signed long remainder
;
;Purpose:
;       Does a signed long remainder of the arguments.  Arguments are
;       not changed.
;
;Entry:
;       Arguments are passed on the stack:
;               1st pushed: divisor (QWORD)
;               2nd pushed: dividend (QWORD)
;
;Exit:
;       EDX:EAX contains the remainder (dividend%divisor)
;       NOTE: this routine removes the parameters from the stack.
;
;Uses:
;       ECX
;
;Exceptions:
;
******************************************************************************/

__declspec(naked) void __cdecl _allrem(void)
{
	__asm {
		push    ebx
			push    edi

			; Set up the local stack and save the index registers.When this is done
			; the stack frame will look as follows(assuming that the expression a%b will
			; generate a call to lrem(a, b)) :
			;
		;               ---------------- -
			; | |
			; | -------------- - |
			; | |
			; | --divisor(b)-- |
			; | |
			; | -------------- - |
			; | |
			; | --dividend(a) - |
			; | |
			; | -------------- - |
			; | return addr** |
			; | -------------- - |
			; | EBX |
			; | -------------- - |
			;       ESP---- > | EDI |
			;               ---------------- -
			;

#define DVNDLO  [esp + 12]      /* stack address of dividend (a) loword */
#define DVNDHI  [esp + 16]      /* stack address of dividend (a) hiword */
#define DVSRLO  [esp + 20]      /* stack address of divisor (b) loword */
#define DVSRHI  [esp + 24]      /* stack address of divisor (b) hiword */


		; Determine sign of the result(edi = 0 if result is positive, non - zero
			; otherwise) and make operands positive.

			xor     edi, edi         ; result sign assumed positive

			mov     eax, DVNDHI      ; hi word of a
			or      eax, eax         ; test to see if signed
			jge     short L1        ; skip rest if a is already positive
			inc     edi             ; complement result sign flag bit
			mov     edx, DVNDLO      ; lo word of a
			neg     eax             ; make a positive
			neg     edx
			sbb     eax, 0
			mov     DVNDHI, eax      ; save positive value
			mov     DVNDLO, edx
	L1 :
		mov     eax, DVSRHI      ; hi word of b
			or      eax, eax         ; test to see if signed
			jge     short L2        ; skip rest if b is already positive
			mov     edx, DVSRLO      ; lo word of b
			neg     eax             ; make b positive
			neg     edx
			sbb     eax, 0
			mov     DVSRHI, eax      ; save positive value
			mov     DVSRLO, edx
	L2 :

		;
		; Now do the divide.First look to see if the divisor is less than 4194304K.
			; If so, then we can use a simple algorithm with word divides, otherwise
			; things get a little more complex.
			;
		; NOTE - eax currently contains the high order word of DVSR
			;

		or      eax, eax         ; check to see if divisor < 4194304K
			jnz     short L3        ; nope, gotta do this the hard way
			mov     ecx, DVSRLO      ; load divisor
			mov     eax, DVNDHI      ; load high word of dividend
			xor     edx, edx
			div     ecx             ; edx < -remainder
			mov     eax, DVNDLO      ; edx:eax < -remainder : lo word of dividend
			div     ecx             ; edx < -final remainder
			mov     eax, edx         ; edx:eax < -remainder
			xor     edx, edx
			dec     edi             ; check result sign flag
			jns     short L4        ; negate result, restore stack and return
			jmp     short L8        ; result sign ok, restore stack and return

			;
		; Here we do it the hard way.Remember, eax contains the high word of DVSR
			;

	L3:
		mov     ebx, eax         ; ebx:ecx < -divisor
		mov     ecx, DVSRLO
		mov     edx, DVNDHI      ; edx:eax < -dividend
		mov     eax, DVNDLO
	L5 :
		shr     ebx, 1           ; shift divisor right one bit
		rcr     ecx, 1
		shr     edx, 1           ; shift dividend right one bit
		rcr     eax, 1
		or      ebx, ebx
		jnz     short L5        ; loop until divisor < 4194304K
		div     ecx             ; now divide, ignore remainder

			;
		; We may be off by one, so to check, we will multiply the quotient
			; by the divisor and check the result against the orignal dividend
			; Note that we must also check for overflow, which can occur if the
			; dividend is close to 2 * *64 and the quotient is off by 1.
			;

		mov     ecx, eax         ; save a copy of quotient in ECX
		mul     dword ptr DVSRHI
		xchg    ecx, eax         ; save product, get quotient in EAX
		mul     dword ptr DVSRLO
		add     edx, ecx         ; EDX:EAX = QUOT * DVSR
		jc      short L6        ; carry means Quotient is off by 1

			;
		; do long compare here between original dividend and the result of the
			; multiply in edx : eax.If original is larger or equal, we are ok, otherwise
			; subtract the original divisor from the result.
			;

		cmp     edx, DVNDHI      ; compare hi words of result and original
		ja      short L6        ; if result > original, do subtract
		jb      short L7        ; if result < original, we are ok
		cmp     eax, DVNDLO      ; hi words are equal, compare lo words
		jbe     short L7        ; if less or equal we are ok, else subtract
	L6 :
		sub     eax, DVSRLO      ; subtract divisor from result
		sbb     edx, DVSRHI
	L7 :

		;
		; Calculate remainder by subtracting the result from the original dividend.
			; Since the result is already in a register, we will do the subtract in the
			; opposite direction and negate the result if necessary.
			;

		sub     eax, DVNDLO      ; subtract dividend from result
		sbb     edx, DVNDHI

			;
		; Now check the result sign flag to see if the result is supposed to be positive
			; or negative.It is currently negated(because we subtracted in the 'wrong'
			; direction), so if the sign flag is set we are done, otherwise we must negate
			; the result to make it positive again.
			;

		dec     edi             ; check result sign flag
		jns     short L8        ; result is ok, restore stack and return
	L4:
		neg     edx             ; otherwise, negate the result
		neg     eax
		sbb     edx, 0

			;
		; Just the cleanup left to do.edx:eax contains the quotient.
			; Restore the saved registers and return.
			;

	L8:
		pop     edi
		pop     ebx

			ret     16
	}
}