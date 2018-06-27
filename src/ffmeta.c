// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffmeta.h>
#include <util.h>

#include <libavutil/dict.h>

#include <stdlib.h>

#define DICT_FLAG AV_DICT_MATCH_CASE

const char* FFMeta_getValue ( const AVDictionary* dic, const char* key )
{
    AVDictionaryEntry* entry = av_dict_get( dic, key, NULL, DICT_FLAG );
    if ( entry ) {
        return entry->value;
    } else {
        return NULL;
    }
}
void FFMeta_setValue ( AVDictionary** dic, const char* key, const char* val )
{
    NULL_GUARD(dic);
    av_dict_set( dic, key, val, DICT_FLAG );
}

FFError FFMeta_assignFromAVDic ( FFMeta* meta, const AVDictionary* dic )
{
    NULL_GUARD( meta ) EASYFF_ERROR_NULL_POINTER;

    meta->title       = FFMeta_getValue( dic, "title" );
    meta->author      = FFMeta_getValue( dic, "author" );
    meta->albumArtist = FFMeta_getValue( dic, "album_artist" );
    meta->album       = FFMeta_getValue( dic, "album" );
    meta->grouping    = FFMeta_getValue( dic, "grouping" );
    meta->composer    = FFMeta_getValue( dic, "composer" );
    meta->year        = FFMeta_getValue( dic, "year" );
    meta->track       = FFMeta_getValue( dic, "track" );
    meta->comment     = FFMeta_getValue( dic, "comment" );
    meta->genre       = FFMeta_getValue( dic, "genre" );
    meta->copyright   = FFMeta_getValue( dic, "copyright" );
    meta->description = FFMeta_getValue( dic, "description" );
    return EASYFF_NOERROR;
}
FFError FFMeta_assignToAVDic ( FFMeta* meta, AVDictionary** dic )
{
    NULL_GUARD( meta ) EASYFF_ERROR_NULL_POINTER;

    FFMeta_setValue( dic, "title", meta->title );
    FFMeta_setValue( dic, "author", meta->author );
    FFMeta_setValue( dic, "album_artist", meta->albumArtist );
    FFMeta_setValue( dic, "album", meta->album );
    FFMeta_setValue( dic, "grouping", meta->grouping );
    FFMeta_setValue( dic, "composer", meta->composer );
    FFMeta_setValue( dic, "year", meta->year );
    FFMeta_setValue( dic, "track", meta->track );
    FFMeta_setValue( dic, "comment", meta->comment );
    FFMeta_setValue( dic, "genre", meta->genre );
    FFMeta_setValue( dic, "copyright", meta->copyright );
    FFMeta_setValue( dic, "description", meta->description );
    return EASYFF_NOERROR;
}
