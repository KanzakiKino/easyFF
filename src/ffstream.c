// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffstream.h>
#include <ffoption.h>
#include <util.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>

#include <stdlib.h>

struct FFStream
{
    FFError error;
    char    writable;

    AVStream* stream;
    AVCodecContext* codec;
};

#define IS_WRITABLE(T) FFStream_isWritable(T)

// This is a private method that creates encoder context.
// This method doesn't open the encoder context.
FFError FFStream_createEncoder ( FFStream* this, AVCodec* type )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( this->codec ) {
        return EASYFF_NOERROR;
    }
    if ( !type ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }

    this->codec = avcodec_alloc_context3( type );
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }
    return EASYFF_NOERROR;
}
// This is a private method that creates decoder context.
FFError FFStream_createDecoder ( FFStream* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( this->codec ) {
        return EASYFF_NOERROR;
    }
    AVCodec* type = avcodec_find_decoder( this->stream->codecpar->codec_id );
    if ( !type ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }

    this->codec = avcodec_alloc_context3( type );
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }

    if ( avcodec_parameters_to_context( this->codec, this->stream->codecpar ) ) {
        goto ABORT;
    }
    if ( avcodec_open2( this->codec, type, NULL ) ) {
        goto ABORT;
    }
    return EASYFF_NOERROR;

ABORT:
    avcodec_free_context( &this->codec );
    this->codec = NULL;
    THROW( EASYFF_ERROR_CREATE_CONTEXT );
}

// This is a private method that creates new FFStream with filepath and writable.
FFStream* FFStream_new ( AVStream* avstream, char writable )
{
    NULL_GUARD(avstream) NULL;

    FFStream* this = malloc( sizeof(FFStream) );
    NULL_GUARD(this) NULL;

    this->error    = EASYFF_NOERROR;
    this->writable = writable;
    this->stream   = avstream;
    this->codec    = NULL;

    return this;
}
FFStream* FFStream_newForRead ( AVStream* avstream )
{
    FFStream* this = FFStream_new( avstream, 0 );
    NULL_GUARD(this) NULL;

    FFError ret = FFStream_createDecoder( this );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return this;
    }
    return this;
}
FFStream* FFStream_newForWrite ( AVStream* avstream, AVCodec* codec )
{
    FFStream* this = FFStream_new( avstream, 1 );
    NULL_GUARD(this) NULL;

    FFError ret = FFStream_createEncoder( this, codec );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return this;
    }
    return this;
}
void FFStream_delete ( FFStream** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    if ( (*this)->codec ) {
        avcodec_free_context( &(*this)->codec );
    }
    free( *this );
    *this = NULL;
}

FFError FFStream_checkError ( FFStream* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}
char FFStream_isWritable ( FFStream* this )
{
    NULL_GUARD(this) 0;
    return this->writable;
}

int FFStream_getIndex ( FFStream* this )
{
    NULL_GUARD(this) -1;
    NULL_GUARD(this->stream) -1;
    return this->stream->index;
}

char FFStream_isVideo ( FFStream* this )
{
    NULL_GUARD(this) 0;
    return this->stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO;
}
char FFStream_isAudio ( FFStream* this )
{
    NULL_GUARD(this) 0;
    return this->stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO;
}

FFRational FFStream_getTimebase ( FFStream* this )
{
    FFRational result = {0,0};
    if ( this && this->stream ) {
        result.num = this->stream->time_base.num;
        result.den = this->stream->time_base.den;
    }
    return result;
}
FFRational FFStream_getAvgFPS ( FFStream* this )
{
    FFRational result = {0,0};
    if ( this && this->stream ) {
        result.num = this->stream->avg_frame_rate.num;
        result.den = this->stream->avg_frame_rate.den;
    }
    return result;
}
long FFStream_getStartTime ( FFStream* this )
{
    NULL_GUARD(this) -1;
    NULL_GUARD(this->stream) -1;
    return this->stream->start_time;
}
long FFStream_getDuration ( FFStream* this )
{
    NULL_GUARD(this) -1;
    NULL_GUARD(this->stream) -1;
    return this->stream->duration;
}
long FFStream_getFrameCount ( FFStream* this )
{
    NULL_GUARD(this) -1;
    NULL_GUARD(this->stream) -1;
    return this->stream->nb_frames;
}

int FFStream_getSampleRate ( FFStream* this )
{
    NULL_GUARD(this) 0;
    NULL_GUARD(this->codec) 0;
    return this->codec->sample_rate;
}
long FFStream_getFrameSize ( FFStream* this )
{
    NULL_GUARD(this) 0;
    NULL_GUARD(this->codec) 0;
    return this->codec->frame_size;
}

