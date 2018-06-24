// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFSTREAM_H
#define EASYFF_FFSTREAM_H

#include "fferror.h"
#include "fftypes.h"

typedef struct FFStream FFStream;

#ifdef EASYFF_PROTECTED
    enum AVPixelFormat;
    enum AVSampleFormat;
    typedef struct AVCodec AVCodec;
    typedef struct AVFrame AVFrame;
    typedef struct AVPacket AVPacket;
    typedef struct AVStream AVStream;

    FFStream* FFStream_newForRead ( AVStream* );
    FFStream* FFStream_newForWrite ( AVStream*, AVCodec* );
    void FFStream_delete ( FFStream** );

    FFError FFStream_sendPacket ( FFStream*, AVPacket* );
    char FFStream_receiveFrame ( FFStream*, AVFrame* );

    enum AVPixelFormat FFStream_getCompatiblePixelFormat( FFStream* );
    enum AVAudioFormat FFStream_getCompatibleAudioFormat( FFStream* );
#endif

FFError FFStream_checkError ( FFStream* );
char    FFStream_isWritable ( FFStream* );

int FFStream_getIndex ( FFStream* );

char FFStream_isVideo ( FFStream* );
char FFStream_isAudio ( FFStream* );

FFRational FFStream_getTimebase   ( FFStream* );
FFRational FFStream_getAvgFPS     ( FFStream* );
long       FFStream_getStartTime  ( FFStream* );
long       FFStream_getDuration   ( FFStream* );
long       FFStream_getFrameCount ( FFStream* );

FFError FFStream_setupVideoEncoder ( FFStream*, int, int, FFRational );
FFError FFStream_setupAudioEncoder ( FFStream*, int, int, FFRational );

#endif
