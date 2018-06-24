// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFWRITER_H
#define EASYFF_FFWRITER_H

#include "fferror.h"
#include "ffmeta.h"

typedef struct FFImage FFImage;
typedef struct FFSound FFSound;
typedef struct FFStream FFStream;
typedef struct FFWriter FFWriter;

// Creates FFWriter with file path. Returns null if failed.
FFWriter* FFWriter_new ( const char* );
// Deletes FFWriter.
void FFWriter_delete ( FFWriter** );

// Checks if the error was thrown.
FFError FFWriter_checkError ( FFWriter* );

// Creates video stream.
FFStream* FFWriter_createVideoStream ( FFWriter* );
// Creates audio stream.
FFStream* FFWriter_createAudioStream ( FFWriter* );

// Writes a header.
// You can't create streams anymore after calling this method.
FFError FFWriter_writeHeader ( FFWriter* );

// Encodes a image.
FFError FFWriter_encodeImage ( FFWriter*, FFImage* );
// Encodes a sound.
FFError FFWriter_encodeSound ( FFWriter*, FFSound* );

#endif
