// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffwriter.h>
#include <ffstream.h>
#include <util.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

struct FFWriter
{
    FFError error;

    AVFormatContext* format;
    char wroteHeader;

    FFStream* video;
    FFStream* audio;
};

// This is a private method that creates stream with the specified codec.
FFStream* FFWriter_createStream ( FFWriter* this, enum AVCodecID cid )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    if ( this->wroteHeader ) {
        this->error = EASYFF_ERROR_STREAM;
        return NULL;
    }

    AVCodec* codec = avcodec_find_encoder( cid );
    if ( !codec ) {
        this->error = EASYFF_ERROR_NO_CODEC;
        return NULL;
    }

    AVStream* stream = avformat_new_stream( this->format, codec );
    if ( !stream ) {
        this->error = EASYFF_ERROR_STREAM;
        return NULL;
    }

    FFStream* result = FFStream_newForWrite( stream, codec );
    if ( !result ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return NULL;
    }
    return result;
}

FFWriter* FFWriter_new ( const char* path )
{
    FFWriter* this = malloc( sizeof(FFWriter) );
    if ( !this ) {
        return NULL;
    }
    this->error       = EASYFF_NOERROR;
    this->format      = NULL;
    this->wroteHeader = 0;
    this->video       = NULL;
    this->audio       = NULL;

    int ret = avformat_alloc_output_context2( &this->format, NULL, NULL, path );
    if ( ret < 0 ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return this;
    }

    ret = avio_open( &this->format->pb, path, AVIO_FLAG_WRITE );
    if ( ret < 0 ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return this;
    }
    return this;
}
void FFWriter_delete ( FFWriter** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    if ( (*this)->video ) {
        FFStream_delete( &(*this)->video );
    }
    if ( (*this)->audio ) {
        FFStream_delete( &(*this)->audio );
    }
    avio_closep( &(*this)->format->pb );
    avformat_free_context( (*this)->format );
    free( *this );
}

FFError FFWriter_checkError ( FFWriter* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}

FFStream* FFWriter_createVideoStream ( FFWriter* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    this->video = FFWriter_createStream( this, this->format->oformat->video_codec );
    return this->video;
}
FFStream* FFWriter_createAudioStream ( FFWriter* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    this->audio = FFWriter_createStream( this, this->format->oformat->audio_codec );
    return this->audio;
}

FFError FFWriter_writeHeader ( FFWriter* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( !this->format || !this->format->oformat || !this->format->pb ) {
        THROW( EASYFF_ERROR_NULL_POINTER );
    }
    if ( avformat_write_header( this->format, NULL ) < 0 ) {
        THROW( EASYFF_ERROR_INVALID_CONTEXT );
    }
    this->wroteHeader = 1;
    return EASYFF_NOERROR;
}
