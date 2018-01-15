/******************************************************************************
** EAString.cpp - functions to read and write single-valued ASCII EA
**
**  Public domain code by Roger Orr:
**  http://www.edm2.com/index.php/Extended_Attributes_-_what_are_they_and_how_can_you_use_them
**
******************************************************************************/

#ifdef __OS2__

#define INCL_DOS
#define INCL_DOSERRORS
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "eastring.h"

#pragma pack(1)

/* Header for a single-valued ASCII EA data item */
typedef struct _EA_ASCII_header
  {
  USHORT usAttr;                 /* value: EAT_ASCII                        */
  USHORT usLen;                  /* length of data                          */
                                 /* ASCII data fits in here ...             */
  } EA_ASCII_HEADER, *PEA_ASCII_HEADER;

#pragma pack()


/*****************************************************************************/
/* EAQueryData: query EA data using supplied 'get' EA list into supplied     */
/*              'full' EA buffer - which need NOT be initialised first       */
/*****************************************************************************/

static APIRET EAQueryData( PSZ pszPathName, PGEA2LIST pGEA2List,
                          ULONG cbBuf, PFEA2LIST pFEA2List )
  {
  EAOP2 eaop2 = { NULL, NULL, 0 }; /* EA 'root' data structure              */

  eaop2.fpGEA2List = pGEA2List;
  eaop2.fpFEA2List = pFEA2List;
  pFEA2List->cbList = cbBuf;     /* Inform OS/2 how big our FEA2List is     */

  return DosQueryPathInfo( pszPathName, FIL_QUERYEASFROMLIST,
            (PBYTE) &eaop2, sizeof ( eaop2 ) );
  }


/*****************************************************************************/
/* EASetData: set EA data using supplied 'full' EA buffer                    */
/*****************************************************************************/

static APIRET EASetData( PSZ pszPathName, PFEA2LIST pFEA2List )
  {
  EAOP2 eaop2 = { NULL, NULL, 0 }; /* EA 'root' data structure              */

  eaop2.fpFEA2List = pFEA2List;

  return DosSetPathInfo( pszPathName, FIL_QUERYEASIZE,
            (PBYTE) &eaop2, sizeof ( eaop2 ), DSPI_WRTTHRU );
  }


/*****************************************************************************/
/* EAQueryString: query EA ASCII data into a supplied buffer as a NUL        */
/*                terminated string.                                         */
/*                                                                           */
/* Note: the NUL terminator is NOT required in the data itself - it will be  */
/* added if required.  (Some ASCII EAs include a NUL, some don't !)          */
/*****************************************************************************/

