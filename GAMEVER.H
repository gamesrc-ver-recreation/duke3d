// *** VERSIONS RESTORATION ***
#ifndef GAMEVER_H
#define GAMEVER_H

// For now adding this here is sufficient, as long as macros originally
// not added by us (like WW2) are still defined using Watcom's -D arguments.
//
// Any source file for which one of our new macros is used, *will* include
// this header first (as of writing this).

// APPVER_DN3DREV definitions
#define AV_DR_NAM10 199805150L // Also for NAPALM
#define AV_DR_WW2GI10 199902280L
#define AV_DR_EDK20021 200006191L // There were two builds during the same day!
#define AV_DR_EDK20023 200006200L

// Now define APPVER_DN3DREV to one of the above,
// based on the definition of APPVER_EXEDEF

#define APPVER_CONCAT1(x,y) x ## y
#define APPVER_CONCAT2(x,y) APPVER_CONCAT1(x,y)
#define APPVER_DN3DREV APPVER_CONCAT2(AV_DR_,APPVER_EXEDEF)

#endif // GAMEVER_H
