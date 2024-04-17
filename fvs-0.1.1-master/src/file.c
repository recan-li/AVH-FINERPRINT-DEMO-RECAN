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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"


/*
 * The implementation of the object is private and must not be known
 * by the user. Use this handle to manipulate the file through the
 * functions provided hereunder.
 */
typedef struct iFvsFile_t
{
    FILE    *pf;        /* file pointer */
} iFvsFile_t;


FvsFile_t FileCreate()
{
    iFvsFile_t* p = NULL;
    p = (iFvsFile_t*)malloc(sizeof(iFvsFile_t));
    if (p!=NULL)
        p->pf = NULL;
    return (FvsFile_t)p;
}


void FileDestroy(FvsFile_t file)
{
    iFvsFile_t* p = NULL;
    if (file==NULL)
        return;
    /* close file if it was still open */
    (void)FileClose(file);
    p = file;
    free(p);
}


FvsError_t FileOpen(FvsFile_t file, const FvsString_t name, const FvsFileOptions_t flags)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    char strFlags[10];
    int nflags = (int)flags;
    /* close eventually already open file */
    (void)FileClose(p);

    strcpy(strFlags, "");
    if ( (nflags & FvsFileRead)!=0   &&
         (nflags & FvsFileWrite)!=0 )
        strcat(strFlags, "rw");
    else
    {
        if ((nflags & FvsFileRead)!=0)
            strcat(strFlags, "r");
        if ((nflags & FvsFileWrite)!=0)
            strcat(strFlags, "w");
    }    
    strcat(strFlags, "b");
    if ((nflags & FvsFileCreate)!=0)
        strcat(strFlags, "+");

    p->pf = fopen(name, strFlags);

    if (FileIsOpen(file)==FvsTrue)
        return FvsOK;

    return FvsFailure;
}


FvsError_t FileClose(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    int nerr = -1;
    if (p->pf!=NULL)
    {
        nerr = fclose(p->pf);
        p->pf = NULL;
    }
    if (nerr==0)
        return FvsOK;
    return FvsFailure;
}


FvsBool_t FileIsOpen(const FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    return (p->pf!=NULL)?FvsTrue:FvsFalse;
}


FvsBool_t FileIsAtEOF(const FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    if (FileIsOpen(p)==FvsFalse)
        return FvsFalse;
    return (feof(p->pf)!=0)?FvsTrue:FvsFalse;
}


FvsError_t FileCommit(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    return (fflush(p->pf)==0)?FvsOK:FvsFailure;
}


FvsError_t FileSeekToBegin(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    if (FileIsOpen(p)==FvsTrue)
    {
        if (fseek(p->pf, 0, SEEK_SET)!=0)
            return FvsFailure;
        return FvsOK;
    }
    return FvsFailure;
}


FvsError_t FileSeekToEnd(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    if (FileIsOpen(p)==FvsTrue)
    {
        if (fseek(p->pf, 0, SEEK_END)!=0)
            return FvsFailure;
        return FvsOK;
    }
    return FvsFailure;
}


FvsUint_t FileGetPosition(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    if (FileIsOpen(p)==FvsTrue)
        return (FvsUint_t)ftell(p->pf);
    return 0;
}


FvsError_t FileSeek(FvsFile_t file, const FvsUint_t position)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    if (FileIsOpen(p)==FvsTrue)
    {
        if (fseek(p->pf, (long int)position, SEEK_SET)!=0)
            return FvsFailure;
        return FvsOK;
    }
    return FvsFailure;
}


FvsUint_t FileRead(FvsFile_t file, FvsPointer_t data, const FvsUint_t length)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    return (FvsUint_t)fread(data, (size_t)1, (size_t)length, p->pf);
}


FvsUint_t FileWrite(FvsFile_t file, const FvsPointer_t data, const FvsUint_t length)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    return (FvsUint_t)fwrite(data, (size_t)1, (size_t)length, p->pf);
}


FvsByte_t FileGetByte(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    return (FvsByte_t)fgetc(p->pf);
}


FvsWord_t FileGetWord(FvsFile_t file)
{
    iFvsFile_t* p = (iFvsFile_t*)file;
    FvsWord_t w = (FvsWord_t)fgetc(p->pf);
    return (w<<8)+fgetc(p->pf);
}

