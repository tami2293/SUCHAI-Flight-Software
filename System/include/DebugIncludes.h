#ifndef _DEBUG_INCLUDES_H
#define _DEBUG_INCLUDES_H

#include <stdio.h>  //para sprintf
#include <stdlib.h>  //para itoa

#define con_putc(c) printf("%c", c)
#define con_putcl(c) printf("%c", c)
#define con_getc() ((char)getcUART1())
#define con_strncpy(a,b,n) strncpy(a,b,n); *((a)+(n))='\0';

void con_printf(char *c);

#endif /* _DEBUG_INCLUDES_H */
