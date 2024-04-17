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

  Authors: Jaap de Haan(jdh)
  
########################################################################*/

#if !defined FVS__IMPORT_HEADER__INCLUDED__
#define FVS__IMPORT_HEADER__INCLUDED__

#include "file.h"
#include "image.h"

/*!
  Imports a fingeprint image from a file.
  The image format is deduced from the file contents or filename extension.
  \param filename  the name of the file from which the image should be read
  \param image     the result will be stored in this image
  \result          an error code
*/
FvsError_t FvsImageImport(FvsImage_t image, const FvsString_t filename);


#endif /* FVS__IMPORT_HEADER__INCLUDED__ */

