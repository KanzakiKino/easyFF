// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFSOUND_H
#define EASYFF_FFSOUND_H

#include "fferror.h"

typedef struct FFSound FFSound;

#ifdef EASYFF_PROTECTED
    enum AVSampleFormat;
    typedef struct AVFrame AVFrame;

    FFSound* FFSound_newFromAVFrame ( AVFrame*, int, int );

    FFError FFSound_convertToAVFrame( FFSound*, enum AVSampleFormat, int, AVFrame* );
#endif

FFSound* FFSound_new ( int, int, int, long );

void FFSound_delete ( FFSound** );

FFError FFSound_checkError ( FFSound* );

long   FFSound_getPts        ( FFSound* );
void   FFSound_setPts        ( FFSound*, long );
int    FFSound_getSamples    ( FFSound* );
int    FFSound_getChannels   ( FFSound* );
int    FFSound_getSampleRate ( FFSound* );
float* FFSound_getBuffer     ( FFSound* );

#endif
