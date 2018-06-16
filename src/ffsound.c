// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffsound.h>
#include <fferror.h>
#include <util.h>

#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>

#include <stdlib.h>

struct FFSound
{
    FFError error;

    long pts;

    int    samples;
    int    channels;
    int    sampleRate;
    float* buffer;
};

FFError FFSound_clear ( FFSound* this, int samples, int channels )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;

    if ( this->samples && this->channels ) {
        free( this->buffer );
        this->buffer = NULL;
    }
    if ( samples && channels ) {
        this->buffer = malloc( sizeof(float)*samples*channels );
    }
    this->samples  = samples;
    this->channels = channels;
    return EASYFF_NOERROR;
}
FFError FFSound_copyAVFrame ( FFSound* this, AVFrame* frame )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;
    NULL_GUARD(frame) EASYFF_ERROR_NULL_POINTER;

    if ( frame->nb_samples <= 0 ) {
        this->error = EASYFF_ERROR_INVALID_FRAME;
        return EASYFF_ERROR_INVALID_FRAME;
    }

    enum AVSampleFormat srcForm       = (enum AVSampleFormat)frame->format;
    int64_t             srcChLayout   = frame->channel_layout;
    int64_t             srcSampleRate = frame->sample_rate;
    enum AVSampleFormat dstForm       = AV_SAMPLE_FMT_FLT;
    SwrContext* swr = swr_alloc_set_opts( NULL,
           dstForm, srcChLayout, srcSampleRate,
           srcChLayout, srcForm, srcSampleRate, 0, NULL);
    if ( !swr || swr_init( swr ) ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return EASYFF_ERROR_CREATE_CONTEXT;
    }

    int srcSamples = frame->nb_samples;
    int dstSamples = swr_get_out_samples( swr, srcSamples );
    int dstChannels = av_get_channel_layout_nb_channels( srcChLayout );
    FFError ret = FFSound_clear( this, dstSamples, dstChannels );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return ret;
    }

    const uint8_t** srcData   = (const uint8_t**)frame->data;
    uint8_t*        dstData[] = {(uint8_t*)this->buffer};
    if ( swr_convert( swr, dstData, dstSamples, srcData, srcSamples ) < 0 ) {
        this->error = EASYFF_ERROR_INVALID_FRAME;
        return EASYFF_ERROR_INVALID_FRAME;
    }
    this->pts        = frame->pts;
    this->sampleRate = srcSampleRate;

    swr_free( &swr );
    return EASYFF_NOERROR;
}

FFSound* FFSound_newFromAVFrame ( AVFrame* frame )
{
    NULL_GUARD(frame) NULL;

    FFSound* this = malloc( sizeof(FFSound) );
    this->error      = EASYFF_NOERROR;
    this->pts        = 0;
    this->samples    = 0;
    this->channels   = 0;
    this->sampleRate = 0;
    this->buffer     = NULL;

    FFError ret = FFSound_copyAVFrame( this, frame );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return this;
    }
    return this;
}
void FFSound_delete ( FFSound** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    FFSound_clear( *this, 0, 0 );
    free( *this );
}

FFError FFSound_checkError ( FFSound* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}

long FFSound_getPts ( FFSound* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->pts;
}
int FFSound_getSamples ( FFSound* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->samples;
}
int FFSound_getChannels ( FFSound* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->channels;
}
int FFSound_getSampleRate ( FFSound* this )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return this->sampleRate;
}
float* FFSound_getBuffer ( FFSound* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;
    return this->buffer;
}
