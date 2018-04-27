#ifndef __DEBUG4G__
#define	__DEBUG4G__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
 #define dassert(__ignore) ((void)0)
#else
 extern void __dassert( char *, char *, int );
 #define dassert(expr)   ((expr)?(void)0:__dassert(#expr,__FILE__,__LINE__))
#endif

void dprintf_clrscr( void );
void dprintf_newline( void );
void dprintf_do( const char *__msg );
int dprintf( const char *__format, ... );
int dprintfxy( int x, int y, const char *__format, ... );

#ifdef __cplusplus
}
#endif

#endif // __DEBUG4G__
