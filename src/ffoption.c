// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffoption.h>
#include <util.h>

#include <libavutil/dict.h>

#include <stdlib.h>

struct FFOption
{
    AVDictionary* dic;
};

#define DICT_FLAGS AV_DICT_MATCH_CASE

FFOption* FFOption_new ()
{
    FFOption* this = malloc( sizeof(FFOption) );
    NULL_GUARD(this) NULL;

    this->dic = NULL;
    return this;
}
void FFOption_delete ( FFOption** this )
{
    NULL_GUARD(this);
    NULL_GUARD(*this);

    if ( (*this)->dic ) {
        av_dict_free( &(*this)->dic );
    }
    free( *this );
    *this = NULL;
}

AVDictionary** FFOption_getAVDic ( FFOption* this )
{
    NULL_GUARD(this) NULL;
    return &this->dic;
}

FFError FFOption_set ( FFOption* this, const char* key, const char* val )
{
    if ( av_dict_set( &this->dic, key, val, DICT_FLAGS ) < 0 ) {
        return EASYFF_ERROR_INVALID_CONTEXT;
    }
    return EASYFF_NOERROR;
}