FFError FFStream_sendPacket ( FFStream* this, AVPacket* packet )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }
    if ( IS_WRITABLE(this) ) {
        THROW( EASYFF_ERROR_STREAM );
    }

    if ( avcodec_send_packet( this->codec, packet ) ) {
        THROW( EASYFF_ERROR_PACKET_LOST );
    }
    return EASYFF_NOERROR;
}
FFError FFStream_sendFrame ( FFStream* this, AVFrame* frame )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }
    if ( !IS_WRITABLE(this) ) {
        THROW( EASYFF_ERROR_STREAM );
    }

    if ( avcodec_send_frame( this->codec, frame ) ) {
        THROW( EASYFF_ERROR_INVALID_FRAME );
    }
    return EASYFF_NOERROR;
}
char FFStream_receiveFrame ( FFStream* this, AVFrame* frame )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    if ( !this->codec ) {
        this->error = EASYFF_ERROR_NO_CODEC;
        return 0;
    }
    if ( IS_WRITABLE(this) ) {
        this->error = EASYFF_ERROR_STREAM;
        return 0;
    }
    return !avcodec_receive_frame( this->codec, frame );
}
char FFStream_receivePacket ( FFStream* this, AVPacket* packet )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    if ( !this->codec ) {
        this->error = EASYFF_ERROR_NO_CODEC;
        return 0;
    }
    if ( !IS_WRITABLE(this) ) {
        this->error = EASYFF_ERROR_STREAM;
        return 0;
    }
    int ret = avcodec_receive_packet( this->codec, packet );
    if ( ret ) {
        return 0;
    }
    av_packet_rescale_ts( packet, this->codec->time_base, this->stream->time_base );
    return 1;
}

enum AVPixelFormat FFStream_getCompatiblePixelFormat ( FFStream* this )
{
    NULL_GUARD(this) AV_PIX_FMT_NONE;
    ILLEGAL_GUARD(this) AV_PIX_FMT_NONE;
    NULL_GUARD(this->codec) AV_PIX_FMT_NONE;
    NULL_GUARD(this->codec->codec) AV_PIX_FMT_NONE;
    NULL_GUARD(this->codec->codec->pix_fmts) AV_PIX_FMT_NONE;
    return this->codec->codec->pix_fmts[0];
}
enum AVSampleFormat FFStream_getCompatibleSampleFormat ( FFStream* this )
{
    NULL_GUARD(this) AV_SAMPLE_FMT_NONE;
    ILLEGAL_GUARD(this) AV_SAMPLE_FMT_NONE;
    NULL_GUARD(this->codec) AV_SAMPLE_FMT_NONE;
    NULL_GUARD(this->codec->codec) AV_SAMPLE_FMT_NONE;
    NULL_GUARD(this->codec->codec->sample_fmts) AV_SAMPLE_FMT_NONE;
    return this->codec->codec->sample_fmts[0];
}
FFError FFStream_setupVideoEncoder ( FFStream* this, int w, int h, FFRational tb, FFOption* opts )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    if ( !IS_WRITABLE(this) ) {
        THROW( EASYFF_ERROR_STREAM );
    }
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }
    if ( this->codec->codec_type != AVMEDIA_TYPE_VIDEO ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }

    this->codec->pix_fmt       = FFStream_getCompatiblePixelFormat( this );
    this->codec->width         = w;
    this->codec->height        = h;
    this->codec->time_base.num = tb.num;
    this->codec->time_base.den = tb.den;

    AVDictionary** options = FFOption_getAVDic( opts );
    if ( avcodec_open2( this->codec, this->codec->codec, options ) ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }
    if ( avcodec_parameters_from_context( this->stream->codecpar, this->codec) ) {
        THROW( EASYFF_ERROR_STREAM );
    }
    return EASYFF_NOERROR;
}
FFError FFStream_setupAudioEncoder ( FFStream* this, int chnl, int rate, FFOption* opts )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    if ( !IS_WRITABLE(this) ) {
        THROW( EASYFF_ERROR_STREAM );
    }
    if ( !this->codec ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }
    if ( this->codec->codec_type != AVMEDIA_TYPE_AUDIO ) {
        THROW( EASYFF_ERROR_NO_CODEC );
    }

    this->codec->sample_fmt     = FFStream_getCompatibleSampleFormat( this );
    this->codec->channel_layout = av_get_default_channel_layout( chnl );
    this->codec->channels       = chnl;
    this->codec->sample_rate    = rate;

    AVDictionary** options = FFOption_getAVDic( opts );
    if ( avcodec_open2( this->codec, this->codec->codec, options ) ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }
    if ( avcodec_parameters_from_context( this->stream->codecpar, this->codec) ) {
        THROW( EASYFF_ERROR_CREATE_CONTEXT );
    }
    return EASYFF_NOERROR;
}
