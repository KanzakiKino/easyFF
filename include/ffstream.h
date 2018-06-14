// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFSTREAM_H
#define EASYFF_FFSTREAM_H

#include "fferror.h"

typedef struct FFStream FFStream;

#ifdef EASYFF_PROTECTED
    typedef struct AVFrame AVFrame;
    typedef struct AVPacket AVPacket;
    typedef struct AVStream AVStream;

    FFStream* FFStream_newForRead ( AVStream* );
    void FFStream_delete ( FFStream** );

    FFError FFStream_sendPacket ( FFStream*, AVPacket* );
    char FFStream_receiveFrame ( FFStream*, AVFrame* );
#endif

FFError FFStream_checkError ( FFStream* );

int FFStream_getIndex ( FFStream* );

char FFStream_isVideo ( FFStream* );
char FFStream_isAudio ( FFStream* );

#endif
