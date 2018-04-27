#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <bios.h>

void test_drive( char *fn )
{
    union REGS r;
    struct SREGS s;

    r.w.ax = 0x1500;
    int386(0x2f,&r,&r);
    if(r.w.bx != 0)
    {
        r.w.ax = 0x1502;
        int386(0x2f,&r,&r);
        printf("%d %d\n",r.h.ah,r.h.al);
    }
}


main(int argc, char *argv[])
{
    test_drive(argv[1]);
}


