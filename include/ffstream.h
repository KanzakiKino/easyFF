// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFSTREAM_H
#define EASYFF_FFSTREAM_H

#include "fferror.h"
#include "fftypes.h"

typedef struct FFStream FFStream;
typedef struct FFOption FFOption;

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
    FFError FFStream_sendFrame  ( FFStream*, AVFrame* );
    char FFStream_receiveFrame  ( FFStream*, AVFrame* );
    char FFStream_receivePacket ( FFStream*, AVPacket* );

    enum AVPixelFormat FFStream_getCompatiblePixelFormat( FFStream* );
    enum AVSampleFormat FFStream_getCompatibleSampleFormat( FFStream* );
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

int  FFStream_getSampleRate ( FFStream* );
long FFStream_getFrameSize  ( FFStream* );

FFError FFStream_setupVideoEncoder ( FFStream*, int, int, FFRational, FFOption* );
FFError FFStream_setupAudioEncoder ( FFStream*, int, int, FFOption* );

#endif
