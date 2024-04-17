/*########################################################################

  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.

  The Initial Developer of the Original Code is Shivang Patel.

  Copyright(C) 2002-2003. All Rights Reserved.

  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
########################################################################*/

#if !defined FVS__FVSTYPES_HEADER__INCLUDED__
#define FVS__FVSTYPES_HEADER__INCLUDED__


#include "config.h"


/* Some of the following types may already be defines in stddef.h on some
** systems. The code here will certainly be improved here (when we will
** provide a configure script). The typedefs provided here are only valid
** on some systems. Modify them according to your systems till we provide
** sufficient self detection.
*/

#if defined(HAVE_STDINT_H) || defined(HAVE_INTTYPES_H)

    #if defined(HAVE_STDINT_H)
        #include <stdint.h>
    #endif
    
    #if defined(HAVE_INTTYPES_H)
        #include <inttypes.h>
    #endif
    
#else

    /* for windows users? */
    typedef unsigned char  uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int   uint32_t;

    typedef signed char    int8_t;
    typedef signed short   int16_t;
    typedef signed int     int32_t;

#endif


/*!
  Natural signed integer type.
*/
typedef int            FvsInt_t;


/*!
  Natural unsigned integer type.
*/
typedef unsigned int   FvsUint_t;


/*!
  Signed byte, word and dword..
*/
typedef int8_t         FvsInt8_t;
typedef int16_t        FvsInt16_t;
typedef int32_t        FvsInt32_t;


/*!
  Unsigned byte, word and dword..
*/
typedef uint8_t        FvsUint8_t;
typedef uint16_t       FvsUint16_t;
typedef uint32_t       FvsUint32_t;

typedef uint8_t        FvsByte_t;
typedef uint16_t       FvsWord_t;
typedef uint32_t       FvsDword_t;


/*!
  Floating point type.
*/
typedef double         FvsFloat_t;


/*!
  Pointer type.
*/
typedef void*          FvsPointer_t;


/*!
  Handle type to manipulate opaque structures.
*/
typedef void*          FvsHandle_t;


/*!
  String type.
*/
typedef char*          FvsString_t;


/*!
  A boolean type
*/
typedef enum FvsBool_t
{
    /*! false tag */
    FvsFalse = 0,
    /*! true tag  */
    FvsTrue  = 1
} FvsBool_t;


/*!
  An approximation of PI for systems that do not already define it.
*/
#ifndef M_PI
#define M_PI          3.1415926535897932384626433832795
#endif


/*!
  Error codes. An error code should be returned by each function from the
  interface. The user of the API should always know when an operation did
  not end up successfully.
*/
typedef enum FvsError_t
{
    /*! an undefined error, use with parcimony */
    FvsFailure          = -1,
    /*! no error */
    FvsOK               = 0,
    /*! not enough memory */
    FvsMemory,
    /*! an invalid parameter was used */
    FvsBadParameter,
    /*! bad file format */
    FvsBadFormat,
    /*! input / output error */
    FvsIoError,
} FvsError_t;


#endif /* FVS__FVSTYPES_HEADER__INCLUDED__ */

