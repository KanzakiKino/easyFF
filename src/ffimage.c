// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffimage.h>
#include <util.h>

#include <libavutil/frame.h>
#include <libswscale/swscale.h>

#include <stdlib.h>

struct FFImage
{
    FFError error;

    long pts;

    int width, height;
    uint8_t* buffer;
};

FFError FFImage_clear ( FFImage* this, int w, int h )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    if ( this->width == w && this->height == h ) {
        return EASYFF_NOERROR;
    }

    if ( this->width || this->height || this->buffer ) {
        free( this->buffer );
        this->width  = 0;
        this->height = 0;
        this->buffer = NULL;
    }
    int size = FFIMAGE_BYTE_PER_PIX*w*h;
    if ( size > 0 ) {
        this->width  = w;
        this->height = h;
        this->buffer = malloc( size );
    }
    return EASYFF_NOERROR;
}

FFError FFImage_copyAVFrame ( FFImage* this, AVFrame* frame, int dstW, int dstH )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    if ( !frame ) {
        THROW( EASYFF_ERROR_NULL_POINTER );
    }

    int srcW = frame->width, srcH = frame->height;
    enum AVPixelFormat srcForm = (enum AVPixelFormat) frame->format;
    if ( srcW == 0 || srcH == 0 ) {
        THROW( EASYFF_ERROR_INVALID_FRAME );
    }

    enum AVPixelFormat dstForm = AV_PIX_FMT_RGBA;

    struct SwsContext* sws = sws_getContext( srcW, srcH, srcForm,
           dstW, dstH, dstForm, SWS_BILINEAR, NULL, NULL, NULL );
    if ( !sws ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }

    FFError ret = FFImage_clear( this, dstW, dstH );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }

    uint8_t** srcData   = frame->data;
    int*      srcLs     = frame->linesize;
    uint8_t*  dstData[] = {this->buffer};
    int       dstLs  [] = {dstW*FFIMAGE_BYTE_PER_PIX};
    sws_scale( sws, (const uint8_t* const*)srcData,
            srcLs, 0, dstH, dstData, dstLs );
    this->pts = frame->pts;

    sws_freeContext( sws );
    return EASYFF_NOERROR;
}

FFImage* FFImage_newFromAVFrame ( AVFrame* frame )
{
    NULL_GUARD(frame) NULL;
    if ( frame->width == 0 || frame->height == 0 ) {
        return NULL;
    }

    FFImage* this = malloc( sizeof(FFImage) );
    this->error  = EASYFF_NOERROR;
    this->pts    = 0;
    this->width  = 0;
    this->height = 0;
    this->buffer = NULL;

    int w = frame->width, h = frame->height;
    FFError ret = FFImage_copyAVFrame( this, frame, w, h );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return this;
    }
    return this;
}
void FFImage_delete ( FFImage** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    FFImage_clear( *this, 0, 0 );
    free( *this );
    *this = NULL;
}

FFError FFImage_checkError ( FFImage* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}

long FFImage_getPts ( FFImage* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->pts;
}

int FFImage_getWidth ( FFImage* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->width;
}
int FFImage_getHeight ( FFImage* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->height;
}
uint8_t* FFImage_getBuffer ( FFImage* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;
    return this->buffer;
}
