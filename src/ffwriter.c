// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffwriter.h>
#include <ffimage.h>
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

// This is a private method that encodes the AVFrame.
FFError FFWriter_encode ( FFWriter* this, FFStream* stream, AVFrame* frame )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;
    if ( !this->wroteHeader ) {
        THROW( EASYFF_ERROR_INVALID_CONTEXT );
    }

    FFError ret = FFStream_sendFrame( stream, frame );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }

    AVPacket* packet = av_packet_alloc();
    int       streamIndex = FFStream_getIndex( stream );

    while ( FFStream_receivePacket( stream, packet ) ) {
        packet->stream_index = streamIndex;
        if ( av_interleaved_write_frame( this->format, packet ) ) {
            THROW( EASYFF_ERROR_PACKET_LOST );
        }
    }
    av_packet_free( &packet );
    return EASYFF_NOERROR;
}

FFWriter* FFWriter_new ( const char* path )
{
    FFWriter* this = malloc( sizeof(FFWriter) );
    NULL_GUARD(this) NULL;

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

FFError FFWriter_encodeImage ( FFWriter* this, FFImage* img )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;
    NULL_GUARD(img) EASYFF_ERROR_NULL_POINTER;

    enum AVPixelFormat fmt = FFStream_getCompatiblePixelFormat( this->video );
    if ( fmt == AV_PIX_FMT_NONE ) {
        THROW( EASYFF_ERROR_INVALID_FRAME );
    }

    AVFrame* frame = av_frame_alloc();
    FFError ret = FFImage_convertToAVFrame( img, fmt, frame );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }

    ret = FFWriter_encode( this, this->video, frame );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }
    av_frame_free( &frame );
    return EASYFF_NOERROR;
}
FFError FFWriter_encodeSound ( FFWriter* this, FFSound* img )
{
    //TODO
    return EASYFF_NOERROR;
}

FFError FFWriter_flush ( FFWriter* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    FFError ret = FFWriter_encode( this, this->video, NULL );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }
    ret = FFWriter_encode( this, this->audio, NULL );
    if ( ret != EASYFF_NOERROR ) {
        THROW( ret );
    }
    if ( av_write_trailer( this->format ) ) {
        THROW( EASYFF_ERROR_INVALID_CONTEXT );
    }
    return EASYFF_NOERROR;
}
