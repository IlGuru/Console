#ifndef _BIT_OPER

#define _BIT_OPER

#define BITMSK(n) (1 << n)
#define SETBIT(bit_fld,n) (bit_fld |=  BITMSK(n))
#define CLRBIT(bit_fld,n) (bit_fld &= ~BITMSK(n))
#define TGLBIT(bit_fld,n) (bit_fld ^=  BITMSK(n))
#define TSTBIT(bit_fld,n) (bit_fld &   BITMSK(n))

#endif