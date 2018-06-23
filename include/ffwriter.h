// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFWRITER_H
#define EASYFF_FFWRITER_H

#include "fferror.h"
#include "ffmeta.h"

typedef struct FFImage  FFImage;
typedef struct FFWriter FFWriter;

// Creates FFWriter with file path. Returns null if failed.
FFWriter* FFWriter_new ( const char* );
// Deletes FFWriter.
void FFWriter_delete ( FFWriter** );

// Checks if the error was thrown.
FFError FFWriter_checkError ( FFWriter* );

#endif
