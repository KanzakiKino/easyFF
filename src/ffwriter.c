// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffwriter.h>
#include <util.h>

#include <libavformat/avformat.h>

struct FFWriter
{
    FFError error;

    AVIOContext*     io;
    AVFormatContext* format;
};

FFWriter* FFWriter_new ( const char* path )
{
    FFWriter* this = malloc( sizeof(FFWriter) );
    if ( !this ) {
        return NULL;
    }
    this->error  = EASYFF_NOERROR;
    this->format = NULL;

    int ret = avformat_alloc_output_context2( &this->format, NULL, NULL, path );
    if ( ret < 0 ) {
        this->error = EASYFF_ERROR_CREATE_CONTEXT;
        return this;
    }

    if ( this->format->flags & AVFMT_NOFILE ) {
        ret = avio_open( &this->io, path, AVIO_FLAG_WRITE );
        if ( ret < 0 ) {
            this->error = EASYFF_ERROR_CREATE_CONTEXT;
            return this;
        }
    }
    this->format->pb = this->io;
    return this;
}
void FFWriter_delete ( FFWriter** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    avformat_free_context( (*this)->format );
    avio_closep( &(*this)->io );
    free( *this );
}

FFError FFWriter_checkError ( FFWriter* this )
{
    NULL_GUARD(this) EASYFF_ERROR_NULL_POINTER;
    return this->error;
}
