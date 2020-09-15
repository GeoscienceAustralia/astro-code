/*---------------------------------------------------------------------------
file broxbool.h
History:
	Interim version of <stdbool.h> from C/C++ Users Journal May 2002. This
	file may be included from C90, C++ or C99 (but C99 should come with its
	own version).
	22Apr02	First written and used. (Brock Bryce)
---------------------------------------------------------------------------*/

/*----If C++, do nothing since bool is built in:
*/
#if !defined(_cplusplus)

#	if !defined(_bool_true_false_are_defined)
#		define _bool_true_false_are_defined 1

/*----If this is C99 or later, use built-in bool:
*/
#		if _STDC_VERSION_ >= 199901L
#			define bool _Bool
#		else
			/* choose an unsigned type that can be used as a bitfield	*/
#			define bool unsigned char
#		endif

#		define true 1
#		define false 0
#	endif
#endif
