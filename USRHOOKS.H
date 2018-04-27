/**********************************************************************
   module: USRHOOKS.H

   author: James R. Dos‚
   phone:  (214)-271-1365 Ext #221
   date:   July 26, 1994

   Public header file for USRHOOKS.C.

   This module contains cover functions for operations the library
   needs that may be restricted by the calling program.  The function
   prototypes in this header should not be modified.
**********************************************************************/

#ifndef __USRHOOKS_H
#define __USRHOOKS_H

/*---------------------------------------------------------------------
   Error definitions
---------------------------------------------------------------------*/

enum USRHOOKS_Errors
   {
   USRHOOKS_Warning = -2,
   USRHOOKS_Error   = -1,
   USRHOOKS_Ok      = 0
   };


/*---------------------------------------------------------------------
   Function Prototypes
---------------------------------------------------------------------*/

int USRHOOKS_GetMem( void **ptr, unsigned long size );
int USRHOOKS_FreeMem( void *ptr );

#endif
