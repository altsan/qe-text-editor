/******************************************************************************
** Header file for EAString.cpp module
**
**  Public domain code by Roger Orr:
**  http://www.edm2.com/index.php/Extended_Attributes_-_what_are_they_and_how_can_you_use_them
**
******************************************************************************/

#ifndef EASTRING_H
#define EASTRING_H
#ifdef __OS2__

#include <os2.h>

APIRET EAQueryString( PSZ pszPathName, PSZ pszEAName, USHORT cbBuf, PSZ pszBuf );
APIRET EASetString( PSZ pszPathName, PSZ pszEAName, PSZ pszBuf );

#define EA_ERROR_NOT_FOUND              (ERROR_USER_DEFINED_BASE + 1)
#define EA_ERROR_WRONG_TYPE             (ERROR_USER_DEFINED_BASE + 2)

#endif
#endif

