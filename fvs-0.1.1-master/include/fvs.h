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
           Jaap de Haan  (jdh)
  
########################################################################*/

#if !defined FVS__GLOBAL_HEADER__INCLUDED__
#define FVS__GLOBAL_HEADER__INCLUDED__

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*! \mainpage Fingerprint Verification System - FVS

  \section intro   Introduction
  The library provides a framework to use when creating a fingerprint
  recognition program. It provides easy to use interfaces to load
  fingerprint files, process fingerprint images and analyse the data.
  
  \section install Installation
  See the file INSTALL provided with the source package

*/

/*
** This is the header to include in your main program to be able to
** use the library capabilities. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* include configuration header created in the configure step */
#include "config.h"

/* basic type definitions */
#include "fvstypes.h"

/* file I/O */
#include "import.h"   /* automatic detection of format to import */
#include "export.h"   /* export to a file */

/* image object */
#include "image.h"

/* floating point field object */
#include "floatfield.h"

/* file object (stream) */
#include "file.h"

/* minutia object */
#include "minutia.h"

/* histogram object */
#include "histogram.h"

/* image processing functions */
#include "imagemanip.h"

/* matching algorithms */
#include "matching.h"

/*!
  Get a string that contains the version information.
  The string is in the form "1.2.4" for version 1.2, build 4.
*/
const FvsString_t FvsGetVersion(void);


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif


#endif /* FVS__GLOBAL_HEADER__INCLUDED__*/


