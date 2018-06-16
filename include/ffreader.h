// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFREADER_H
#define EASYFF_FFREADER_H

#include "fferror.h"
#include "ffmeta.h"

typedef struct FFImage  FFImage;
typedef struct FFReader FFReader;
typedef struct FFSound  FFSound;
typedef struct FFStream FFStream;

// Creates FFReader from file path. Returns null if failed.
FFReader* FFReader_new ( const char* );
// Deletes FFReader.
void FFReader_delete ( FFReader** );

// Checks if the error was thrown.
FFError FFReader_checkError ( FFReader* );

// Returns the specified stream.
FFStream* FFReader_getStream ( FFReader*, unsigned int );
// Finds video stream and returns it.
FFStream* FFReader_findVideoStream ( FFReader* );
// Finds audio stream and returns it.
FFStream* FFReader_findAudioStream ( FFReader* );

// Generates FFMeta.
FFMeta FFReader_getMeta ( FFReader* );

// Decodes the stream.
char FFReader_decode ( FFReader*, FFStream* );

// Converts the decoded frame to image.
// You need to delete the image after using.
FFImage* FFReader_convertFrameToImage ( FFReader* );
// Converts the decoded frame to sound.
// You need to delete the sound after using.
FFSound* FFReader_convertFrameToSound ( FFReader* );

#endif
