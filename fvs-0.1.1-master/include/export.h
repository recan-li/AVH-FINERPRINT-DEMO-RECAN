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

#if !defined FVS__EXPORT_HEADER__INCLUDED__
#define FVS__EXPORT_HEADER__INCLUDED__

#include "file.h"
#include "image.h"

/*!
  Exports a fingerprint image to a file.
  The file format is extrapolated from the filename extension.
  \param filename  the filename of the file to which the image should be saved
  \param image     image to export
  \result          an error code
*/
FvsError_t FvsImageExport(const FvsImage_t image, const FvsFile_t filename);


#endif /* FVS__EXPORT_HEADER__INCLUDED__ */

