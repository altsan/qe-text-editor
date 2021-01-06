#ifndef QE_VERSION_H
#define QE_VERSION_H

#define VER_FILEVERSION_STR         "0.9.9"
#define VER_PRODUCTVERSION_STR      VER_FILEVERSION_STR

#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR

#define VER_FILEDESCRIPTION_STR     "QE Text Editor"
#define VER_PRODUCTNAME_STR         VER_FILEDESCRIPTION_STR

#define VER_INTERNALNAME_STR        "QE"
#define VER_ORIGINALFILENAME_STR    "qe.exe"

#define VER_COMPANYNAME_STR         "Alexander Taylor"
#define VER_COMPANYDOMAIN_STR       "altsan.org"

#ifdef __OS2__
#define VER_LEGALCOPYRIGHT_STR      "Copyright ¸ 2021 Alexander Taylor"

#else
#define VER_LEGALCOPYRIGHT_STR      "Copyright © 2021 Alexander Taylor"
#define VER_FILEVERSION             0,9,9,0
#define VER_PRODUCTVERSION          VER_FILEVERSION

#endif  // else

#endif  // QE_VERSION_H