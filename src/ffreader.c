// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffreader.h>
#include <ffimage.h>
#include <ffsound.h>
#include <ffstream.h>
#include <util.h>

#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <stdlib.h>

struct FFReader
{
    FFError error;

    AVFormatContext* format;

    unsigned int streamCount;
    FFStream**   streams;

    AVFrame* frame;
};

// This is a private method that deletes streams.
FFError FFReader_deleteStreams ( FFReader* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    if ( !this->streams || !this->streamCount ) {
        return EASYFF_NOERROR;
    }

    FFStream** streams = this->streams;
    for ( size_t i = 0; i < this->streamCount; i++ ) {
        if ( streams[i] ) {
            FFStream_delete( &streams[i] );
        }
    }
    this->streamCount = 0;
    free( streams );

    return EASYFF_NOERROR;
}

// This is a private method that finds streams.
FFError FFReader_findStreams ( FFReader* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    ILLEGAL_GUARD(this) EASYFF_ERROR_ILLEGAL_OBJECT;

    AVFormatContext* format = this->format;
    if ( avformat_find_stream_info( format, NULL ) ) {
        this->error = EASYFF_ERROR_STREAM;
        return EASYFF_ERROR_STREAM;
    }

    FFError ret = FFReader_deleteStreams( this );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return ret;
    }

    this->streamCount = format->nb_streams;
    this->streams     = malloc( this->streamCount * sizeof(FFStream*) );

    for ( size_t i = 0; i < this->streamCount; i++ ) {
        FFStream* stream = FFStream_newForRead( format->streams[i] );
        this->streams[i] = stream;
    }
    return EASYFF_NOERROR;
}

FFReader* FFReader_new ( const char* path )
{
    NULL_GUARD(path) NULL;

    FFReader* this = malloc( sizeof(FFReader) );
    if ( !this ) {
        return NULL;
    }
    this->error       = EASYFF_NOERROR;
    this->format      = NULL;
    this->streamCount = 0;
    this->streams     = NULL;
    this->frame       = av_frame_alloc();

    if ( avformat_open_input( &this->format, path, NULL, NULL ) ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return this;
    }

    FFError ret = FFReader_findStreams ( this );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return this;
    }

    return this;
}
void FFReader_delete ( FFReader** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    FFReader_deleteStreams( *this );
    avformat_close_input( &(*this)->format );
    av_frame_free( &(*this)->frame );
    free( *this );
    *this = NULL;
}

FFError FFReader_checkError ( FFReader* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}

FFStream* FFReader_getStream ( FFReader* this, unsigned int index )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    if ( this->streamCount > index ) {
        return this->streams[index];
    } else {
        return NULL;
    }
}
FFStream* FFReader_findVideoStream ( FFReader* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    for ( size_t i = 0; i < this->streamCount; i++ ) {
        if ( FFStream_isVideo( this->streams[i] ) ) {
            return this->streams[i];
        }
    }
    return NULL;
}
FFStream* FFReader_findAudioStream ( FFReader* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;

    for ( size_t i = 0; i < this->streamCount; i++ ) {
        if ( FFStream_isAudio( this->streams[i] ) ) {
            return this->streams[i];
        }
    }
    return NULL;
}

FFMeta FFReader_getMeta ( FFReader* this )
{
    FFMeta meta = FFMETA_INIT;
    NULL_GUARD(this) meta;
    ILLEGAL_GUARD(this) meta;

    NULL_GUARD(this->format) meta;
    NULL_GUARD(this->format->metadata) meta;

    FFError ret = FFMeta_assignAVDic( &meta, this->format->metadata );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return meta;
    }
    return meta;
}

char FFReader_decode ( FFReader* this, FFStream* stream )
{
    NULL_GUARD(this) 0;
    ILLEGAL_GUARD(this) 0;

    if ( FFStream_receiveFrame( stream, this->frame ) ) {
        return EASYFF_NOERROR;
    }

    AVPacket packet;
    while ( av_read_frame( this->format, &packet ) == 0 )
    {
        if ( FFStream_getIndex( stream ) == packet.stream_index ) {
            FFError ret = FFStream_sendPacket( stream, &packet );
            if ( ret != EASYFF_NOERROR ) {
                this->error = ret;
                av_packet_unref( &packet );
                return 0;
            }
            if ( FFStream_receiveFrame( stream, this->frame ) ) {
                av_packet_unref( &packet );
                return 1;
            }
        }
        av_packet_unref( &packet );
    }

    // Flushes remained frames.
    FFError ret = FFStream_sendPacket( stream, NULL );
    if ( ret != EASYFF_NOERROR ) {
        this->error = ret;
        return 0;
    }
    return FFStream_receiveFrame( stream, this->frame );
}

FFImage* FFReader_convertFrameToImage ( FFReader* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;
    return FFImage_newFromAVFrame( this->frame );
}
FFSound* FFReader_convertFrameToSound ( FFReader* this )
{
    NULL_GUARD(this) NULL;
    ILLEGAL_GUARD(this) NULL;
    return FFSound_newFromAVFrame( this->frame );
}
