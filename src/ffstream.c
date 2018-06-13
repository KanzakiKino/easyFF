// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#include <ffstream.h>
#include <util.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>

#include <stdlib.h>

struct FFStream
{
    FFError error;

    AVStream* stream;
    AVCodecContext* codec;
};

// This is a private method that creates codec context.
FFError FFStream_createCodec ( FFStream* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( this->codec ) {
        return EASYFF_NOERROR;
    }
    AVCodec* type = avcodec_find_decoder( this->stream->codecpar->codec_type );
    NULL_GUARD(type) EASYFF_ERROR_NO_CODEC;

    this->codec = avcodec_alloc_context3( type );
    NULL_GUARD(this->codec) EASYFF_ERROR_CREATE_CONTEXT;

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
    return EASYFF_ERROR_CREATE_CONTEXT;
}

FFStream* FFStream_newForRead ( AVStream* avstream )
{
    NULL_GUARD(avstream) NULL;

    FFStream* this = malloc( sizeof(FFStream) );
    NULL_GUARD(this) NULL;

    this->stream = avstream;

    FFError ret = FFStream_createCodec( this );
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

int FFStream_getIndex ( FFStream* this )
{
    NULL_GUARD(this) -1;
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

FFError FFStream_sendPacket ( FFStream* this, AVPacket* packet )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( avcodec_send_packet( this->codec, packet ) ) {
        this->error = EASYFF_ERROR_PACKET_LOST;
        return EASYFF_ERROR_PACKET_LOST;
    }
    return EASYFF_NOERROR;
}
char FFStream_receiveFrame ( FFStream* this, AVFrame* frame )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;
    return !avcodec_receive_frame( this->codec, frame );
}
