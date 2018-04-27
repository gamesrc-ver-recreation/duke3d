/*  Copyright (C) 1993   Marc Stern  (internet: stern@mble.philips.be)  */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys\stat.h>

void __interrupt __far newint24( int errval, int ax, int bp, int si )
{
    outp(0x20,0x20);
}

int test_drive( char *fn )
{

 int fil;
 void (__interrupt __far *oldint24)();

 oldint24 = _dos_getvect(0x24);
 _disable(); _dos_setvect(0x24, newint24); _enable();

 fil = open( fn, O_CREAT, S_IREAD );

 _disable(); _dos_setvect(0x24, oldint24); _enable();

 if ( fil >= 0 )
 {
    close( fil );
    return 0;
 }
 return 1;
}

main()
{
    if( test_drive("A:\hello.txt") )
        puts("Drive Error");
    else puts("Drive Ok");
}


