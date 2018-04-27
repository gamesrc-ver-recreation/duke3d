/**********************************************************************
   module: USRHOOKS.C

   author: James R. Dos‚
   phone:  (214)-271-1365 Ext #221
   date:   July 26, 1994

   This module contains cover functions for operations the library
   needs that may be restricted by the calling program.  This code
   is left public for you to modify.
**********************************************************************/

#include <stdlib.h>
#include "usrhooks.h"


/*---------------------------------------------------------------------
   Function: USRHOOKS_GetMem

   Allocates the requested amount of memory and returns a pointer to
   its location, or NULL if an error occurs.  NOTE: pointer is assumed
   to be dword aligned.
---------------------------------------------------------------------*/

int USRHOOKS_GetMem
   (
   void **ptr,
   unsigned long size
   )

   {
   void *memory;

   memory = malloc( size );
   if ( memory == NULL )
      {
      return( USRHOOKS_Error );
      }

   *ptr = memory;

   return( USRHOOKS_Ok );
   }


/*---------------------------------------------------------------------
   Function: USRHOOKS_FreeMem

   Deallocates the memory associated with the specified pointer.
---------------------------------------------------------------------*/

int USRHOOKS_FreeMem
   (
   void *ptr
   )

   {
   if ( ptr == NULL )
      {
      return( USRHOOKS_Error );
      }

   free( ptr );

   return( USRHOOKS_Ok );
   }
