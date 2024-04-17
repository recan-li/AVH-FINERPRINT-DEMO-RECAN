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

  Copyright(C) 2002. All Rights Reserved.

  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
########################################################################*/

#if !defined FVS__FILE_HEADER__INCLUDED__
#define FVS__FILE_HEADER__INCLUDED__


/* basic type definitions */
#include "fvstypes.h"


/*!
  The implementation of the object is private and must not be known
  by the user. Use this handle to manipulate the file through the
  functions provided hereunder.
*/
typedef /*@mutable@*/ FvsHandle_t FvsFile_t;


/*!
  Create a new file object. Only after having being created, can the file
  object be used by the other functions.
  \return      NULL if allocation failed, otherwise a new object handle.
*/
/*@only@*/ /*@null@*/ FvsFile_t FileCreate(void);


/*!
  Destroy an existing file object. After having been destructed, the file
  object cannot be used anymore.
  \param file  a pointer to a file object to delete
  \return      nothing
*/
void FileDestroy(/*@only@*/ /*@out@*/ /*@null@*/ FvsFile_t file);


/*!
  File options that can be used with the FileOpen function.
*/
typedef enum FvsFileOptions_t
{
    /*! Open for reading */
    FvsFileRead   = (1<<1),
    /*! Open for writing */
    FvsFileWrite  = (1<<2),
    /*! Create if the file does not exist */
    FvsFileCreate = (1<<3)
} FvsFileOptions_t;


/*!
 Open a file. A file can be opened for reading (it must then exist). It can
 also be opened for writing and an empty file may be created. Read and Write
 operations can only be performed on open files.
 \param file    a file object
 \param name    name of the file to open
 \param flags   open flags
 \return        an error code
*/
FvsError_t FileOpen(FvsFile_t file, const FvsString_t name, const FvsFileOptions_t flags);


/*!
 Close an open file. After having been closed, the file operations cannot be
 used anymore.
 \param file  a file object
 \return      an error code
*/
FvsError_t FileClose(FvsFile_t file);


/*!
 Read data from the file. The amount of bytes to be read is specified by length.
 The data read will be stored into the location pointed by data.
 \param file    a file object
 \param data    pointer to an array where the data will be stored
 \param length  number of bytes to read from the file
 \return        the number of bytes read
*/
FvsUint_t FileRead(FvsFile_t file, /*@out@*/ FvsPointer_t data, const FvsUint_t length)
    /*@modifies data@*/;


/*!
 Write data to the file. The amount of data to be written is specified by length.
 The data to write is stored into the location pointed to by data.
 \param file    a file object
 \param data    pointer to an array where the data is stored
 \param length  number of bytes to write to the file
 \return        the number of bytes written
*/
FvsUint_t FileWrite(FvsFile_t file, /*@in@*/ const FvsPointer_t data, const FvsUint_t length);


/*!
 Get a byte from the file.
 \param file    a file object
 \return        the byte read
*/
FvsByte_t FileGetByte(FvsFile_t file);


/*!
 Get a word from the file.
 \param file    a file object
 \return        the word read
*/
FvsWord_t FileGetWord(FvsFile_t file);


/*!
 Test if a file is open.
 \param file  a file object
 \return      a boolean value true when the file is open
*/
FvsBool_t FileIsOpen(const FvsFile_t file);


/*!
 Test if the file pointer is currently at the end of the file.
 \param file  a file object
 \return      a boolean value true when EOF is reached
*/
FvsBool_t FileIsAtEOF(const FvsFile_t file);


/*!
 Commit the changes made to the file.
 \param file  a file object
 \return      an error code
*/
FvsError_t FileCommit(FvsFile_t file);


/*!
 Seek to the begin of the file.
 \param file  a file object
 \return      an error code
*/
FvsError_t FileSeekToBegin(FvsFile_t file);


/*!
 Seek to the end of the file.
 \param file  a file object
 \return      an error code
*/
FvsError_t FileSeekToEnd(FvsFile_t file);


/*!
 Seek to a given position.
 \param file      a file object
 \param position  the new file pointer position from beginning of the file
 \return          an error code
*/
FvsError_t FileSeek(FvsFile_t file, const FvsUint_t position);


/*!
 Get the current position in the file.
 \param file  a file object
 \return      the current position
*/
FvsUint_t FileGetPosition(FvsFile_t file);


#endif /* FVS__FILE_HEADER__INCLUDED__ */

