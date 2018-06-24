// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFIMAGE_H
#define EASYFF_FFIMAGE_H

#include "fferror.h"

typedef struct FFImage FFImage;

#define FFIMAGE_BYTE_PER_PIX 4

#ifdef EASYFF_PROTECTED
    enum AVPixelFormat;
    typedef struct AVFrame AVFrame;
    FFImage* FFImage_newFromAVFrame   ( AVFrame* );

    FFError  FFImage_convertToAVFrame ( FFImage*, enum AVPixelFormat, AVFrame* );
#endif

FFImage* FFImage_new ( int, int, long );

void FFImage_delete ( FFImage** );

long           FFImage_getPts    ( FFImage* );
void           FFImage_setPts    ( FFImage*, long );
int            FFImage_getWidth  ( FFImage* );
int            FFImage_getHeight ( FFImage* );
unsigned char* FFImage_getBuffer ( FFImage* );

#endif
