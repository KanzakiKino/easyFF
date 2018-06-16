// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFSOUND_H
#define EASYFF_FFSOUND_H

#include "fferror.h"

typedef struct FFSound FFSound;

#ifdef EASYFF_PROTECTED
    typedef struct AVFrame AVFrame;
    FFSound* FFSound_newFromAVFrame ( AVFrame* );
#endif

void FFSound_delete ( FFSound** );

FFError FFSound_checkError ( FFSound* );

int    FFSound_getSamples    ( FFSound* );
int    FFSound_getChannels   ( FFSound* );
int    FFSound_getSampleRate ( FFSound* );
float* FFSound_getBuffer     ( FFSound* );

#endif