APIRET EAQueryString( PSZ pszPathName, PSZ pszEAName, USHORT cbBuf, PSZ pszBuf )
  {
  APIRET rc = ERROR_NOT_ENOUGH_MEMORY; /* return code                       */
  PFEA2LIST pFEA2List = NULL;    /* pointer to returned EA data             */
  PGEA2LIST pGEA2List = NULL;    /* pointer to list of EAs requested        */
  PEA_ASCII_HEADER pEAData = NULL; /* pointer to data item itself           */
  size_t GEAlen = 0;             /* length of GEA list                      */
  size_t FEAlen = 0;             /* length of FEA list                      */
  PSZ pszAscii = NULL;           /* pointer to ASCII data itself            */


  /*
   * Build an FEA2 list buffer with enough space for cbBuf data items
   * The length is obtained by:
   *     size for FEA2LIST header and one FEA2 item
   *   + room for the EA name (the NUL is included in size of FEA2! )
   *   + EAT_ASCII header
   *   + up to cbBuf bytes of EAT_ASCII data (may or may not end with a NUL)
   */
  FEAlen = sizeof( FEA2LIST ) + strlen( (const char *)pszEAName ) +
              sizeof( EA_ASCII_HEADER ) + cbBuf;

  /* FEAlen MUST be rounded up to a doubleword boundary since
     OS/2 may use buffer space up to this boundary */
  FEAlen = ( ( FEAlen + 3 ) / 4 ) * 4;

  pFEA2List = (PFEA2LIST) malloc( FEAlen );
  if ( pFEA2List != NULL )
     {
     /*
      * Build a GEA2 list for the EA we require
      * The length is obtained by:
      *        size for GEA2LIST header and one GEA2 item
      *      + room for the EA name (the NUL is included in the size of GEA2 !)
      */
     GEAlen = sizeof( GEA2LIST ) + strlen( (const char *)pszEAName );
     pGEA2List = (PGEA2LIST) malloc( GEAlen );
     if ( pGEA2List != NULL )
        {
        pGEA2List->cbList = GEAlen;
        pGEA2List->list[0].oNextEntryOffset = 0;
        pGEA2List->list[0].cbName = (BYTE)strlen( (const char *)pszEAName );
        strcpy( pGEA2List->list[0].szName, (const char *)pszEAName );

        rc = EAQueryData( pszPathName, pGEA2List, FEAlen, pFEA2List );
        if ( rc == 0 )
           {
           if ( pFEA2List->list[0].cbValue == 0 )
              {
              /* THere is no data for this EA, return an error */
              rc = EA_ERROR_NOT_FOUND;
              }
           else
              {
              /* Verify the data type is what we're expecting */
              pEAData = (PEA_ASCII_HEADER) ( (PSZ)pFEA2List->list[0].szName
                           + pFEA2List->list[0].cbName + 1 );
              if ( pEAData->usAttr == EAT_ASCII )
                 {
                 /* skip ASCII header to point to ASCII data */
                 pszAscii = (PSZ) (pEAData + 1);

                 /* If a trailing NUL is present, ignore it */
                 if ( pszAscii[ pEAData->usLen - 1 ] == '\0' )
                    pEAData->usLen--;

                 if ( pEAData->usLen < cbBuf )
                    {
                    /* Give the user the data as a NUL terminated string */
                    memcpy( pszBuf, pEAData + 1, pEAData->usLen );
                    pszBuf[ pEAData->usLen ] = '\0';
                    }
                 else
                    {
                    /* data read is too long for user's buffer */
                    rc = ERROR_BUFFER_OVERFLOW;
                    }
                 }
              else
                 {
                 /* This function only processes EAT_ASCII ! */
                 rc = EA_ERROR_WRONG_TYPE;
                 }
              }
           }

        free( pGEA2List );
        }

     free( pFEA2List );
     }

  return rc;
  }


/*****************************************************************************/
/* EASetString: set EA ASCII data from a NUL terminated string               */
/*                                                                           */
/* Note1: the NUL terminator is NOT stored since the EAT_ASCII type already  */
/* includes a length field.                                                  */
/* Note2: setting a string consisting only of the NUL character will delete  */
/* the EA.                                                                   */
/*****************************************************************************/

APIRET EASetString( PSZ pszPathName, PSZ pszEAName, PSZ pszBuf )
  {
  APIRET rc = ERROR_NOT_ENOUGH_MEMORY; /* return code                       */
  PFEA2LIST pFEA2List = NULL;
  PFEA2 pFEA2 = NULL;
  PEA_ASCII_HEADER pEAData = NULL;
  size_t len = 0;
  size_t cbBuf = 0;


  /* Build an FEA2LIST buffer of the right size (see EAQueryString above) */
  len = sizeof( FEA2LIST ) + strlen( (const char *)pszEAName );
  cbBuf = strlen( (const char *)pszBuf );
  if ( cbBuf != 0 )
     len += sizeof( EA_ASCII_HEADER ) + cbBuf;

  pFEA2List = (PFEA2LIST) malloc( len );
  if ( pFEA2List != NULL )
     {
     pFEA2List->cbList = len;

     pFEA2 = pFEA2List->list;
     pFEA2->oNextEntryOffset = 0; /* no more fields                         */
     pFEA2->fEA = 0;             /* no flags                                */
     pFEA2->cbName = (BYTE) strlen( (const char *) pszEAName );
     strcpy( pFEA2->szName, (const char *) pszEAName );

     if ( cbBuf == 0 )
        pFEA2->cbValue = 0;      /* this will delete the EA!                */
     else
        {
        pFEA2->cbValue = (USHORT)( sizeof( EA_ASCII_HEADER ) + cbBuf );

        /* Fill in the EA data area using an ASCII EA template */
        pEAData = (PEA_ASCII_HEADER) ( (PSZ)pFEA2List->list[0].szName
                     + pFEA2List->list[0].cbName + 1 );
        pEAData->usAttr = EAT_ASCII;
        pEAData->usLen = (USHORT) cbBuf;
        memcpy( pEAData + 1, pszBuf, cbBuf );
        }

     rc = EASetData( pszPathName, pFEA2List );

     free( pFEA2List );
     }

  return rc;
  }

#endif
